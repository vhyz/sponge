#include "HttpRequest.h"

namespace sponge {

namespace http {

bool HttpRequest::setMethod(const char* str, size_t len) {
    std::string_view method(str, len);
    if (method == "GET") {
        method_ = kGet;
    } else if (method == "POST") {
        method_ = kPost;
    } else if (method == "PUT") {
        method_ = kPut;
    } else if (method == "HEAD") {
        method_ = kHead;
    } else if (method == "DELETE") {
        method_ = kDelete;
    } else {
        method_ = kMethodNone;
    }
    return method_ == kMethodNone;
}

const char* HttpRequest::getMethodString() const {
    const char* ret = nullptr;
    switch (method_) {
        case kMethodNone:
            ret = "NONE";
            break;
        case kGet:
            ret = "GET";
            break;
        case kPost:
            ret = "POST";
            break;
        case kPut:
            ret = "PUT";
            break;
        case kHead:
            ret = "HEAD";
            break;
        case kDelete:
            ret = "DELETE";
            break;
    }
    return ret;
}

const char* HttpRequest::getVersionString() const {
    const char* ret = nullptr;
    switch (version_) {
        case kVersionNone:
            ret = "NONE";
            break;
        case kHttp10:
            ret = "HTTP/1.0";
            break;
        case kHttp11:
            ret = "HTTP/1.1";
            break;
    }
    return ret;
}

}  // namespace http
}  // namespace sponge