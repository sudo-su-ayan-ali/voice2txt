#include "unix_socket_client.h"
#include "log.h"

namespace voicetype {

class UnixSocketClient::Impl {
public:
    bool connected = false;
};

UnixSocketClient::UnixSocketClient() : pimpl_(std::make_unique<Impl>()) {}
UnixSocketClient::~UnixSocketClient() { disconnect(); }

bool UnixSocketClient::connect(const std::string&) {
    VT_LOG_WARN("UnixSocketClient: stub implementation");
    pimpl_->connected = true;
    return true;
}

void UnixSocketClient::disconnect() {
    pimpl_->connected = false;
}

bool UnixSocketClient::is_connected() const {
    return pimpl_->connected;
}

std::optional<IpcResponse> UnixSocketClient::send_request(const IpcRequest&, std::chrono::milliseconds) {
    VT_LOG_WARN("UnixSocketClient::send_request: stub");
    IpcResponse resp;
    resp.ok = false;
    resp.error = "Not implemented";
    return resp;
}

} // namespace voicetype