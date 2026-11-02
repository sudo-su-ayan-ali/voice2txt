#pragma once

#include "protocol.h"
#include <string>
#include <optional>
#include <chrono>

namespace voicetype {

class IpcClient {
public:
    virtual ~IpcClient() = default;
    virtual bool connect(const std::string& address) = 0;
    virtual void disconnect() = 0;
    virtual bool is_connected() const = 0;
    virtual std::optional<IpcResponse> send_request(const IpcRequest& req, 
                                                     std::chrono::milliseconds timeout = std::chrono::seconds(5)) = 0;

    static std::unique_ptr<IpcClient> create();
};

} // namespace voicetype