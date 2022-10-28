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

typedef struct _DisplayMode
{
    bool active;

    int width;
    int height;
    int refreshRate;
}DisplayMode;

typedef struct _Address
{
    std::string m_Address;
    uint16_t m_Port;
}Address;

typedef struct _App {
    int id = 0;


    std::string name;
    bool hdrSupported;
    bool isAppCollectorGame ;
    bool hidden;
    bool directLaunch;

    bool active;
}App;


typedef struct _ServerInfor{
    std::string gfeVersion; // xml:root.GfeVersion
    std::string appVersion; // xml:root.appversion
    std::string gpuModel; // xml:root.gputype

    int maxLumaPixelsHEVC; // xml:root.MaxLumaPixelsHEVC | 0
    int serverCodecModeSupport; // xml:root.ServerCodecModeSupport | 0
    bool isSupportedServerVersion; // check gfeVersion

    // Persisted traits
    // Can restore from saved settings
    Address localAddress; // xml:root.LocalIP + request HTTP port
    Address remoteAddress; // xml:root.ExternalIP + externalPort
    Address ipv6Address; // get from saved settings
    Address manualAddress; // get from saved settings
    std::string macAddress; // xml:root.mac

    std::string name; // xml:root.hostname | "UNKNOWN"

    bool hasCustomName; // get from saved settings
    std::string uuid; // xml:root.uniqueid

    DisplayMode displayModes[5]; // xml:root.SupportedDisplayMode
    App appList[5]; // polling /applist from server 
}ServerInfor;

typedef struct _LaunchRequest{
    std::string rikey;
    std::string rikeyid;
    std::string appid;
    bool localAudioPlayMode;
}LaunchRequest;

typedef struct _LaunchResponse{
    std::string sessionUrl;
    std::string gamesession;
}LaunchResponse;


typedef struct _SignalingClient SignalingClient;

typedef void (*OnStart) (void* data); 
typedef void (*OnServerInfo) (ServerInfor* a, void* data); 
typedef void (*OnLaunchRequest) (LaunchRequest* a, void* data); 
typedef void (*OnLaunchResponse) (LaunchResponse* a, void* data); 

void WaitForStart           (SignalingClient* client); 
void SendServerInfor        (SignalingClient* client, ServerInfor* a); 
void SendLaunchRequest      (SignalingClient* client, LaunchRequest* a); 
void SendLaunchResponse     (SignalingClient* client, LaunchResponse* a); 
void WaitForConnected       (SignalingClient* client);
void CloseSignaling         (SignalingClient* client);



typedef struct _GrpcConfig {
    std::string token;
    std::string signaling_ip;
    int grpc_port;
}GrpcConfig;

SignalingClient*   new_signaling_client         (GrpcConfig config,
                                                 OnServerInfo computer,
                                                 OnLaunchRequest selection,
                                                 OnLaunchResponse response,
                                                 OnStart start,
                                                 void *data);

#endif