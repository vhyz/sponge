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
* Reactor模式，线程分为主线程与IO线程，主线程仅监听一个新连接(accept)的事件，并按照Round Robin策略分发给其他IO线程，IO线程负责对一个连接的读写
* 由于TCP连接常在回调函数中使用，其生命周期模糊，使用std::shared_ptr管理TCP连接，保证了TCP连接生命周期不会过早结束

## Performance Test

* 项目使用wrk进行HTTP压力测试，分别测试了localhost与内网的1K和10K个并发长连接时的情况，使用的为UCloud上的机器。

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

