#pragma once

#include "server.h"
#include <memory>

namespace voicetype {

class UnixSocketServer : public IpcServer {
public:
    UnixSocketServer();
    ~UnixSocketServer() override;

    bool start(const std::string& address) override;
    void stop() override;
    bool is_running() const override;
    void set_handler(RequestHandler handler) override;
    void broadcast_event(const IpcEvent& event) override;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace voicetype