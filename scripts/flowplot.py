# -*- coding:utf-8 -*-

import matplotlib.pyplot as plt
import redis

def main()->int:
    """
        Main Function
        ===
    """
    # Get all data in form of List[string]
    conn = redis.Redis(host="localhost", port=6379, decode_responses=True)
    lenth = conn.llen("packet")
    rdata = conn.lrange("packet", 0, lenth)
    conn.close()
    print("Read %d items from redis"%(len(rdata)))

    # Convert data from string to int
    data = []
    for i in rdata:
        data.append(int(i))

    # Count data
    interval = 100000
    maxTime = max(data)
    print("Max time stamp is: %d"%(maxTime))
    slices = maxTime//interval
    count = [0]*(slices + 1)
    for i in data:
        count[i//interval] += 1

    # Plot data
    plt.xlabel("time")
    plt.ylabel("count")
    plt.plot(count)
    plt.savefig("data.png")
    #plt.show()

if __name__ == "__main__":
    main()
    pass