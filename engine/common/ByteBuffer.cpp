#include <cstring>
#include "ByteBuffer.h"


// constructor
ByteBuffer::ByteBuffer() {
    _storage = createBuffer();
}


ByteBuffer::~ByteBuffer() {
    delete _storage;
}

RingByteBuf *ByteBuffer::createBuffer() {
    //return ObjPool::create<CBuffer>();
    return new RingByteBuf(512);
}


int ByteBuffer::getNextPackLen() {
    int readableBytes = _storage->readableBytes();
    if (readableBytes < 8) {
        return -1;
    }

    uint32 packLen = 0;
    _storage->peek(&packLen, 4);
    packLen = Endian::fromNetwork<uint32>(packLen);
    if (packLen > readableBytes - 4) {
        return -1;
    }
    return packLen + 4;
}
