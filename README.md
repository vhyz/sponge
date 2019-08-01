# SpongeBob: A C++ Multithread Network Library

## Introduction

SpongeBob是一个基于Reactor模式的多线程服务器框架，是我在阅读陈硕老师的《Linux多线程服务端编程》后的一个练手项目。

## Envoirment

* OS: Ubuntu 18.04
* Complier: g++ 7.4
* Build: Cmake 3.10

## Feature

* Poller使用Epoll实现IO复用和非阻塞IO
* 单进程多线程，其中多线程使用到了C++11的std::thread库
* Reactor模式，线程分为主线程与IO线程，主线程仅监听一个新连接(accept)的事件，并按照Round Robin策略分发给其他IO线程，IO线程负责对主线程分发的连接的读写
* 由于TCP连接常在回调函数中使用，其生命周期模糊，使用std::shared_ptr管理TCP连接，保证了TCP连接生命周期不会过早结束

## Usage

写一个服务器，需要include"TcpServer.h"

一共有三个回调函数可以设置

* setNewConnCallBack() 设置连接成功建立时的回调函数 

传入的回调函数签名为 void(const spTcpConnection&)
* setMsgCallBack() 设置收到消息时的回调函数 

传入的回调函数签名为 void(const spTcpConnection&, std::string&)
* setWriteCompleteCallBack() 设置消息完全发送时的回调函数

传入的回调函数签名为 void(const spTcpConnection&)

比如写一个Echo服务器，我们需要设置收到消息时的回调函数

```C++
#include "TcpServer.h"

void echo(const spTcpConnection& spConn, std::string& msg) {
    std::string s;
    s.swap(msg);
    spConn->send(s);
}

int main() {
    // 首先创建一个EventLoop
    EventLoop loop;

    // 随后创建一个服务器，后两个参数分别为端口和IO线程数量
    TcpServer tcpServer(&loop, 5000, 7);

    tcpServer.setMsgCallBack(
        std::bind(&echo, std::placeholders::_1, std::placeholders::_2));

    tcpServer.start();
    loop.loop();
}
```

## Performance Test

* 项目使用wrk进行HTTP压力测试，分别测试了localhost与内网的1K和10K个并发长连接时的情况，使用的为UCloud上的云服务器。

* 机器配置：
    * CPU： 高频型16核
    * 内存： 16GB
    * 操作系统： Ubuntu 18.04

### localhost测试

* 主线程+7个IO线程 并发1K个长连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/1.png)

* 主线程+7个IO线程 并发10K个长连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/2.png)

* 主线程+15个IO线程 并发1K个连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/3.png)

* 主线程+15个IO线程 并发10K个连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/4.png)

### 内网测试

内网测试指开两台服务器，一台服务器用来当客户端，另外一台服务器用来当服务器

* 主线程+7个IO线程 并发1K个连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/6.png)

* 主线程+7个IO线程 并发10K个连接

![](https://github.com/vhyz/SpongeBob/blob/master/img/5.png)

由内网测试结果可推测，传输速度受到了带宽的限制，导致了并发量无法继续上涨

## ToDoList

* 定时器
* 业务处理线程池
* 更好用的Buffer缓冲区
* 日志系统