#include "named_pipe_server.h"
#include "log.h"

namespace voicetype {

class NamedPipeServer::Impl {
public:
    bool running = false;
    RequestHandler handler;
};

NamedPipeServer::NamedPipeServer() : pimpl_(std::make_unique<Impl>()) {}
NamedPipeServer::~NamedPipeServer() { stop(); }

bool NamedPipeServer::start(const std::string&) {
    VT_LOG_WARN("NamedPipeServer: stub implementation");
    pimpl_->running = true;
    return true;
}

void NamedPipeServer::stop() {
    pimpl_->running = false;
}

bool NamedPipeServer::is_running() const {
    return pimpl_->running;
}

void NamedPipeServer::set_handler(RequestHandler handler) {
    pimpl_->handler = std::move(handler);
}

void NamedPipeServer::broadcast_event(const IpcEvent&) {
    // TODO: implement
}

} // namespace voicetype