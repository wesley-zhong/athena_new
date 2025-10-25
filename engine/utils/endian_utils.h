
#ifndef ATHENA_ENDIAN_UTILS_H
#define ATHENA_ENDIAN_UTILS_H
#include <cstdint>

// --- 1. 平台检测和标准库引入 ---
#if defined(_WIN32)
// Windows 平台
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib") // 确保链接 ws2_32.lib
    // Windows 的 Winsock2.h 已经提供了这四个函数

#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
// 类 Unix 平台 (Linux, macOS, BSD)
#include <arpa/inet.h>
// 类 Unix 的 arpa/inet.h 已经提供了这四个函数
#endif

// --- 3. C++ 封装 (可选) ---

// 这是一个额外的封装，用于提供一个统一的命名空间或类接口，
// 但在大多数情况下，直接使用上面引入或定义的函数即可。

namespace NetUtil {
    // 使用标准库提供的函数
    // htons: Host to Network Short (uint16_t)
    inline uint16_t htons(uint16_t hostshort) {
    // 直接调用标准库中的 htons
    return ::htons(hostshort);
}

// htonl: Host to Network Long (uint32_t)
inline uint32_t htonl(uint32_t hostlong) {
// 直接调用标准库中的 htonl
return ::htonl(hostlong);
}

// ntohs: Network to Host Short (uint16_t)
inline uint16_t ntohs(uint16_t netshort) {
// 直接调用标准库中的 ntohs
return ::ntohs(netshort);
}

// ntohl: Network to Host Long (uint32_t)
inline uint32_t ntohl(uint32_t netlong) {
// 直接调用标准库中的 ntohl
return ::ntohl(netlong);
}

} // namespace NetUtil

#endif //ATHENA_ENDIAN_UTILS_H
