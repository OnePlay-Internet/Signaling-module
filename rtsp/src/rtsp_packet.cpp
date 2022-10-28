#include <cstring>

#include "rtsp_packet.h"




void
to_c_string(std::string str, char* buffer)
{
	memcpy(buffer,str.c_str(),str.size());
}


void serverinfor_to_map(DataField *j, const ServerInfor *n) {
	(*j)["display[0].width"] = std::to_string(n->displayModes[0].width);
	(*j)["display[0].refreshRate"] = std::to_string(n->displayModes[0].refreshRate);
	(*j)["display[0].height"] = std::to_string(n->displayModes[0].height);

    (*j)["app[0].name"] = n->appList[0].name;
    (*j)["app[0].id"] = std::to_string(n->appList[0].id);
    (*j)["app[0].hdrSupported"] = std::to_string(n->appList[0].hdrSupported);
    (*j)["app[0].isAppCollectorGame"] = std::to_string(n->appList[0].isAppCollectorGame);
    (*j)["app[0].hidden"] = std::to_string(n->appList[0].hidden);
    (*j)["app[0].directLaunch"] = std::to_string(n->appList[0].directLaunch);

  	(*j)["gfeVersion"] =  n->gfeVersion;
    (*j)["appVersion"] =  n->appVersion;
    (*j)["gpuModel"] =  n->gpuModel;
    (*j)["macAddress"] = n->macAddress;
    (*j)["name"] = n->name;

    (*j)["localAddress.Address"] = n->localAddress.m_Address;
    (*j)["remoteAddress.Address"] = n->remoteAddress.m_Address;
    (*j)["ipv6Address.Address"] = n->ipv6Address.m_Address;
    (*j)["manualAddress.Address"] = n->manualAddress.m_Address;

    (*j)["localAddress.Port"] =	std::to_string(n->localAddress.m_Port);
    (*j)["remoteAddress.Port"] =	std::to_string(n->remoteAddress.m_Port);
    (*j)["ipv6Address.Port"] =	std::to_string(n->ipv6Address.m_Port);
    (*j)["manualAddress.Port"] =	std::to_string(n->manualAddress.m_Port);

    (*j)["uuid"] = n->uuid;
    (*j)["hasCustomName"] = std::to_string(n->hasCustomName);
    (*j)["maxLumaPixelsHEVC"] =  std::to_string(n->maxLumaPixelsHEVC);
    (*j)["serverCodecModeSupport"] =  std::to_string(n->serverCodecModeSupport);
    (*j)["isSupportedServerVersion"] =  std::to_string(n->isSupportedServerVersion);

}

void serverinfor_from_map(const DataField *j, ServerInfor *n) {
	memset(n,0,sizeof(ServerInfor));
	to_c_string(j->at("gfeVersion"),n->gfeVersion);
	to_c_string(j->at("appVersion"),n->appVersion);
	to_c_string(j->at("gpuModel"),n->gpuModel);
	to_c_string(j->at("macAddress"),n->macAddress);
	to_c_string(j->at("name"),n->name);

	to_c_string(j->at("localAddress.Address"),n->localAddress.m_Address);
	to_c_string(j->at("remoteAddress.Address"),n->remoteAddress.m_Address);
	to_c_string(j->at("ipv6Address.Address"),n->ipv6Address.m_Address);
	to_c_string(j->at("manualAddress.Address"),n->manualAddress.m_Address);

	n->localAddress.m_Port = atoi(j->at("localAddress.Port").c_str());
	n->remoteAddress.m_Port = atoi(j->at("remoteAddress.Port").c_str());
	n->ipv6Address.m_Port = atoi(j->at("ipv6Address.Port").c_str());
	n->manualAddress.m_Port = atoi(j->at("manualAddress.Port").c_str());

	n->maxLumaPixelsHEVC = atoi(j->at("maxLumaPixelsHEVC").c_str());
	n->serverCodecModeSupport = atoi(j->at("serverCodecModeSupport").c_str());
	n->isSupportedServerVersion = atoi(j->at("isSupportedServerVersion").c_str());
	n->hasCustomName = atoi(j->at("hasCustomName").c_str());
	to_c_string(j->at("uuid"),n->uuid);

    n->displayModes[0].active = true;
    n->displayModes[0].width = 			atoi(j->at("display[0].width").c_str());
    n->displayModes[0].height = 		atoi(j->at("display[0].height").c_str());
    n->displayModes[0].refreshRate = 	atoi(j->at("display[0].refreshRate").c_str());


	n->appList[0].active = true;
	to_c_string(j->at("app[0].name"),n->appList[0].name);
	n->appList[0].id = 					atoi(j->at("app[0].id").c_str());
	n->appList[0].hdrSupported = 		atoi(j->at("app[0].hdrSupported").c_str());
	n->appList[0].isAppCollectorGame = 	atoi(j->at("app[0].isAppCollectorGame").c_str());
	n->appList[0].hidden = 				atoi(j->at("app[0].hidden").c_str());
	n->appList[0].directLaunch = 		atoi(j->at("app[0].directLaunch").c_str());
}

void launchrequest_to_map(DataField *j, const LaunchRequest *n) {
    (*j)["rikey"] = n->rikey;
    (*j)["rikeyid"] = n->rikeyid;
    (*j)["appid"] = n->appid;
    (*j)["localAudioPlayMode"] = std::to_string(n->localAudioPlayMode);
}

void launchrequest_from_map(const DataField *j, LaunchRequest *n) {
	to_c_string( j->at("rikey"),n->rikey );
	to_c_string( j->at("rikeyid"),n->rikeyid );
	to_c_string( j->at("appid"),n->appid );
	n->localAudioPlayMode = atoi(j->at("localAudioPlayMode").c_str());
}

void launchresponse_to_map(DataField *j, const LaunchResponse *n) {
	(*j)["sessionUrl"] = n->sessionUrl;
	(*j)["gamesession"] = n->gamesession;
}

void launchresponse_from_map(const DataField *j, LaunchResponse *n) {
	to_c_string(j->at("sessionUrl"),n->sessionUrl);
	to_c_string(j->at("gamesession"),n->gamesession);
}










bool 
compare_server_infor	(ServerInfor* a, 
						ServerInfor* b)
{
	int size = sizeof(ServerInfor);
	for(int i = 0;i<size-1;i++)
	{
		if (*(((char*)a)+i) != *(((char*)b)+i)) {
			return false;
		}
	}
	return true;
}

bool 
compare_launch_request (LaunchRequest* a, 
						LaunchRequest* b)
{
	int size = sizeof(LaunchRequest);
	for(int i = 0;i<size-1;i++)
	{
		if (*(((char*)a)+i) != *(((char*)b)+i)) {
			return false;
		}
	}
	return true;
}

bool 
compare_launch_response (LaunchResponse* a, 
						LaunchResponse* b)
{
	int size = sizeof(LaunchResponse);
	for(int i = 0;i<size-1;i++)
	{
		if (*(((char*)a)+i) != *(((char*)b)+i)) {
			return false;
		}
	}
	return true;
}