#include "HttpRequestParser.h"
#include "http_parser.h"

namespace sponge {

namespace http {

namespace {

void clearCapacity(std::string& str) {
    str.clear();
    str.shrink_to_fit();
}

}  // namespace

HttpRequestParser::HttpRequestParser()
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

HttpRequestParser::~HttpRequestParser() {}

int HttpRequestParser::parse(const char* str, size_t length) {
    size_t parseLength =
        http_parser_execute(parser_.get(), settings_.get(), str, length);

    if (parser_->http_errno) {
        return -1;
    }

    return parseLength;
}

void HttpRequestParser::clear() {
    http_parser_init(parser_.get(), HTTP_REQUEST);
    clearCapacity(currentField_);
    clearCapacity(currentValue_);
    clearCapacity(httpRequest_.url_);
    clearCapacity(httpRequest_.body_);
    httpRequest_.headers_.clear();
    isComplete_ = false;
}

void HttpRequestParser::addField() {
    if (!currentValue_.empty()) {
        httpRequest_.headers_[currentField_] = currentValue_;
        currentField_.clear();
        currentValue_.clear();
    }
}

int HttpRequestParser::onMessageBegin(http_parser* parser) { return 0; }

int HttpRequestParser::onMessageComplete(http_parser* parser) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->isComplete_ = true;

    return 0;
}

int HttpRequestParser::onHeadersComplete(http_parser* parser) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->addField();

    return 0;
}

int HttpRequestParser::onUrl(http_parser* parser, const char* at,
                             size_t length) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->httpRequest_.url_.append(at, length);

    return 0;
}

int HttpRequestParser::onHeaderField(http_parser* parser, const char* at,
                                     size_t length) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->addField();
    httpRequestParser->currentField_.append(at, length);

    return 0;
}

int HttpRequestParser::onHeaderValue(http_parser* parser, const char* at,
                                     size_t length) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->currentValue_.append(at, length);

    return 0;
}

int HttpRequestParser::onBody(http_parser* parser, const char* at,
                              size_t length) {
    auto httpRequestParser = reinterpret_cast<HttpRequestParser*>(parser->data);
    httpRequestParser->httpRequest_.body_.append(at, length);

    return 0;
}

int HttpRequestParser::onChunkComplete(http_parser* parser) { return 0; }

int HttpRequestParser::onChunkHeader(http_parser* parser) { return 0; }

int HttpRequestParser::onStatus(http_parser* parser, const char* at,
                                size_t length) {
    return 0;
}

}  // namespace http
}  // namespace sponge