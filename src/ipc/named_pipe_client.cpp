#include "named_pipe_client.h"
#include "log.h"

namespace voicetype {

class NamedPipeClient::Impl {
public:
    bool connected = false;
};

NamedPipeClient::NamedPipeClient() : pimpl_(std::make_unique<Impl>()) {}
NamedPipeClient::~NamedPipeClient() { disconnect(); }

bool NamedPipeClient::connect(const std::string&) {
    VT_LOG_WARN("NamedPipeClient: stub implementation");
    pimpl_->connected = true;
    return true;
}

void NamedPipeClient::disconnect() {
    pimpl_->connected = false;
}

bool NamedPipeClient::is_connected() const {
    return pimpl_->connected;
}

std::optional<IpcResponse> NamedPipeClient::send_request(const IpcRequest&, std::chrono::milliseconds) {
    VT_LOG_WARN("NamedPipeClient::send_request: stub");
    IpcResponse resp;
    resp.ok = false;
    resp.error = "Not implemented";
    return resp;
}

} // namespace voicetype