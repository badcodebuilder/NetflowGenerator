# How to prepare environment

There are 3 parts: compiling, script-running, program-running

## Compiling

1. build-essential

    Easy to install, just run the command below:

    ```bash
    sudo apt install build-essential
    ```

2. pthread

    No need to install, pre-installed in almost all Linux or Unix system

3. hiredis

    A little difficult, but also easy to make it.

    Firstly, clone the repository from github:

    ```bash
    git clone https://github.com/redis/hiredis.git
    ```

    Then, go to the directory and use make tool:

    ```bash
    cd hiredis
    make
    sudo make install
    ```

    Finally, refresh dynamic linked library with:

    ```bash
    sudo ldconfig
    ```

## Script-Running

1. Python3

    Python3 is pre-installed in Ubuntu 20.04, you can consult any other installation guide

2. matplotlib

    Using pip tool:

    ```bash
    python3 -m pip install matplotlib
    ```

3. redis

    Using pip tool:

    ```bash
    python3 -m pip install redis
    ```

## Program-Running

1. redis

    Different from section above, this is the really redis server, and others are just redis-API in different language

    Firstly, get source code from website with:

    ```bash
    wget https://download.redis.io/releases/redis-6.0.9.tar.gz
    ```

    Then, unzip this file with:

    ```bash
    tar zxvf redis-6.0.9.tar.gz
    ```

    Finally, use make tool with:

    ```bash
    cd redis-6.0.9
    make
    sudo make install
    ```
