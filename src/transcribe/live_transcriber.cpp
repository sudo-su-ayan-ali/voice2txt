#include "live_transcriber.h"
#include "log.h"

namespace voicetype {

bool LiveTranscriber::init(std::unique_ptr<WhisperContext> ctx, const std::string& language) {
    ctx_ = std::move(ctx);
    language_ = language;
    return ctx_ != nullptr;
}

void LiveTranscriber::start() {
    active_ = true;
    buffer_.clear();
}

void LiveTranscriber::push_audio(const float* pcm, size_t samples) {
    if (!active_) return;
    buffer_.insert(buffer_.end(), pcm, pcm + samples);
}

void LiveTranscriber::finish(ResultCallback callback) {
    if (!active_ || !ctx_) {
        if (callback) callback(TranscriptResult{});
        return;
    }
    active_ = false;
    
    // For now, just return empty result
    TranscriptResult result;
    result.text = "[transcription not implemented]";
    if (callback) callback(result);
}

void LiveTranscriber::cancel() {
    active_ = false;
    buffer_.clear();
}

bool LiveTranscriber::is_active() const {
    return active_;
}

} // namespace voicetype