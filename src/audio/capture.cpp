#include "capture.h"
#include "log.h"
#include <memory>

namespace voicetype {

class NullAudioCapture : public AudioCapture {
public:
    bool start(Callback) override { return false; }
    void stop() override {}
    bool is_recording() const override { return false; }
    uint32_t sample_rate() const override { return 16000; }
    uint32_t channels() const override { return 1; }
};

std::unique_ptr<AudioCapture> AudioCapture::create(uint32_t, uint32_t, const std::string&) {
    VT_LOG_WARN("AudioCapture: using null implementation (miniaudio not integrated yet)");
    return std::make_unique<NullAudioCapture>();
}

std::vector<AudioDevice> enumerate_audio_devices() {
    return {};
}

} // namespace voicetype