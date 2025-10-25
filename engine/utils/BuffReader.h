//
// Created by Wesly Zhong on 2025/10/8.
//

#ifndef ATHENA_BUFFREADER_H
#define ATHENA_BUFFREADER_H

#include "BaseType.h"
#include "hv/Buffer.h"

class BuffReader {
public:
    int32 readInt32(hv::Buffer *buff, int32 fromIndex) {
        return read < int32 > (buff, fromIndex);
    }

    uint32 readUint32(hv::Buffer *buff, int32 fromIndex) {
        return read < uint32 > (buff, fromIndex);
    }

    int32 readShort(hv::Buffer *buff, int32 fromIndex) {
        return read < int16 > (buff, fromIndex);
    }

    uint16 readUint16(hv::Buffer *buff, int32 fromIndex) {
        return read < uint16 > (buff, fromIndex);
    }


private:
    template<typename T>
    T read(hv::Buffer *buffer, int32 fromIndex) {
        if (!buffer) {
            throw std::invalid_argument("buffer is null");
        }

        size_t len = sizeof(T);
        if (fromIndex + len > buffer->size()) {
            throw std::out_of_range("read index out of range");
        }

        T value;
        std::memcpy(&value, (uint8_t *) buffer->data() + fromIndex, len);
        if (sizeof(T) == 2) {
            value = ntohs(value);
        } else if (sizeof(T) == 4) {
            value = ntohl(value);
        }
        return value;
    }

};


#endif //ATHENA_BUFFREADER_H
