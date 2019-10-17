#pragma once

namespace sponge {

namespace http {

enum HttpMethod {
    HttpMethodDelete = 0,
    HttpMethodGet,
    HttpMethodHead,
    HttpMethodPost,
    HttpMethodPut
};

enum HttpVersion { HttpVersion10, HttpVersion11, HttpVersionOther };

}  // namespace http

}  // namespace sponge
