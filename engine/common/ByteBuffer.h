#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "ByteConverter.h"
#include <string>
#include  "BaseType.h"
#include "RingByteBuf.h"
#include "spdlog/fmt/bundled/base.h"


class ByteBuffer {
public:
    static size_t const DEFAULT_SIZE = 0x1000;

    // constructor
    ByteBuffer();

    ByteBuffer(size_t reserve);

    ByteBuffer(ByteBuffer &&buf);

    ByteBuffer(ByteBuffer const &right);

    virtual ~ByteBuffer();

    ByteBuffer &operator=(ByteBuffer const &right);

    ByteBuffer &operator=(ByteBuffer &&right);

    RingByteBuf *createBuffer();

    void swapBuffer(ByteBuffer &right);

    RingByteBuf &storage() { return *_storage; }


    void writeInt8(uint8 value) const {
        _storage->write(&value, 1);
    }

    uint8 getInt8() const {
        uint8 value;
        uint8 ret = _storage->peek(&value, sizeof(value));
        return Endian::fromNetwork<uint8>(ret);
    }

    void writeInt16(uint16 value) const {
        value = Endian::toNetwork<uint16>(value);
        _storage->write(&value, sizeof(value));
    }

    uint16 getInt16() const {
        uint16 value;
        _storage->peek(&value, sizeof(value));
        return Endian::fromNetwork<uint16>(value);
    }

    void writeInt32(uint32 value) const {
        value = Endian::toNetwork<uint32>(value);
        _storage->write(&value, sizeof(value));
    }

    uint32 getInt32() const {
        uint32 value;
        uint32 ret = _storage->peek(&value, sizeof(value));
        return Endian::fromNetwork<uint32>(ret);
    }

    void writeInt64(uint64 value) const {
        value = Endian::toNetwork<uint64>(value);
        _storage->write(&value, sizeof(value));
    }

    uint64 getInt64() const {
        uint64 value;
        uint64 ret = _storage->peek(&value, sizeof(value));
        return Endian::fromNetwork<uint64>(ret);
    }

    size_t writeBytes(char *body, size_t size) const {
        return _storage->write(body, size);
    }

    size_t readBytes(char *body, size_t size) const {
        return _storage->read(body, size);
    }

    int getNextPackLen();

private:
    RingByteBuf *_storage;
};

#endif
