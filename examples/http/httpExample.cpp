#include <sponge/http/HttpServer.h>
#include <stdio.h>
#include <iostream>

using namespace sponge;
using namespace sponge::http;

int main() {
    EventLoop loop;
    HttpServer httpServer(&loop, 80, 7);
    httpServer.setHttpCallBack(
        [](const HttpRequest& request, HttpResponse* response) {
            const std::string& url = request.getUrl();
            std::string body;
            if (url == "/") {
                body =
                    "<html>"
                    "<head><title>sponge</title></head>"
                    "<body><h1>Hello</h1><h2>url is /</h2></body>"
                    "</html";
            } else if (url == "/favicon.ico") {
                /*
                FILE* fp = fopen("favicon.ico", "r");
                size_t n;
                do {
                    char buf[10240];
                    n = fread(buf, 1, sizeof(buf), fp);
                    body.append(buf, n);
                } while (n > 0);
                */
            } else {
                body =
                    "<html>"
                    "<head><title>sponge</title></head>"
                    "<body><h1>Hello</h1><h2>url is other</h2></body>"
                    "</html";
            }
            response->setStatusCode(200);
            response->setBody(std::move(body));
            response->addHeader("Content-Length",
                                std::to_string(response->getBody().size()));
            response->addHeader("Server", "sponge 1.0");
            response->setKeepAlive(true);
            std::cout << request.serialize() << std::endl;
        });
    httpServer.start();
    loop.loop();
}