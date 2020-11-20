# Methodology

Multithread is used in branch master. But it does not return a correct result. Maybe there is a **BUG** about <code>clock()</code>, I will try it later, but I rewrited that in single thread.

In client, I use 2 threads, one is used to send packets, and the other is used to send a signal to enable sending.
