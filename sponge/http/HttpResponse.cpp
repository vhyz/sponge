#include "HttpResponse.h"
#include "http_parser.h"

namespace sponge {

namespace http {

const char* HEADER_FIELD_CONNECTION = "Connection";
const char* HEADER_VALUE_KEEP_ALIVE = "keep-alive";
const char* HEADER_VALUE_CLOSE = "close";

std::string HttpResponse::serialize() const {
    std::string res;

    // Response Line
    res += httpVersion_ == HttpVersion11 ? "HTTP/1.1" : "HTTP/1.0";
    res += " ";
    res += std::to_string(statusCode_) + " ";
    res += http_status_str(static_cast<http_status>(statusCode_));
    res += "\r\n";

    // Response Headers
    for (auto& p : headers_) {
        res += p.first + ": " + p.second + "\r\n";
    }
    res += "\r\n";

    // Response Body
    res += body_;

    return res;
}

void HttpResponse::setKeepAlive(bool on) {
    if (on) {
        headers_[HEADER_FIELD_CONNECTION] = HEADER_VALUE_KEEP_ALIVE;
    } else {
        headers_[HEADER_FIELD_CONNECTION] = HEADER_VALUE_CLOSE;
    }
}

bool HttpResponse::isKeepAlive() const {
    auto it = headers_.find(HEADER_FIELD_CONNECTION);
    if (it != headers_.end()) {
        if (it->second == HEADER_VALUE_KEEP_ALIVE) {
            return true;
        } else {
            return false;
        }
    } else {
        if (httpVersion_ == HttpVersion11) {
            return true;
        } else {
            return false;
        }
    }
}

}  // namespace http

}  // namespace sponge
