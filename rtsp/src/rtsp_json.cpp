#include <cstring>

#include "rtsp_json.h"

// json serialize/de-serialize

void to_json(json &j, const DisplayMode &d) {
  j = json{{"active", d.active},
           {"width", d.width},
           {"height", d.height},
           {"refreshRate", d.refreshRate}};
}

void from_json(const json &j, DisplayMode &d) {
  j.at("active").get_to(d.active);
  j.at("width").get_to(d.width);
  j.at("height").get_to(d.height);
  j.at("refreshRate").get_to(d.refreshRate);
}

void to_json(json &j, const Address &a) {
  j = json{{"m_Address", a.m_Address}, {"m_Port", a.m_Port}};
}

void from_json(const json &j, Address &a) {
  j.at("m_Address").get_to(a.m_Address);
  j.at("m_Port").get_to(a.m_Port);
}

void to_json(json &j, const App &a) {
  j = json{{"id", a.id},
           {"name", a.name},
           {"hdrSupported", a.hdrSupported},
           {"isAppCollectorGame", a.isAppCollectorGame},
           {"hidden", a.hidden},
           {"directLaunch", a.directLaunch},
           {"active", a.active}};
}

void from_json(const json &j, App &a) {
  j.at("id").get_to(a.id);
  j.at("name").get_to(a.name);
  j.at("hdrSupported").get_to(a.hdrSupported);
  j.at("isAppCollectorGame").get_to(a.isAppCollectorGame);
  j.at("hidden").get_to(a.hidden);
  j.at("directLaunch").get_to(a.directLaunch);
  j.at("active").get_to(a.active);
}

void to_json(json &j, const ServerInfor &n) {
  auto displayModes = json::array();
  for (int i = 0; i < sizeof(n.displayModes); i++) {
    displayModes.push_back(n.displayModes[i]);
  }

  auto appList = json::array();
  for (int i = 0; i < sizeof(n.appList); i++) {
    appList.push_back(n.appList[i]);
  }

  j = json{{"gfeVersion", n.gfeVersion},
           {"appVersion", n.appVersion},
           {"gpuModel", n.gpuModel},
           {"maxLumaPixelsHEVC", n.maxLumaPixelsHEVC},
           {"serverCodecModeSupport", n.serverCodecModeSupport},
           {"isSupportedServerVersion", n.isSupportedServerVersion},
           {"localAddress", n.localAddress},
           {"remoteAddress", n.remoteAddress},
           {"ipv6Address", n.ipv6Address},
           {"manualAddress", n.manualAddress},
           {"macAddress", n.macAddress},
           {"hasCustomName", n.hasCustomName},
           {"uuid", n.uuid},
           {"displayModes", displayModes},
           {"appList", appList}};
}

void from_json(const json &j, ServerInfor &n) {
  std::memset(&n, 0, sizeof(ServerInfor));
  j.at("gfeVersion").get_to(n.gfeVersion);
  j.at("appVersion").get_to(n.appVersion);
  j.at("gpuModel").get_to(n.gpuModel);
  j.at("maxLumaPixelsHEVC").get_to(n.maxLumaPixelsHEVC);
  j.at("serverCodecModeSupport").get_to(n.serverCodecModeSupport);
  j.at("isSupportedServerVersion").get_to(n.isSupportedServerVersion);
  j.at("localAddress").get_to(n.localAddress);
  j.at("remoteAddress").get_to(n.remoteAddress);
  j.at("ipv6Address").get_to(n.ipv6Address);
  j.at("manualAddress").get_to(n.manualAddress);
  j.at("macAddress").get_to(n.macAddress);
  j.at("hasCustomName").get_to(n.hasCustomName);
  j.at("uuid").get_to(n.uuid);

  auto displayModes = j.at("displayModes");
  for (int i = 0; i < sizeof(n.displayModes); i++) {
    displayModes[i].get_to(n.displayModes[i]);
  }

  auto appList = j.at("appList");
  for (int i = 0; i < sizeof(n.appList); i++) {
    appList[i].get_to(n.appList[i]);
  }
}

void to_json(json &j, const LaunchRequest &n) {
  j = json{{"rikey", n.rikey},
           {"rikeyid", n.rikeyid},
           {"appid", n.appid},
           {"localAudioPlayMode", n.localAudioPlayMode}};
}

void from_json(const json &j, LaunchRequest &n) {
  j.at("rikey").get_to(n.rikey);
  j.at("rikeyid").get_to(n.rikeyid);
  j.at("appid").get_to(n.appid);
  j.at("localAudioPlayMode").get_to(n.localAudioPlayMode);
}

void to_json(json &j, const LaunchResponse &n) {
  j = json{{"sessionUrl", n.sessionUrl}, {"gamesession", n.gamesession}};
}

void from_json(const json &j, LaunchResponse &n) {
  j.at("sessionUrl").get_to(n.sessionUrl);
  j.at("gamesession").get_to(n.gamesession);
}
