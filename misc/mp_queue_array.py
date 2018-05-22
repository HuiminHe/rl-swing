# multi process

import multiprocessing
from time import sleep
import random
import os
N = 10

def worker(a, que):
    i = que.get()
    t = random.random() * 3
    sleep(t)
    print(i)
    a[i] = i**2

if __name__ == '__main__':
    pool = multiprocessing.Pool(processes=4)
    m = multiprocessing.Manager()
    q = m.Queue()
    for i in range(N):
        q.put(i)
    a = m.Array('i', [0] * N)
    print(a[:])
    workers = [pool.apply_async(worker, (a, q)) for i in range(N)]
    for w in workers:
        w.get(timeout=5)

    print(a[:])