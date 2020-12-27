# 网络流量发生器

该项目是网络编程课程的大作业中的一个题目。目的是通过客户端向服务器发送报文，服务器收到报文并统计每一个时间片中报文的数量，绘制成一个流量图，要求绘制的流量图为指定的波形。

想如果想了解更多该项目是如何实现的，请[看这里](docs/methodology.md)

## 环境准备

该项目的客户端和服务器端都是由C实现，绘图脚本由Python实现，数据库使用的是redis，因此需要以下编译以及运行环境

编译代码：

+ build-essential: 默认版本即可
+ hiredis: redis的C语言的API，需要去github上面下载并编译安装
+ pthread: 一般Linux自带的，所以Windows就不要用了，或者魔改也行，反正源码用的线程非常简单

运行脚本：

+ python: 请用python3，作者开发环境用的系统自带的python 3.8.5
+ matplotlib: python的绘图库，默认版本即可，pip安装（你用conda我也管不着）
+ redis: redis的python的API，默认版本即可，pip安装

数据库存储：

+ redis: 这才是redis真身，去官网下载解压编译安装

如果实在不会安装上述环境，请[看这里](docs/envinstall.md)

## 如何运行

运行该项目非常简单，分为以下几步：

1. 下载该项目，并切换目录到该项目的根目录下

    ```bash
    git clone https://github.com/CallMeChinese/NetflowGenerator.git
    cd NetflowGenerator
    ```

2. 使用<code>make</code>命令完成编译，二进制文件就在<code>bin/</code>下

    ```bash
    make
    ```

3. 按下面的顺序依次在不同的终端运行，即无特殊说明的情况下，每一个命令需要创建一个新的终端来运行

    + 启动redis

        ```bash
        redis-server config/redis.conf
        ```

    + 启动服务器

        ```bash
        ./bin/srv
        ```

    + 启动客户端

        ```bash
        ./bin/cli
        ```

    + 停止客户端（ctrl+c），并执行脚本，该步骤可以不创建新的终端

        ```bash
        python3 scripts/flowplot.py
        ```

4. 可以在项目的根目录下找到流量图片<code>data.png</code>（这么做是为了应对无界面服务器，有界面的PC可以直接在python中显示出来）
