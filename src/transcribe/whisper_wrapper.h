#pragma once

#include <string>
#include <vector>
#include <optional>

namespace voicetype {

struct TranscriptSegment {
    int64_t start_ms = 0;
    int64_t end_ms = 0;
    std::string text;
    float avg_logprob = 0.0f;
    float no_speech_prob = 0.0f;
};

struct TranscriptResult {
    std::string text;
    std::string language;
    std::vector<TranscriptSegment> segments;
    int64_t duration_ms = 0;
    int64_t inference_ms = 0;
};

class WhisperContext {
public:
    WhisperContext() = default;
    virtual ~WhisperContext() = default;
    WhisperContext(const WhisperContext&) = delete;
    WhisperContext& operator=(const WhisperContext&) = delete;
    WhisperContext(WhisperContext&&) = default;
    WhisperContext& operator=(WhisperContext&&) = default;

    virtual bool load_model(const std::string& model_path) = 0;
    virtual std::optional<TranscriptResult> transcribe_file(const std::string& wav_path) = 0;
    virtual std::optional<TranscriptResult> transcribe_buffer(const float* pcm, size_t samples) = 0;
    
    static std::unique_ptr<WhisperContext> create();
};

} // namespace voicetype