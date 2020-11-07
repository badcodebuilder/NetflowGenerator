# Netflow Generator

This is one of task assigned in *Network programming* lesson. In this task, you need to create a client and a server. In server, a netflow counter should be run, and after collecting packets sent by client, it should plot the number of those. In client, a netflow generator should be run, and control how much packet it should send. And for more information about how to implement it, please consult this [document](docs/methodology.md).

## ENV requirment

+ pthread, which means you have to use **UNIX** or **LINUX**(maybe macOS but I cannot afford it)
+ redis

## How to RUN

1. Please change macro definition <code>SRV_ADDR</code> and <code>SRV_PORT</code> in file <code>config.h</code> to your real server if you need to use client. I am so sorry I do not know how to use autoconf to generate configuration script, maybe that will be added to this repo later.

2. Run the commands <code>make</code> in shell

3. Run <code>srv</code> on server and <code>cli</code> on client, and terminate them if you want.

4. Run the script to plot the chart

## Changes to redis

In <code>redis.conf</code>, line 112, timeout is set as 60, in order to terminal connection automatially after using ctrl-c to close server.
