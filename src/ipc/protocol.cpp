#include "protocol.h"
#include <sstream>

namespace voicetype {

std::optional<IpcRequest> parse_request(std::string_view data) {
    auto json = JsonValue::parse(data);
    if (!json || !json->is_object()) return std::nullopt;

    IpcRequest req;
    const auto& obj = json->as_object();
    
    if (auto* id = obj.find("id"); id && id->is_int()) req.id = id->as_int();
    if (auto* cmd = obj.find("cmd"); cmd && cmd->is_string()) req.cmd = cmd->as_string();
    if (auto* params = obj.find("params"); params) req.params = *params;
    
    return req;
}

std::string serialize_response(const IpcResponse& resp) {
    JsonValue json = JsonValue::make_object({
        {"id", JsonValue(resp.id)},
        {"ok", JsonValue(resp.ok)}
    });
    if (resp.ok) {
        json["result"] = resp.result;
    } else {
        json["error"] = JsonValue(resp.error);
    }
    return json.dump();
}

std::string serialize_event(const IpcEvent& evt) {
    JsonValue json = JsonValue::make_object({
        {"event", JsonValue(evt.event)},
        {"data", evt.data}
    });
    return json.dump();
}

} // namespace voicetype