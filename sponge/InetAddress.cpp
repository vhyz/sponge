#include "InetAddress.h"
#include <arpa/inet.h>
#include <strings.h>

namespace sponge {

InetAddress::InetAddress(const std::string& ip, uint16_t port) {
    bzero(&addr_, sizeof(addr_));
    addr_.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
    addr_.sin_port = htons(port);
}

uint16_t InetAddress::getPort() const { return ntohs(addr_.sin_port); }

std::string InetAddress::getIp() const {
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof(buf));
    return std::string(buf);
}

std::string InetAddress::getIpAndPort() const {
    std::string ip = getIp();
    ip.push_back(':');
    return ip + std::to_string(getPort());
}

}  // namespace sponge