#pragma once

#include <string_view>
#include <unordered_map>
#include "HttpEnums.h"

namespace sponge {

namespace http {

class HttpRequest {
    friend class HttpRequestParser;

   public:
    using Headers = std::unordered_map<std::string, std::string>;

    HttpRequest() = default;

    ~HttpRequest() = default;

    HttpRequest(const HttpRequest&) = default;
    HttpRequest& operator=(const HttpRequest&) = default;
    HttpRequest(HttpRequest&&) noexcept = default;
    HttpRequest& operator=(HttpRequest&&) noexcept = default;

    void setHttpMethod(const char* str, size_t length);

    void setHttpMethod(HttpMethod httpMethod) { httpMethod_ = httpMethod; }

    void setHttpVersion(HttpVersion version) { httpVersion_ = version; }

    const char* getHttpMethodString() const;

    void setUrl(std::string url) { url_ = std::move(url); }

    void setBody(std::string body) { body_ = std::move(body); }

    void setHeaders(Headers headers) { headers_ = std::move(headers); }

    HttpMethod getHttpMethod() const { return httpMethod_; }

    std::string& getUrl() { return url_; }

    const std::string& getUrl() const { return url_; }

    HttpVersion getHttpVersion() const { return httpVersion_; }

    Headers& getHeaders() { return headers_; }

    const Headers& getHeaders() const { return headers_; }

    std::string& getBody() { return body_; }

    const std::string& getBody() const { return body_; }

    void addHeader(std::string key, std::string value) {
        headers_[std::move(key)] = std::move(value);
    }

    std::string serialize() const;

    bool isKeepAlive() const { return keepAlive_; }

    void setKeepAlive(bool v) { keepAlive_ = v; }

   private:
    HttpMethod httpMethod_;

    std::string url_;

    HttpVersion httpVersion_;

    Headers headers_;

    std::string body_;

    bool keepAlive_;
};

}  // namespace http
}  // namespace sponge