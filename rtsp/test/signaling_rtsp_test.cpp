#include <iostream>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>

#include "rtsp_packet.h"
#include "signaling_rtsp.h"

using namespace std::literals;

ServerInfor si = {
		"1.0",
		"2.0",
		"Nvidia Test",
		0,
		1,
		true,
		Address{ "127.0.0.1", 1234},
		Address{ "127.0.0.1", 1234},
		Address{ "127.0.0.1", 1234},
		Address{ "127.0.0.1", 1234},
		"AB:CD:EF:12:34:56",
		"Stream Server",
		false,
		"1234-4567890-ABCDEFGH",
		{{true, 1024, 1080, 144}},
		{
			{ 
				4,
				"App 4",
				true,
				false,
				true,
				false,
				true
			}
		}};

LaunchRequest lreq = { "abcdefgh", "ri-1234567890", "app-1", true};
LaunchResponse lres = { "rtsp://172.24.144.1:48010", "1"};
struct TestResult {
	std::mutex lock;
	bool done;
	bool failed;
	std::string failed_reason;
	SignalingClient *client;
	SignalingClient *server;

	bool start_received;

	TestResult() {
		done = false;
		failed = false;
		failed_reason = "";
		client = nullptr;
		server = nullptr;
		start_received = false;
	}
};

void on_error(const char* error_msg, void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->failed = true;
	result->failed_reason = std::string(error_msg);
	result->done = true;
}

void server_on_start(void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->start_received = true;

	// Send SERVERINFOR
	SendServerInfor(result->server, &si);
}

void client_on_start(void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->failed = true;
	result->failed_reason = "Client should not receive START";
	result->done = true;
}

void server_on_server_infor(ServerInfor *r, void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->failed = true;
	result->failed_reason = "Server should not receive SERVERINFOR";
	result->done = true;
}

void client_on_server_infor(ServerInfor *r, void *data) {
	ServerInfor sent_json = si;
	ServerInfor recv_json = *r;

	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	if (!compare_server_infor(&sent_json,&recv_json)) { // compare
	  result->failed = true;
	  result->failed_reason = "[SERVERINFOR]: request and response mismatch";
	  result->done = true;
	} else {
		// Send SELECTION (LAUNCHREQUEST)
		SendLaunchRequest(result->client, &lreq);
	}
}

void server_on_launch_request(LaunchRequest *r, void *data) {
	LaunchRequest sent_json = lreq;
	LaunchRequest recv_json = *r;

	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	if (!compare_launch_request(&sent_json,&recv_json)) { // compare
	  result->failed = true;
	  result->failed_reason = "[REQUEST]: request mismatch";
	  result->done = true;
	} else {
	  // Send RESPONSE (LAUNCHRESPONSE)
		SendLaunchResponse(result->server, &lres);
	}
}

void client_on_launch_request(LaunchRequest *r, void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->failed = true;
	result->failed_reason = "Client should not receive REQUEST";
	result->done = true;
}

void server_on_launch_response(LaunchResponse *r, void *data) {
	struct TestResult *result = (struct TestResult *)data;
	std::lock_guard<std::mutex> g(result->lock);
	result->failed = true;
	result->failed_reason = "Server should not receive RESPONSE";
	result->done = true;
}

void client_on_launch_response(LaunchResponse *r, void *data) {
	struct TestResult *result = (struct TestResult *)data;
	LaunchResponse sent_json = lres;
	LaunchResponse recv_json = *r;

	std::lock_guard<std::mutex> g(result->lock);
	if (!compare_launch_response(&sent_json,&recv_json)) { // compare
	  result->failed = true;
	  result->failed_reason = "[RESPONSE]: request and response mismatch";
	  result->done = true;
	} else {
	  // Done testing
		result->done = true;
	}
}

int main(int argc, char **argv) {
	GrpcConfig client_config = {
			"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
			"eyJyZWNpcGllbnQiOiIxMzciLCJpc1NlcnZlciI6IkZhbHNlIiwiaWQiOiIxNTAiLCJuYmYi"
			"OjE2NjY2MTYzNzAsImV4cCI6MTY2Njg3NTU3MCwiaWF0IjoxNjY2NjE2MzcwfQ."
			"QqCLRKwhG4N18t1O-gn9ZuVnFFRyIqGhSsT4188l4hM",
			"localhost",
			8000};

	GrpcConfig server_config = {
			"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
			"eyJyZWNpcGllbnQiOiIzIiwiaXNTZXJ2ZXIiOiJUcnVlIiwiaWQiOiIxNTAiLCJuYmYiOjE2"
			"NjY2MTYzNzAsImV4cCI6MTY2Njg3NTU3MCwiaWF0IjoxNjY2NjE2MzcwfQ."
			"D6dC7Sp5DUIDZk7cuS0wZ8MHxssuotG-eSUuBLbiNKY",
			"localhost",
			8000};

	struct TestResult result;
	result.client = new_signaling_client(client_config,
																			 client_on_server_infor,
																			 client_on_launch_request,
																			 client_on_launch_response,
																			 client_on_start,
																			 on_error,
																			 (void *)&result);

	result.server = new_signaling_client(server_config,
																			 server_on_server_infor,
																			 server_on_launch_request,
																			 server_on_launch_response,
																			 server_on_start,
																			 on_error,
																			 (void *)&result);

	if (!result.client || !result.server) {
		std::cout << "Test failed: could not create client or server." << std::endl;
		return 1;
	}

	while (!result.done) {
		std::this_thread::sleep_for(100ms);
		{
			std::lock_guard<std::mutex> g(result.lock);
			if (result.failed) {
				std::cout << "Test failed: " << result.failed_reason << std::endl;
				return 1;
			}
		}
	}

	std::cout << "Test success" << std::endl;
	return 0;
}