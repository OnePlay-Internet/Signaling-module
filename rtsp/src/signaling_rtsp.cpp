/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <thread>

#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <protobuf.grpc.pb.h>

#include "rtsp_packet.h"
#include "signaling_rtsp.h"

using namespace std::literals;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;
using grpc::Status;
using signalingGRPC::StreamService;
using signalingGRPC::UserRequest;
using signalingGRPC::UserResponse;

// Handshake commands
static constexpr auto CMD_START = "START";
static constexpr auto CMD_SERVERINFOR = "SERVERINFOR";
static constexpr auto CMD_LAUNCH_REQUEST = "REQUEST";
static constexpr auto CMD_LAUNCH_RESPONSE = "RESPONSE";

// Fw declaration
static bool handle_response(SignalingClient *sc, UserResponse &r);
static bool RecvStart(SignalingClient *sc);
static bool RecvServerInfor(SignalingClient *sc, UserResponse &r);
static bool RecvLaunchRequest(SignalingClient *sc, UserResponse &r);
static bool RecvLaunchResponse(SignalingClient *sc, UserResponse &r);

// gRPC
typedef std::unique_ptr<ClientReaderWriter<UserRequest, UserResponse>> Stream;

class GRPCClient {
public:
  GRPCClient(std::shared_ptr<Channel> channel)
      : stub_(StreamService::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  Stream StreamRequest(ClientContext &context) {
    auto stream = stub_->StreamRequest(&context);
    return stream;
  }

private:
  std::unique_ptr<StreamService::Stub> stub_;
};

// Streaming Server [Signaling Client] <===> Signaling Server <===> Streaming
// Client [Signaling Client]
struct _SignalingClient {
  OnServerInfo on_serverinfor;
  OnLaunchRequest on_select;
  OnLaunchResponse on_response;
  OnStart on_start;
  OnError on_error;
  void *data;

  // Stream Server
  bool start_received;
  bool serverinfo_sent;
  bool selection_received;
  bool response_sent;

  // Stream Client
  bool serverinfo_received;
  bool selection_sent;
  bool response_received;

  std::string error_msg;
  int request_count;
  bool connected;
  bool stream_server;
  Stream stream;
  GRPCClient *grpc_client;
};

SignalingClient *new_signaling_client(GrpcConfig config,
                                      OnServerInfo on_serverinfor_cb,
                                      OnLaunchRequest on_selection_cb,
                                      OnLaunchResponse on_response_cb,
                                      OnStart on_start_cb,
                                      OnError on_error_cb,
                                      void *data) {
	SignalingClient *sc = (SignalingClient *)malloc(sizeof(SignalingClient));
	memset(sc, 0, sizeof(SignalingClient));

	// Register callback functions
	sc->on_serverinfor = on_serverinfor_cb;
	sc->on_select = on_selection_cb;
	sc->on_response = on_response_cb;
	sc->on_start = on_start_cb;
	sc->on_error = on_error_cb;
	sc->data = data;

	// Init
	sc->start_received = false;
	sc->serverinfo_sent = false;
	sc->serverinfo_received = false;
	sc->selection_sent = false;
	sc->selection_received = false;
	sc->response_sent = false;
	sc->response_received = false;
	sc->error_msg = "none";
	sc->request_count = 0;
	sc->connected = false;
  sc->stream_server = false;

	// Create comm channel
	auto grpc_target =
		std::string(config.signaling_ip) + ":" + std::to_string(config.grpc_port);
	sc->grpc_client = new GRPCClient(
		grpc::CreateChannel(std::string(grpc_target), grpc::InsecureChannelCredentials()));



	// Start receiving thread
	// TODO: handshake with timeout/deadline
	// TODO: handle error
	std::thread recv([config](SignalingClient* signaling) {
		// Setup authorization
		ClientContext context;
		context.AddMetadata("authorization", std::string(config.token));
		signaling->stream = signaling->grpc_client->StreamRequest(context);
		if (signaling->stream == nullptr) {
			signaling->on_error("unable to create stream", signaling->data);
		}

		for (;;) {
			// Read response
			UserResponse res;
			bool has_output = signaling->stream->Read(&res);

			// Handle error
			if (!has_output) {
				std::this_thread::sleep_for(10ms);
				continue;
			} else if (res.error().length() != 0) {
				break;
			}

			// Handle response
			auto err = handle_response(signaling, res);
		}
		// Report error and return
		signaling->on_error(signaling->error_msg.c_str(), signaling->data);
	},sc);
	recv.detach();
	return sc;
}

static bool handle_response(SignalingClient *sc, UserResponse &r) {
  std::string target = r.data().at("Target");

  bool err;
  if (target == CMD_START) {
    err = RecvStart(sc);
  } else if (target == CMD_SERVERINFOR) {
    err = RecvServerInfor(sc, r);
  } else if (target == CMD_LAUNCH_RESPONSE) {
    err = RecvLaunchResponse(sc, r);
  } else if (target == CMD_LAUNCH_REQUEST) {
    err = RecvLaunchRequest(sc, r);
  } else {
    err = true;
    sc->error_msg = "Unknown Target command. Received: " + target;
  }

  return err;
}

void WaitForStart(SignalingClient *sc) {
  while (!sc->start_received)
    std::this_thread::sleep_for(10ms);
}
void WaitForConnected(SignalingClient *sc) {
  while (!sc->connected)
    std::this_thread::sleep_for(10ms);
}
void CloseSignaling(SignalingClient *sc) {
  sc->stream->Finish();
  delete sc->grpc_client;
  free(sc);
}

// from signaling server to stream server
static bool RecvStart(SignalingClient *sc) {
	if(sc == nullptr) {
		sc->error_msg = "RecvStart: invalid state";
		return false;
	}


	sc->start_received = true;
	sc->stream_server = true;
	sc->on_start(sc->data);

	return sc->start_received;
}

// from stream server to stream client
bool SendServerInfor(SignalingClient *sc, ServerInfor *a) {
  if (sc == nullptr || !sc->stream_server || !sc->start_received) {
    sc->error_msg = "SendServerInfor: invalid state";
    return false;
  }

  UserRequest r;
  serverinfor_to_map(r.mutable_data(),a);
  r.set_target(CMD_SERVERINFOR);
  sc->serverinfo_sent = sc->stream->Write(r);
  return sc->serverinfo_sent;
}

// stream client recv from stream server
static bool RecvServerInfor(SignalingClient *sc, UserResponse &r) {
  if (sc == nullptr || sc->stream_server) {
    sc->error_msg = "RecvServerInfor: invalid state";
    return false;
  }

  // do not allow exceptions
  ServerInfor server_infor = {0};
  DataField data = r.data();
  serverinfor_from_map(&data,&server_infor); 
  sc->serverinfo_received = true;
  sc->on_serverinfor(&server_infor, sc->data);
  return sc->serverinfo_received;
}

// from stream client to stream server
bool SendLaunchRequest(SignalingClient *sc, LaunchRequest *a) {
  if (sc == nullptr || sc->stream_server || !sc->serverinfo_received) {
    sc->error_msg = "SendLaunchRequest: invalid state";
    return false;
  }

  UserRequest r;
  r.set_target(CMD_LAUNCH_REQUEST);
  launchrequest_to_map(r.mutable_data(),a);
  sc->selection_sent = sc->stream->Write(r);
  return sc->selection_sent;
}

static bool RecvLaunchRequest(SignalingClient *sc, UserResponse &r) {
  if (sc == nullptr || !sc->stream_server || !sc->start_received ||
      !sc->serverinfo_sent) {
    sc->error_msg = "RecvLaunchRequest: invalid state";
    return false;
  }

  // do not allow exceptions
  LaunchRequest request = {0};
  launchrequest_from_map(&r.data(),&request);
  sc->selection_received = true;
  sc->on_select(&request, sc->data);

  return sc->selection_received;
}

// from stream server to stream client
bool SendLaunchResponse(SignalingClient *sc, LaunchResponse *a) {
  if (sc == nullptr || !sc->stream_server || !sc->start_received ||
      !sc->serverinfo_sent || !sc->selection_received) {
    sc->error_msg = "SendLaunchResponse: invalid state";
    return false;
  }

  UserRequest r;
  launchresponse_to_map(r.mutable_data(),a);
  r.set_target(CMD_LAUNCH_RESPONSE);
  sc->response_sent = sc->stream->Write(r);
  return sc->response_sent;
}

static bool RecvLaunchResponse(SignalingClient *sc, UserResponse &r) {
  if (sc == nullptr || sc->stream_server || !sc->serverinfo_received ||
      !sc->selection_sent) {
    sc->error_msg = "RecvLaunchResponse: invalid state";
    return false;
  }

  // do not allow exceptions
  LaunchResponse nr = {0};

  launchresponse_from_map(&r.data(),&nr);
  sc->response_received = true;
  sc->on_response(&nr, sc->data);

  // TODO: Establish and verify RTSP connection first
  sc->connected = true;

  return sc->selection_received;
}