#include "HttpParser.h"
#include "http_parser.h"

namespace sponge {

namespace http {

void clearCapacity(std::string& str) {
    str.clear();
    str.shrink_to_fit();
}

HttpParser::HttpParser()
    : parser_(std::make_unique<http_parser>()),
      settings_(std::make_unique<http_parser_settings>()),
      isComplete_(false) {
    http_parser_init(parser_.get(), HTTP_REQUEST);
    http_parser_settings_init(settings_.get());

    settings_->on_body = onBody;
    settings_->on_chunk_complete = onChunkComplete;
    settings_->on_chunk_header = onChunkHeader;
    settings_->on_header_field = onHeaderField;
    settings_->on_header_value = onHeaderValue;
    settings_->on_headers_complete = onHeadersComplete;
    settings_->on_message_begin = onMessageBegin;
    settings_->on_message_complete = onMessageComplete;
    settings_->on_status = onStatus;
    settings_->on_url = onUrl;

    parser_->data = this;
}

HttpParser::~HttpParser() {}

int HttpParser::parse(const char* str, size_t length) {
    size_t parseLength =
        http_parser_execute(parser_.get(), settings_.get(), str, length);

    if (parser_->http_errno) {
        return -1;
    }

    return parseLength;
}

void HttpParser::clear() {
    http_parser_init(parser_.get(), HTTP_REQUEST);
    clearCapacity(url_);
    clearCapacity(currentField_);
    clearCapacity(currentValue_);
    clearCapacity(body_);
    headers_.clear();
    isComplete_ = false;
}

void HttpParser::addField() {
    if (!currentValue_.empty()) {
        headers_[currentField_] = currentValue_;
        currentField_.clear();
        currentValue_.clear();
    }
}

int HttpParser::onMessageBegin(http_parser* parser) { return 0; }

int HttpParser::onMessageComplete(http_parser* parser) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->isComplete_ = true;

    return 0;
}

int HttpParser::onHeadersComplete(http_parser* parser) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->addField();

    return 0;
}

int HttpParser::onUrl(http_parser* parser, const char* at, size_t length) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->url_.append(at, length);

    return 0;
}

int HttpParser::onHeaderField(http_parser* parser, const char* at,
                              size_t length) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->addField();
    httpParser->currentField_.append(at, length);

    return 0;
}

int HttpParser::onHeaderValue(http_parser* parser, const char* at,
                              size_t length) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->currentValue_.append(at, length);

    return 0;
}

int HttpParser::onBody(http_parser* parser, const char* at, size_t length) {
    auto httpParser = reinterpret_cast<HttpParser*>(parser->data);
    httpParser->body_.append(at, length);

    return 0;
}

int HttpParser::onChunkComplete(http_parser* parser) { return 0; }

int HttpParser::onChunkHeader(http_parser* parser) { return 0; }

int HttpParser::onStatus(http_parser* parser, const char* at, size_t length) {
    return 0;
}

}  // namespace http
}  // namespace sponge