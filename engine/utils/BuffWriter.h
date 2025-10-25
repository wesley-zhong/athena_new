//
// Created by Wesly Zhong on 2025/10/8.
//

#ifndef ATHENA_BUFFWRITER_H
#define ATHENA_BUFFWRITER_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "hv/Buffer.h"

class BuffWriter {
public:
    void writeInt32(hv::Buffer *buffer_, int32_t value) {
        write < uint32_t > (buffer_, value);
    }

    void writeUint32(hv::Buffer *buffer_, uint32_t value) {
        write < uint32_t > (buffer_, value);
    }

    void writeShort(hv::Buffer *buffer_, int16_t value) {
        write < uint16_t > (buffer_, value);
    }

    void writeUint16(hv::Buffer *buffer_, uint16_t value) {
        write < uint16_t > (buffer_, value);
    }

    void writeBytes(hv::Buffer *buffer_, const void *data, size_t len) {
        if (!data || len == 0) return;
     //   buffer_->append(data, len);

    }

    size_t size(hv::Buffer *buffer_) const { return buffer_->size(); }

private:

    template<typename T>
    void write(hv::Buffer *buffer_, T value) {
        if (sizeof(T) == 2) {
            value = htons(value);
        } else if (sizeof(T) == 4) {
            value = htonl(value);
        }
       // buffer_->append(&value, sizeof(T));
    }
};

#endif //ATHENA_BUFFWRITER_H
