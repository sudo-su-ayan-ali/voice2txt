#include "client.h"
#include "unix_socket_client.h"
#include "named_pipe_client.h"

namespace voicetype {

std::unique_ptr<IpcClient> IpcClient::create() {
#ifdef _WIN32
    return std::make_unique<NamedPipeClient>();
#else
    return std::make_unique<UnixSocketClient>();
#endif
}

} // namespace voicetype