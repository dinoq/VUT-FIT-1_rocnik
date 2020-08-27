#!/usr/bin/env python3

import fileinput
import time

start_time = time.time()
words = set([])

for line in fileinput.input('words.txt'):
	words.add(line.rstrip())

palindroms =set([w for w in words if w == w[::-1]]) 

result = [w for w in words if w not in palindroms and w[::-1] in words]

print(sorted(result))

print(time.time()-start_time)



