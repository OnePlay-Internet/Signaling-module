#pragma once


#include "signaling_rtsp.h"
#include <protobuf.grpc.pb.h>
#include <grpcpp/grpcpp.h>

typedef google::protobuf::Map<std::string, std::string> DataField;

void serverinfor_to_map(DataField *j, const ServerInfor  *n);
void serverinfor_from_map(const DataField *j, ServerInfor  *n);

void launchrequest_to_map(DataField *j, const LaunchRequest  *n);
void launchrequest_from_map(const DataField *j, LaunchRequest  *n);

void launchresponse_to_map(DataField *j, const LaunchResponse  *n);
void launchresponse_from_map(const DataField *j, LaunchResponse  *n);