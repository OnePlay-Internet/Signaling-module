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

    (*j)["IpAddress"] = n->Ip;
    (*j)["maxLumaPixelsHEVC"] =  std::to_string(n->maxLumaPixelsHEVC);
    (*j)["serverCodecModeSupport"] =  std::to_string(n->serverCodecModeSupport);


  	(*j)["gfeVersion"] =  n->gfeVersion;
    (*j)["appVersion"] =  n->appVersion;
    (*j)["gpuModel"] =  n->gpuModel;



}

void serverinfor_from_map(const DataField *j, ServerInfor *n) {
	memset(n,0,sizeof(ServerInfor));
	to_c_string(j->at("gfeVersion"),n->gfeVersion);
	to_c_string(j->at("appVersion"),n->appVersion);
	to_c_string(j->at("gpuModel"),n->gpuModel);
	to_c_string(j->at("IpAddress"),n->Ip);

	n->maxLumaPixelsHEVC = atoi(j->at("maxLumaPixelsHEVC").c_str());
	n->serverCodecModeSupport = atoi(j->at("serverCodecModeSupport").c_str());

    n->displayModes[0].active = true;
    n->displayModes[0].width = 			atoi(j->at("display[0].width").c_str());
    n->displayModes[0].height = 		atoi(j->at("display[0].height").c_str());
    n->displayModes[0].refreshRate = 	atoi(j->at("display[0].refreshRate").c_str());
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