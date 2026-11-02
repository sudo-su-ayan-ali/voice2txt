#include "whisper_wrapper.h"
#include "log.h"
#include <memory>

namespace voicetype {

class NullWhisperContext : public WhisperContext {
public:
    bool load_model(const std::string&) override {
        VT_LOG_WARN("WhisperContext: using null implementation (whisper.cpp not integrated yet)");
        return false;
    }
    std::optional<TranscriptResult> transcribe_file(const std::string&) override { return std::nullopt; }
    std::optional<TranscriptResult> transcribe_buffer(const float*, size_t) override { return std::nullopt; }
};

std::unique_ptr<WhisperContext> WhisperContext::create() {
    return std::make_unique<NullWhisperContext>();
}

} // namespace voicetype