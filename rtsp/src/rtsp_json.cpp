#include <cstring>

#include "rtsp_json.h"



void serverinfor_to_map(DataField *j, const ServerInfor *n) {

	(*j)["display[0].active"] = n->displayModes[0].active;
	(*j)["display[0].width"] = n->displayModes[0].width;
	(*j)["display[0].refreshRate"] = n->displayModes[0].refreshRate;
	(*j)["display[0].height"] = n->displayModes[0].height;

    (*j)["app[0].id"] = n->appList[0].id;
    (*j)["app[0].name"] = n->appList[0].name;
    (*j)["app[0].hdrSupported"] = n->appList[0].hdrSupported;
    (*j)["app[0].isAppCollectorGame"] = n->appList[0].isAppCollectorGame;
    (*j)["app[0].hidden"] = n->appList[0].hidden;
    (*j)["app[0].directLaunch"] = n->appList[0].directLaunch;
    (*j)["app[0].active"] = n->appList[0].active;

  	(*j)["gfeVersion"] =  n->gfeVersion;
    (*j)["appVersion"] =  n->appVersion;
    (*j)["gpuModel"] =  n->gpuModel;
    (*j)["maxLumaPixelsHEVC"] =  n->maxLumaPixelsHEVC;
    (*j)["serverCodecModeSupport"] =  n->serverCodecModeSupport;
    (*j)["isSupportedServerVersion"] =  n->isSupportedServerVersion;

    (*j)["localAddress.Address"] = n->localAddress.m_Address;
    (*j)["remoteAddress.Address"] = n->remoteAddress.m_Address;
    (*j)["ipv6Address.Address"] = n->ipv6Address.m_Address;
    (*j)["manualAddress.Address"] = n->manualAddress.m_Address;

    (*j)["localAddress.Port"] = n->localAddress.m_Port;
    (*j)["remoteAddress.Port"] = n->remoteAddress.m_Port;
    (*j)["ipv6Address.Port"] = n->ipv6Address.m_Port;
    (*j)["manualAddress.Port"] = n->manualAddress.m_Port;

    (*j)["macAddress"] = n->macAddress;
    (*j)["hasCustomName"] = n->hasCustomName;
    (*j)["uuid"] = n->uuid;

}

void serverinfor_from_map(const DataField *j, ServerInfor *n) {
	std::memset(n, 0, sizeof(ServerInfor));
	n->gfeVersion = j->at("gfeVersion");
	n->appVersion = j->at("appVersion");
	n->gpuModel = j->at("gpuModel");
	n->macAddress = j->at("macAddress");

	n->localAddress.m_Address = j->at("localAddress.Address");
	n->localAddress.m_Port = atoi(j->at("localAddress.Port").c_str());

	n->remoteAddress.m_Address = j->at("remoteAddress.Address");
	n->remoteAddress.m_Port = atoi(j->at("remoteAddress.Port").c_str());

	n->ipv6Address.m_Address = j->at("ipv6Address.Address");
	n->ipv6Address.m_Port = atoi(j->at("ipv6Address.Port").c_str());

	n->manualAddress.m_Address = j->at("manualAddress.Address");
	n->manualAddress.m_Port = atoi(j->at("manualAddress.Port").c_str());

	n->maxLumaPixelsHEVC = atoi(j->at("maxLumaPixelsHEVC").c_str());
	n->serverCodecModeSupport = atoi(j->at("serverCodecModeSupport").c_str());
	n->isSupportedServerVersion = atoi(j->at("isSupportedServerVersion").c_str());
	n->hasCustomName = atoi(j->at("hasCustomName").c_str());
	n->uuid = j->at("uuid");

    n->displayModes[0].active = 		atoi(j->at("display[0].active").c_str());
    n->displayModes[0].width = 			atoi(j->at("display[0].width").c_str());
    n->displayModes[0].height = 		atoi(j->at("display[0].height").c_str());
    n->displayModes[0].refreshRate = 	atoi(j->at("display[0].refreshRate").c_str());


	n->appList[0].id = 					atoi(j->at("app[0].id").c_str());
	n->appList[0].name = 				atoi(j->at("app[0].name").c_str());
	n->appList[0].hdrSupported = 		atoi(j->at("app[0].hdrSupported").c_str());
	n->appList[0].isAppCollectorGame = 	atoi(j->at("app[0].isAppCollectorGame").c_str());
	n->appList[0].hidden = 				atoi(j->at("app[0].hidden").c_str());
	n->appList[0].directLaunch = 		atoi(j->at("app[0].directLaunch").c_str());
	n->appList[0].active = 				atoi(j->at("app[0].active").c_str());
}

void launchrequest_to_map(DataField *j, const LaunchRequest *n) {
    (*j)["rikey"] = n->rikey;
    (*j)["rikeyid"] = n->rikeyid;
    (*j)["appid"] = n->appid;
    (*j)["localAudioPlayMode"] = n->localAudioPlayMode;
}

void launchrequest_from_map(const DataField *j, LaunchRequest *n) {
	n->rikey = j->at("rikey");
	n->rikeyid = j->at("rikeyid");
	n->appid = j->at("appid");
	n->localAudioPlayMode = atoi(j->at("localAudioPlayMode").c_str());
}

void launchresponse_to_map(DataField *j, const LaunchResponse *n) {
	(*j)["sessionUrl"] = n->sessionUrl;
	(*j)["gamesession"] = n->gamesession;
}

void launchresponse_from_map(const DataField *j, LaunchResponse *n) {
	n->sessionUrl = j->at("sessionUrl");
	n->gamesession = j->at("gamesession");
}
