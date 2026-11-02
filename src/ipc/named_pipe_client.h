#pragma once

#include "client.h"

namespace voicetype {

class NamedPipeClient : public IpcClient {
public:
    NamedPipeClient();
    ~NamedPipeClient() override;

    bool connect(const std::string& address) override;
    void disconnect() override;
    bool is_connected() const override;
    std::optional<IpcResponse> send_request(const IpcRequest& req, 
                                             std::chrono::milliseconds timeout) override;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace voicetype