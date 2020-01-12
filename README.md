# sponge: A C++ Multithread Network Library

[![Build Status](https://travis-ci.org/vhyz/sponge.svg?branch=master)](https://travis-ci.org/vhyz/sponge)

## Introduction

sponge是一个基于Reactor模式的多线程非阻塞网络库，是我在阅读陈硕老师的《Linux多线程服务端编程》后的一个项目。

## Envoirment

* OS: Ubuntu 18.04
* Complier: g++ 7.4
* Build: Cmake 3.10

## Feature

* Poller使用epoll LT实现IO复用和非阻塞IO
* 单进程多线程，其中多线程使用到了C++11的std::thread库
* Reactor模式，线程分为主线程与IO线程，主线程仅监听一个新连接(accept)的事件，并按照Round Robin策略分发给其他IO线程，IO线程负责对主线程分发的连接的读写
* 由于TCP连接常在回调函数中使用，其生命周期模糊，使用std::shared_ptr管理TCP连接，保证了TCP连接生命周期不会过早结束
* 用Linux提供的timerfd实现定时器功能，每个EventLoop都有一个TimerManager，底层数据结构采用std::set，添加与删除的复杂度均为O(logn)
* TcpClient支持自动重连服务器
* 内置简单的线程池，可将复杂的业务逻辑交给线程池异步处理

## Usage

写一个服务器，需要include <sponge/TcpServer.h>

一共有三个回调函数可以设置

* setConnCallBack() 设置连接成功建立和断开时的回调函数 

传入的回调函数签名为 void(const TcpConnection::Ptr&)
* setMessageCallBack() 设置收到消息时的回调函数 

传入的回调函数签名为 void(const TcpConnection::Ptr&, ChannelBuffer&)
* setWriteCompleteCallBack() 设置消息完全发送时的回调函数

传入的回调函数签名为 void(const TcpConnection::Ptr&)

比如写一个Echo服务器，我们需要设置收到消息时的回调函数

```C++
#include <sponge/Logger.h>
#include <sponge/TcpServer.h>

int main() {
    using namespace sponge;
    // 设置LOGGER日志等级
    setLogLevel(LOG_LEVEL_DEBUG);
    // 创建一个EventLoop
    EventLoop loop;
    // 创建服务器，后两个参数分别为端口与IO线程数量
    TcpServer tcpServer(&loop, 5000, 7);
    // 设置消息回调
    tcpServer.setMessageCallBack(
        [](const TcpConnection::Ptr& spConn, ChannelBuffer& msg) {
            INFO("handleRead %d bytes", msg.readableBytes());
            spConn->send(msg.readPtr(), msg.readableBytes());
            msg.readAllBytes();
        });
    tcpServer.start();
    loop.loop();
}
```

## Performance Test

* 为了测试方便，写了一个简单的HTTP服务器，然后使用wrk进行HTTP压力测试，分别测试了localhost与内网的1K和10K个并发长连接时的情况，使用的为UCloud上的云服务器。

* 机器配置：
    * CPU： 高频型16核
    * 内存： 16GB
    * 操作系统： Ubuntu 18.04

### localhost测试

* 主线程+7个IO线程 并发1K个长连接

![](https://github.com/vhyz/sponge/blob/master/img/1.png)

* 主线程+7个IO线程 并发10K个长连接

![](https://github.com/vhyz/sponge/blob/master/img/2.png)

* 主线程+15个IO线程 并发1K个连接

![](https://github.com/vhyz/sponge/blob/master/img/3.png)

* 主线程+15个IO线程 并发10K个连接

![](https://github.com/vhyz/sponge/blob/master/img/4.png)

## ToDoList

* 更好用的Buffer缓冲区
* 日志系统