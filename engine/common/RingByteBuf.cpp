//
// Created by zhongweiqi on 2025/10/27.
//

#include "RingByteBuf.h"

RingByteBuf::RingByteBuf(size_t capacity)
    : buf_(nullptr), cap_(capacity), head_(0), tail_(0) {
    if (capacity == 0) throw std::invalid_argument("capacity must be > 0");
    buf_ = static_cast<uint8_t *>(je_malloc(capacity));
    if (!buf_) throw std::bad_alloc();
}


RingByteBuf::~RingByteBuf() {
    if (buf_) je_free(buf_);
}


RingByteBuf::RingByteBuf(RingByteBuf &&other) noexcept
    : buf_(other.buf_), cap_(other.cap_), head_(other.head_.load()), tail_(other.tail_.load()) {
    other.buf_ = nullptr;
    other.head_.store(0);
    other.tail_.store(0);
}


RingByteBuf &RingByteBuf::operator=(RingByteBuf &&other) noexcept {
    if (this == &other) return *this;
    if (buf_) je_free(buf_);
    buf_ = other.buf_;
    // cap_ is const, cannot be assigned — this object must be constructed with same capacity in move-assignment usage.
    other.buf_ = nullptr;
    head_.store(other.head_.load());
    tail_.store(other.tail_.load());
    other.head_.store(0);
    other.tail_.store(0);
    return *this;
}


size_t RingByteBuf::capacity() const noexcept { return cap_; }


size_t RingByteBuf::readableBytes() const noexcept {
    size_t h = head_.load(std::memory_order_acquire);
    size_t t = tail_.load(std::memory_order_acquire);
    if (t >= h) return t - h;
    return cap_ - h + t;
}


size_t RingByteBuf::writableBytes() const noexcept {
    return cap_ - readableBytes();
}


bool RingByteBuf::tryWrite(const void *src, size_t len) {
    if (len == 0) return true;
    std::lock_guard<std::mutex> lk(write_mtx_);
    size_t writable = writableBytes();
    if (len > writable) return false;


    size_t h = head_.load(std::memory_order_acquire);
    size_t t = tail_.load(std::memory_order_relaxed);


    if (t + len <= cap_) {
        // single contiguous write
        std::memcpy(buf_ + t, src, len);
        tail_.store((t + len) % cap_, std::memory_order_release);
    } else {
        // wraps around
        size_t first = cap_ - t;
        std::memcpy(buf_ + t, src, first);
        std::memcpy(buf_, static_cast<const uint8_t *>(src) + first, len - first);
        tail_.store((t + len) % cap_, std::memory_order_release);
    }
    return true;
}

size_t RingByteBuf::read(void *dst, size_t len) {
    if (len == 0) return 0;
    std::lock_guard<std::mutex> lk(read_mtx_);
    size_t avail = readableBytes();
    if (avail == 0) return 0;
    size_t toread = (len <= avail) ? len : avail;


    size_t h = head_.load(std::memory_order_relaxed);
    if (h + toread <= cap_) {
        std::memcpy(dst, buf_ + h, toread);
        head_.store((h + toread) % cap_, std::memory_order_release);
    } else {
        size_t first = cap_ - h;
        std::memcpy(dst, buf_ + h, first);
        std::memcpy(static_cast<uint8_t *>(dst) + first, buf_, toread - first);
        head_.store((h + toread) % cap_, std::memory_order_release);
    }
    return toread;
}

size_t RingByteBuf::peek(void *dst, size_t len) const {
    if (len == 0) return 0;
    std::lock_guard<std::mutex> lk(read_mtx_);
    size_t avail = readableBytes();
    if (avail == 0) return 0;
    size_t toread = (len <= avail) ? len : avail;
    size_t h = head_.load(std::memory_order_relaxed);
    if (h + toread <= cap_) {
        std::memcpy(dst, buf_ + h, toread);
    } else {
        size_t first = cap_ - h;
        std::memcpy(dst, buf_ + h, first);
        std::memcpy(static_cast<uint8_t *>(dst) + first, buf_, toread - first);
    }
    return toread;
}


const uint8_t *RingByteBuf::linearReadablePtr(size_t *outLen) const noexcept {
    std::lock_guard<std::mutex> lk(read_mtx_);
    size_t h = head_.load(std::memory_order_relaxed);
    size_t t = tail_.load(std::memory_order_acquire);
    if (h == t) {
        *outLen = 0;
        return nullptr;
    }
    if (t > h) {
        *outLen = t - h;
        return buf_ + h;
    }
    // wrapped: contiguous chunk from h to cap_
    *outLen = cap_ - h;
    return buf_ + h;
}


void RingByteBuf::consume(size_t n) {
    if (n == 0) return;
    std::lock_guard<std::mutex> lk(read_mtx_);
    size_t avail = readableBytes();
    if (n > avail) throw std::out_of_range("consume > readableBytes");
    size_t h = head_.load(std::memory_order_relaxed);
    head_.store((h + n) % cap_, std::memory_order_release);
}

// 新增: 手动移动写索引（跳过未写区域）
void RingByteBuf::increase(size_t n) {
    if (n == 0) return;
    std::lock_guard<std::mutex> lk(read_mtx_);

    size_t t = tail_.load(std::memory_order_relaxed);
    tail_.store((t + n) % cap_, std::memory_order_release);
}


void RingByteBuf::clear() noexcept {
    std::lock_guard<std::mutex> rlk(read_mtx_);
    std::lock_guard<std::mutex> wlk(write_mtx_);
    head_.store(0);
    tail_.store(0);
}


void *RingByteBuf::je_malloc(size_t capacity) {
    return std::malloc(static_cast<size_t>(capacity));

}

void RingByteBuf::je_free(uint8_t *buf) {
    std::free(buf);\
}