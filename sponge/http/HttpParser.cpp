#include "HttpParser.h"
#include <algorithm>

namespace sponge {

namespace http {

const char* CRLF = "\r\n";

const char* findCRLF(const ChannelBuffer& buffer) {
    const char* crlf =
        std::search(buffer.readPtr(), buffer.writePtr(), CRLF, CRLF + 2);
    return crlf == buffer.writePtr() ? nullptr : crlf;
}

bool HttpParser::parse(ChannelBuffer& buffer) {
    bool ok = true;
    bool quit = false;
    while (!quit) {
        switch (state_) {
            case kExpectNone:
                break;

            case kExpectRequestLine:
                const char* crlf = findCRLF(buffer);
                if (crlf) {
                } else {
                    quit = true;
                }
                break;

            case kExpectHeaders:
                const char* crlf = findCRLF(buffer);
                if (crlf) {
                } else {
                    quit = true;
                }
                break;

            case kExpectBody:
                break;
        }
    }
}

bool HttpParser::parseRequestLine(const char* begin, const char* end) {
    bool ok;
    const char* token;

    // parse method
    token = std::find(begin, end, ' ');
    if (token == end) {
        return false;
    }

    ok = httpRequest_.setMethod(begin, token - begin);
    if (!ok) {
        return false;
    }

    begin = token + 1;
    if (begin == end) {
        return false;
    }

    
}

bool HttpParser::parseHeaders(const char* begin, const char* end) {}

bool HttpParser::parseBody(const char* begin, const char* end) {}

}  // namespace http
}  // namespace sponge