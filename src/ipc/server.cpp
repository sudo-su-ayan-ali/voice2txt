#include "server.h"
#include "unix_socket_server.h"
#include "named_pipe_server.h"

namespace voicetype {

std::unique_ptr<IpcServer> IpcServer::create() {
#ifdef _WIN32
    return std::make_unique<NamedPipeServer>();
#else
    return std::make_unique<UnixSocketServer>();
#endif
}

} // namespace voicetype