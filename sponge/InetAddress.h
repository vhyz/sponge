#pragma once

#include <netinet/in.h>
#include <string>

namespace sponge {

class InetAddress {
   public:
    InetAddress() = default;

    InetAddress(const std::string& ip, uint16_t port);

    void setAddress(const sockaddr_in& addr) { addr_ = addr; }

    uint16_t getPort() const;

    std::string getIp() const;

    std::string getIpAndPort() const;

    const sockaddr_in& getSockaddr() const { return addr_; }

   private:
    sockaddr_in addr_;
};

}  // namespace sponge
