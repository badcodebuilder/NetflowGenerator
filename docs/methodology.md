# Methodology

When you ask how to implement this program, I will give you 2 keywords, socket and multithread(maybe I need to add Redis). In the client, I use about 50 threads, and each of them sends and only sends UDP packet if necessary. I think if all of packets(missing a few of them will be OK) are received and record the receiving time, group them by slices of time, it should make it.
