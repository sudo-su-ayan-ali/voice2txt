#pragma once

#include "json.h"
#include <string>
#include <optional>
#include <functional>

namespace voicetype {

struct IpcRequest {
    int id = 0;
    std::string cmd;
    JsonValue params;
};

struct IpcResponse {
    int id = 0;
    bool ok = false;
    JsonValue result;
    std::string error;
};

struct IpcEvent {
    std::string event;
    JsonValue data;
};

using RequestHandler = std::function<std::optional<IpcResponse>(const IpcRequest&)>;

std::optional<IpcRequest> parse_request(std::string_view data);
std::string serialize_response(const IpcResponse& resp);
std::string serialize_event(const IpcEvent& evt);

} // namespace voicetype