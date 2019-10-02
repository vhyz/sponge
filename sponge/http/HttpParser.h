#pragma once

#include <sponge/ChannelBuffer.h>
#include "HttpRequest.h"

namespace sponge {
namespace http {

class HttpParser {
   public:
    enum HttpParseState {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kExpectNone
    };

    HttpParser() : state_(kExpectRequestLine) {}

    bool parse(ChannelBuffer& buffer);

    bool parseCompletely() { return state_ == kExpectNone; }

   private:
    bool parseRequestLine(const char* begin, const char* end);

    bool parseHeaders(const char* begin, const char* end);

    bool parseBody(const char* begin, const char* end);

   private:
    HttpParseState state_;

    HttpRequest httpRequest_;

    size_t contentLength_;
};

}  // namespace http
}  // namespace sponge