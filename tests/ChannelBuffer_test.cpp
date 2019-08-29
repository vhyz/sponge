#include <SpongeBob/ChannelBuffer.h>
#include <stdio.h>
#include <iostream>

void printBufferInfo(ChannelBuffer& buffer) {
    std::cout << "capacity(): " << buffer.capacity() << std::endl;
    std::cout << "readable(): " << buffer.readable() << std::endl;
    std::cout << "readableBytes(): " << buffer.readableBytes() << std::endl;
    std::cout << "discardableBytes(): " << buffer.discardableBytes()
              << std::endl;
    std::cout << "writeable(): " << buffer.writeable() << std::endl;
    std::cout << "writeableBytes(): " << buffer.writeableBytes() << std::endl;
    std::cout << std::endl;
}

void printBuffer(ChannelBuffer& buffer) {
    for (const char* ptr = buffer.readPtr(); ptr != buffer.writePtr(); ++ptr) {
        std::cout << *ptr;
    }
    std::cout << std::endl;
}

int main() {
    std::string a(5000, '1');

    ChannelBuffer buffer;
    printBufferInfo(buffer);

    buffer.append(a);
    printBufferInfo(buffer);

    buffer.readNBytes(200);
    buffer.shrink();
    printBufferInfo(buffer);

    buffer.readAllBytes();
    printBufferInfo(buffer);

    buffer.shrink();
    printBufferInfo(buffer);
    printBuffer(buffer);

    buffer.appendInt32(10);
    printBufferInfo(buffer);
    std::cout << buffer.peekInt32() << std::endl;
    printBufferInfo(buffer);
    std::cout << buffer.readInt32() << std::endl;
    printBufferInfo(buffer);
}