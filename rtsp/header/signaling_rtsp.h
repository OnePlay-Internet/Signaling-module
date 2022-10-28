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

typedef struct _Address {
	char  m_Address[50];
	uint16_t m_Port;
} Address;

typedef struct _App {
	int id = 0;

	char  name[50];
	int hdrSupported;
	int isAppCollectorGame;
	int hidden;
	int directLaunch;

	int active;
} App;

typedef struct _ServerInfor {
	char  gfeVersion[50]; // xml:root.GfeVersion
	char  appVersion[50]; // xml:root.appversion
	char  gpuModel[50];   // xml:root.gputype

	int maxLumaPixelsHEVC;         // xml:root.MaxLumaPixelsHEVC | 0
	int serverCodecModeSupport;    // xml:root.ServerCodecModeSupport | 0
	int isSupportedServerVersion; // check gfeVersion

	// Persisted traits
	// Can restore from saved settings
	Address localAddress;   // xml:root.LocalIP + request HTTP port
	Address remoteAddress;  // xml:root.ExternalIP + externalPort
	Address ipv6Address;    // get from saved settings
	Address manualAddress;  // get from saved settings
	char  macAddress[50]; // xml:root.mac

	char  name[50]; // xml:root.hostname | "UNKNOWN"

	int hasCustomName; // get from saved settings
	char  uuid[50];   // xml:root.uniqueid

	DisplayMode displayModes[5]; // xml:root.SupportedDisplayMode
	App appList[5];              // polling /applist from server
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
	char  token[300];
	char  signaling_ip[50];
	int grpc_port;
	bool stream_server;
} GrpcConfig;

SignalingClient *new_signaling_client(GrpcConfig config,
                                      OnServerInfo computer,
                                      OnLaunchRequest selection,
                                      OnLaunchResponse response,
                                      OnStart start,
                                      OnError error,
                                      void *data);

#endif