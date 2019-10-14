#include "HttpResponse.h"
#include "http_parser.h"

namespace sponge {

namespace http {

std::string HttpResponse::serialize() const {
    std::string res;

    res += httpVersion_ == HttpVersion11 ? "HTTP/1.1" : "HTTP/1.0";
    res += " ";
    res += std::to_string(statusCode_) + " ";
    res += http_status_str((http_status)statusCode_);
}

}  // namespace http

}  // namespace sponge
