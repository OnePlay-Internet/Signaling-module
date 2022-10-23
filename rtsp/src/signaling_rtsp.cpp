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

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <signaling_rtsp.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/client_context.h>


#include <protobuf.grpc.pb.h>
#include <stdio.h>
#include <cstdint>

using namespace std::literals;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReaderWriter;
using signalingGRPC::UserRequest;
using signalingGRPC::UserResponse;
using signalingGRPC::StreamService;

typedef std::shared_ptr<ClientReaderWriter<UserRequest, UserResponse>> Stream;






class GRPCClient {
  public:
    GRPCClient(std::shared_ptr<Channel> channel)
        : stub_(StreamService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    Stream StreamRequest(ClientContext& context)
    {
      Stream stream( stub_->StreamRequest(&context));
      return stream;
    }

  private:
    std::unique_ptr<StreamService::Stub> stub_;
};

struct _SignalingClient {
  OnNvComputer  on_comp;
  OnNvSelection  on_select;
  OnNvResponse  on_response;
  void* data;

  int request_count;
  bool start_received;
  bool connected;


  Stream stream;
  GRPCClient* grpc_client;
};


SignalingClient*        
new_signaling_client(std::string token,
                    OnNvComputer computer,
                    OnNvSelection selection,
                    OnNvResponse response,
                    void* data)
{
  SignalingClient* impl = (SignalingClient*)malloc(sizeof(SignalingClient));
  memset(impl,0,sizeof(SignalingClient));
  impl->on_comp  = computer;
  impl->on_select  = selection;
  impl->on_response  = response;
  impl->data = data;
  impl->grpc_client = new GRPCClient(grpc::CreateChannel("dsf", grpc::InsecureChannelCredentials()));

  ClientContext context;
  context.AddMetadata("authorization",token);
  auto stream = impl->grpc_client->StreamRequest(context);
  if(stream == nullptr) {
    free(impl);
    return nullptr;
  }

  impl->stream = stream;


  impl->request_count = 0;
  std::thread recv ([impl](){
    for (;;)
    {
      UserResponse res;
      bool has_output = impl->stream->Read(&res);
      if (!has_output) {
      } else if (res.error().length() != 0) {

      } 

      std::string target = res.data().at("Target");
      if (target == "START") {
        impl->start_received = true;
      } else if (target == "SERVERINFO") {
        NvComputer computer;

        impl->on_comp(&computer,impl->data);
      } else if (target == "RESPONSE") {
        NvResponse response;
        response.gamesession = res.data().at("gamesession");
        response.sessionUrl  = res.data().at("sessionUrl");
        impl->on_response(&response,impl->data);
      } else if (target == "SELECTION") {
        NvSelection select;
        select.rikey   = res.data().at("rikey");
        select.rikeyid = res.data().at("rikeyid");
        select.appid   = res.data().at("appid");
        select.localAudioPlayMode = res.data().at("localAudioPlayMode") == "1";
        impl->on_select(&select,impl->data);
      } else {
        
      }
    }
  });
  
  return impl;
}



void 
SendNvComputer     (SignalingClient* client, 
                    NvComputer* a)
{
  UserRequest req;
  req.mutable_data()->emplace("app","asdf");
  req.set_target("SERVERINFO");
  client->stream->Write(req);

}
void 
SendNvSelection    (SignalingClient* client, 
                    NvSelection* a)
{

}
void 
SendNvResponse     (SignalingClient* client, 
                    NvSelection* a)
{

}



void 
WaitForStart(SignalingClient* client) {
  while (!client->start_received)
    std::this_thread::sleep_for(10ms);
}
void 
WaitForConnected(SignalingClient* client) {
  while (!client->connected)
    std::this_thread::sleep_for(10ms);
}
void 
CloseSignaling(SignalingClient* client) {
  client->stream->Finish();
  delete client->grpc_client;
  free(client);
}