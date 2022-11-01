/**
 * @file packet.h
 * @author {Do Huy Hoang} ({huyhoangdo0205@gmail.com})
 * @brief
 * @version 1.0
 * @date 2022-10-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __SIGNALING_RTSP_H__
#define __SIGNALING_RTSP_H__
#include <string>

typedef struct _DisplayMode {
	int active;

	int width;
	int height;
	int refreshRate;
} DisplayMode;

typedef struct _ServerInfor {
	char  gfeVersion[50]; // xml:root.GfeVersion
	char  appVersion[50]; // xml:root.appversion

	int maxLumaPixelsHEVC;         // xml:root.MaxLumaPixelsHEVC | 0
	int serverCodecModeSupport;    // xml:root.ServerCodecModeSupport | 0

	char Ip[50];

	DisplayMode displayModes[5]; // xml:root.SupportedDisplayMode
} ServerInfor;

bool compare_server_infor	(ServerInfor* a, ServerInfor* b);

typedef struct _LaunchRequest {
	char  rikey[50];
	char  rikeyid[50];
	char  appid[50];
	int localAudioPlayMode;
} LaunchRequest;

bool compare_launch_request (LaunchRequest* a, LaunchRequest* b);

typedef struct _LaunchResponse {
	char  sessionUrl[50];
	char  gamesession[50];
} LaunchResponse;

bool compare_launch_response (LaunchResponse* a, LaunchResponse* b);

typedef struct _SignalingClient SignalingClient;

typedef void (*OnStart)(void *data);
typedef void (*OnServerInfo)(ServerInfor *a, void *data);
typedef void (*OnLaunchRequest)(LaunchRequest *a, void *data);
typedef void (*OnLaunchResponse)(LaunchResponse *a, void *data);
typedef void (*OnError)(const char*  error, void *data);

void WaitForStart(SignalingClient *sc);
bool SendServerInfor(SignalingClient *sc, ServerInfor *a);
bool SendLaunchRequest(SignalingClient *sc, LaunchRequest *a);
bool SendLaunchResponse(SignalingClient *sc, LaunchResponse *a);
void WaitForConnected(SignalingClient *sc);
void CloseSignaling(SignalingClient *sc);

typedef struct _GrpcConfig {
	char  	token[300];
	char  	signaling_ip[50];
	int 	grpc_port;
} GrpcConfig;

SignalingClient *new_signaling_client(GrpcConfig config,
                                      OnServerInfo computer,
                                      OnLaunchRequest selection,
                                      OnLaunchResponse response,
                                      OnStart start,
                                      OnError error,
                                      void *data);

#endif