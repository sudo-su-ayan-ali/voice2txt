#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace voicetype {

class AudioCapture {
public:
    using Callback = std::function<void(const float* data, size_t frames)>;

    AudioCapture() = default;
    virtual ~AudioCapture() = default;

    // Non-copyable, movable
    AudioCapture(const AudioCapture&) = delete;
    AudioCapture& operator=(const AudioCapture&) = delete;
    AudioCapture(AudioCapture&&) = default;
    AudioCapture& operator=(AudioCapture&&) = default;

    virtual bool start(Callback cb) = 0;
    virtual void stop() = 0;
    virtual bool is_recording() const = 0;
    virtual uint32_t sample_rate() const = 0;
    virtual uint32_t channels() const = 0;

    static std::unique_ptr<AudioCapture> create(uint32_t sample_rate = 16000, 
                                                 uint32_t channels = 1,
                                                 const std::string& device_id = "");
};

struct AudioDevice {
    std::string id;
    std::string name;
    bool is_default = false;
};

std::vector<AudioDevice> enumerate_audio_devices();

} // namespace voicetype