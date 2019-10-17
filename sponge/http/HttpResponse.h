#pragma once

#include <string>
#include <unordered_map>
#include "HttpEnums.h"

namespace sponge {

namespace http {

class HttpResponse {
   public:
    HttpResponse(HttpVersion version) : httpVersion_(version) {}

    ~HttpResponse() {}

    using Headers = std::unordered_map<std::string, std::string>;

    void setStatusCode(int statusCode) { statusCode_ = statusCode; }

    void setBody(std::string body) { body_ = std::move(body); }

    void setHttpVersion(HttpVersion value) { httpVersion_ = value; }

    void setKeepAlive(bool on);

    bool isKeepAlive() const;

    std::string& getBody() { return body_; }

    const std::string& getBody() const { return body_; }

    Headers& getHeaders() { return headers_; }

    const Headers& getHeaders() const { return headers_; }

    void addHeader(std::string key, std::string value) {
        headers_[std::move(key)] = std::move(value);
    }

    std::string serialize() const;

   private:
    int statusCode_;

    HttpVersion httpVersion_;

    Headers headers_;

    std::string body_;

};  // namespace http

}  // namespace http
}  // namespace sponge