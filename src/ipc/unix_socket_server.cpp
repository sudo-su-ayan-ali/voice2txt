#include "unix_socket_server.h"
#include "log.h"

namespace voicetype {

class UnixSocketServer::Impl {
public:
    bool running = false;
    RequestHandler handler;
};

UnixSocketServer::UnixSocketServer() : pimpl_(std::make_unique<Impl>()) {}
UnixSocketServer::~UnixSocketServer() { stop(); }

bool UnixSocketServer::start(const std::string&) {
    VT_LOG_WARN("UnixSocketServer: stub implementation");
    pimpl_->running = true;
    return true;
}

void UnixSocketServer::stop() {
    pimpl_->running = false;
}

bool UnixSocketServer::is_running() const {
    return pimpl_->running;
}

void UnixSocketServer::set_handler(RequestHandler handler) {
    pimpl_->handler = std::move(handler);
}

void UnixSocketServer::broadcast_event(const IpcEvent&) {
    // TODO: implement
}

} // namespace voicetype