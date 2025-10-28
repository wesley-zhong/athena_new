//
// Created by zhongweiqi on 2025/10/28.
//

#ifndef ATHENA_BYTEUTILS_H
#define ATHENA_BYTEUTILS_H
#include "BaseType.h"


class ByteUtils {
public:
    static uint32 readInt32(void *body);

    static uint16 readInt16(void  *body);
};


#endif //ATHENA_BYTEUTILS_H
