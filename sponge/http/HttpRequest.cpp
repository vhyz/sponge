#include "HttpRequest.h"
#include"http_parser.h"

namespace sponge {
namespace http {

void HttpRequest::setHttpMethod(const char* str, size_t length) {
    std::string_view method(str, length);
    if (method == "GET") {
        httpMethod_ = HttpMethodGet;
    } else if (method == "POST") {
        httpMethod_ = HttpMethodPost;
    } else if (method == "DELETE") {
        httpMethod_ = HttpMethodDelete;
    } else if (method == "PUT") {
        httpMethod_ = HttpMethodPut;
    } else if (method == "HEAD") {
        httpMethod_ = HttpMethodHead;
    }
}

const char* HttpRequest::getHttpMethodString() const {
    switch (httpMethod_) {
        case HttpMethodGet:
            return "GET";

        case HttpMethodHead:
            return "HEAD";

        case HttpMethodPost:
            return "POST";

        case HttpMethodPut:
            return "Put";

        case HttpMethodDelete:
            return "DELETE";
    }

    return nullptr;
}

std::string HttpRequest::serialize() const {
    std::string res;

    // Request Line
    res += getHttpMethodString();
    res += " ";
    res += url_ + " ";
    res += httpVersion_ == HttpVersion10 ? "HTTP/1.0" : "HTTP/1.1";
    res += "\r\n";

    // Request Headers
    for(auto& p:headers_) {
        res += p.first + ": " + p.second + "\r\n";
    }
    res += "\r\n";

    // Request Body
    res += body_;

    return res;
}

}  // namespace http
}  // namespace sponge