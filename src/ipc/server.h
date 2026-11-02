#pragma once

#include "protocol.h"
#include <string>
#include <functional>
#include <memory>

namespace voicetype {

class IpcServer {
public:
    virtual ~IpcServer() = default;
    virtual bool start(const std::string& address) = 0;
    virtual void stop() = 0;
    virtual bool is_running() const = 0;
    virtual void set_handler(RequestHandler handler) = 0;
    virtual void broadcast_event(const IpcEvent& event) = 0;

    static std::unique_ptr<IpcServer> create();
};

} // namespace voicetype