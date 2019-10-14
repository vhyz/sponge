#pragma once

namespace sponge {

namespace http {

enum HttpMethod {
    HttpMethodGet,
    HttpMethodPost,
    HttpMethodDelete,
    HttpMethodPut,
    HttpMethodHead
};

enum HttpVersion { HttpVersion10, HttpVersion11 };

}  // namespace http

}  // namespace sponge
