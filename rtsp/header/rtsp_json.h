#pragma once

#include <nlohmann/json.hpp>

#include "signaling_rtsp.h"

using json = nlohmann::json;

// json serialize/de-serialize
void to_json(json &j, const DisplayMode &d);
void from_json(const json &j, DisplayMode &d);

void to_json(json &j, const Address &a);
void from_json(const json &j, Address &a);

void to_json(json &j, const App &a);
void from_json(const json &j, App &a);

void to_json(json &j, const ServerInfor &n);
void from_json(const json &j, ServerInfor &n);

void to_json(json &j, const LaunchRequest &n);
void from_json(const json &j, LaunchRequest &n);

void to_json(json &j, const LaunchResponse &n);
void from_json(const json &j, LaunchResponse &n);