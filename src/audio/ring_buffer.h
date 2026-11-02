#pragma once

#include <vector>
#include <cstddef>

namespace voicetype {

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : buffer_(capacity), capacity_(capacity) {}

    void clear() { head_ = 0; tail_ = 0; size_ = 0; }
    size_t capacity() const { return capacity_; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == capacity_; }
    size_t available() const { return capacity_ - size_; }

    // Write data, returns number of samples written
    size_t write(const float* data, size_t count) {
        size_t to_write = std::min(count, available());
        if (to_write == 0) return 0;

        size_t first_chunk = std::min(to_write, capacity_ - head_);
        std::copy(data, data + first_chunk, buffer_.begin() + head_);
        head_ = (head_ + first_chunk) % capacity_;

        if (first_chunk < to_write) {
            size_t second_chunk = to_write - first_chunk;
            std::copy(data + first_chunk, data + to_write, buffer_.begin());
            head_ = second_chunk;
        }
        size_ += to_write;
        return to_write;
    }

    // Read data, returns number of samples read
    size_t read(float* data, size_t count) {
        size_t to_read = std::min(count, size_);
        if (to_read == 0) return 0;

        size_t first_chunk = std::min(to_read, capacity_ - tail_);
        std::copy(buffer_.begin() + tail_, buffer_.begin() + tail_ + first_chunk, data);
        tail_ = (tail_ + first_chunk) % capacity_;

        if (first_chunk < to_read) {
            size_t second_chunk = to_read - first_chunk;
            std::copy(buffer_.begin(), buffer_.begin() + second_chunk, data + first_chunk);
            tail_ = second_chunk;
        }
        size_ -= to_read;
        return to_read;
    }

    // Peek at data without removing
    size_t peek(float* data, size_t count) const {
        size_t to_read = std::min(count, size_);
        if (to_read == 0) return 0;

        size_t first_chunk = std::min(to_read, capacity_ - tail_);
        std::copy(buffer_.begin() + tail_, buffer_.begin() + tail_ + first_chunk, data);
        if (first_chunk < to_read) {
            size_t second_chunk = to_read - first_chunk;
            std::copy(buffer_.begin(), buffer_.begin() + second_chunk, data + first_chunk);
        }
        return to_read;
    }

    // Get contiguous pointer to data (may be two chunks)
    std::pair<const float*, size_t> get_contiguous() const {
        if (size_ == 0) return {nullptr, 0};
        size_t first_chunk = std::min(size_, capacity_ - tail_);
        return {buffer_.data() + tail_, first_chunk};
    }

private:
    std::vector<float> buffer_;
    size_t capacity_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t size_ = 0;
};

} // namespace voicetype