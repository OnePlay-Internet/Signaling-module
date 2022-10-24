#include <signaling_rtsp.h>
#include <thread>
#include <stdio.h>
using namespace std::literals;


void
server_on_start(void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
client_on_start(void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
server_on_response(NvResponse* comp, 
                   void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
client_on_response(NvResponse* comp, 
                   void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
server_on_selection(NvSelection* comp, 
                       void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
client_on_selection(NvSelection* comp, 
                       void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
server_on_server_infor(NvComputer* comp, 
                       void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}
void
client_on_server_infor(NvComputer* comp, 
                       void* data)
{
    struct TestResult *result = (struct TestResult*)data;

}

struct TestResult {
    bool done;
    bool failed;
    SignalingClient* client;
    SignalingClient* server;
};

int main(int argc, char** argv) {
    GrpcConfig client_config = {
        "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJyZWNpcGllbnQiOiIxMzciLCJpc1NlcnZlciI6IkZhbHNlIiwiaWQiOiIxNTAiLCJuYmYiOjE2NjY2MTYzNzAsImV4cCI6MTY2Njg3NTU3MCwiaWF0IjoxNjY2NjE2MzcwfQ.QqCLRKwhG4N18t1O-gn9ZuVnFFRyIqGhSsT4188l4hM",
        "54.169.49.176",
        30000
    };

    GrpcConfig server_config = {
        "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJyZWNpcGllbnQiOiIzIiwiaXNTZXJ2ZXIiOiJUcnVlIiwiaWQiOiIxNTAiLCJuYmYiOjE2NjY2MTYzNzAsImV4cCI6MTY2Njg3NTU3MCwiaWF0IjoxNjY2NjE2MzcwfQ.D6dC7Sp5DUIDZk7cuS0wZ8MHxssuotG-eSUuBLbiNKY",
        "54.169.49.176",
        30000
    };

    struct TestResult result = {
        false,
        true,
        nullptr,
        nullptr
    };
    result.client = new_signaling_client(client_config,
        server_on_server_infor,
        server_on_selection,
        server_on_response,
        server_on_start,
        (void*)&result);  

    result.server = new_signaling_client(server_config,
        server_on_server_infor,
        server_on_selection,
        server_on_response,
        server_on_start,
        (void*)&result);  

    while (!result.done) {
        std::this_thread::sleep_for(100ms);
        if (result.failed) {
            printf("test failed");
        }
    }
}