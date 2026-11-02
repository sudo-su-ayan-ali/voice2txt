#pragma once

#include "whisper_wrapper.h"
#include <functional>
#include <string>

namespace voicetype {

class LiveTranscriber {
public:
    using ResultCallback = std::function<void(const TranscriptResult&)>;

    LiveTranscriber() = default;
    ~LiveTranscriber() = default;

    bool init(std::unique_ptr<WhisperContext> ctx, const std::string& language = "en");
    void start();
    void push_audio(const float* pcm, size_t samples);
    void finish(ResultCallback callback);
    void cancel();
    bool is_active() const;

private:
    std::unique_ptr<WhisperContext> ctx_;
    std::string language_;
    bool active_ = false;
    std::vector<float> buffer_;
    ResultCallback callback_;
};

} // namespace voicetype