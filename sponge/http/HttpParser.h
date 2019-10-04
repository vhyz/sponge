#pragma once

#include <memory>
#include <string>
#include <unordered_map>

struct http_parser;
struct http_parser_settings;

namespace sponge {

namespace http {

class HttpParser {
   public:
    using Ptr = std::shared_ptr<HttpParser>;

    HttpParser();

    ~HttpParser();

    void clear();

    // parse http request
    // return parse length when success
    // return -1 when error
    int parse(const char* str, size_t length);

    const std::string& getUrl() const { return url_; }

    const std::string& getBody() const { return body_; }

    const std::unordered_map<std::string, std::string>& getHeaders() const {
        return headers_;
    }

    bool isComplete() const { return isComplete_; }

   private:
    // add field and value
    // it will clear field and value
    void addField();

    static int onMessageBegin(http_parser* parser);

    static int onMessageComplete(http_parser* parser);

    static int onHeadersComplete(http_parser* parser);

    static int onUrl(http_parser* parser, const char* at, size_t length);

    static int onHeaderField(http_parser* parser, const char* at,
                             size_t length);

    static int onHeaderValue(http_parser* parser, const char* at,
                             size_t length);

    static int onBody(http_parser* parser, const char* at, size_t length);

    static int onChunkComplete(http_parser* parser);

    static int onChunkHeader(http_parser* parser);

    static int onStatus(http_parser* parser, const char* at, size_t length);

   private:
    std::unique_ptr<http_parser> parser_;

    std::unique_ptr<http_parser_settings> settings_;

    std::string url_;

    std::string currentField_;

    std::string currentValue_;

    std::string body_;

    std::unordered_map<std::string, std::string> headers_;

    bool isComplete_;

    bool isKeepAlive_;
};

}  // namespace http
}  // namespace sponge