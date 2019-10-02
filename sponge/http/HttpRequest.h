#pragma once

#include <map>
#include <string>
#include <string_view>

namespace sponge {

namespace http {

class HttpRequest {
   public:
    enum Method { kMethodNone, kGet, kPost, kPut, kHead, kDelete };

    enum Version { kVersionNone, kHttp10, kHttp11 };

    HttpRequest() : method_(kMethodNone), version_(kVersionNone) {}

    void setMethod(Method method) { method_ = method; }

    bool setMethod(const char* str, size_t len);

    Method getMethod() const { return method_; }

    const char* getMethodString() const;

    void setVersion(Version version) { version_ = version; }

    Version getVersion() const { return version_; }

    const char* getVersionString() const;

    void setPath(std::string path) { path_ = std::move(path); }

    const std::string& getPath() const { return path_; }

    std::map<std::string, std::string>& headers() { return headers_; }

    const std::map<std::string, std::string>& headers() const {
        return headers_;
    }

    std::map<std::string, std::string>& params() { return params_; }

    const std::map<std::string, std::string>& params() const { return params_; }

    std::string& body() { return body_; }

    const std::string& body() const { return body_; }

   private:
    Version version_;

    Method method_;

    std::string path_;

    std::map<std::string, std::string> params_;

    std::map<std::string, std::string> headers_;

    std::string body_;
};

}  // namespace http
}  // namespace sponge