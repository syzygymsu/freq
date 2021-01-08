#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import math
import progressbar
import statistics
import subprocess
import sys
import timeit

rounds = 200
times = []

widgets = [
    ' [', progressbar.Timer(format= 'elapsed time: %(elapsed)s'), '] ',
    progressbar.Bar('*'),
    ' (', progressbar.ETA(), ') ',
]
bar = progressbar.ProgressBar(max_value=rounds, widgets=widgets).start()

for i in xrange(rounds):
    start_ts = timeit.default_timer()
    process = subprocess.Popen(["bazel-bin/src/freq", "test/samples/war_and_peace.txt"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    end_ts = timeit.default_timer()
    return_code = process.returncode

    failed = False
    if stderr:
        failed = True
        print("Error:", file=sys.stderr)
        print(stderr, file=sys.stderr)
    if 0 != return_code:
        failed = True
        print("Return code is not zero: ", return_code, file=sys.stderr)

    if failed:
        print("Performance test failed", file=sys.stderr)
        exit(-1)

    times.append(end_ts - start_ts)

    bar.update(i)


print(
    "Perf time(ms):", round(statistics.mean(times) * 1000, 1),
    "±" + str(round(statistics.stdev(times) * 1000, 1)) + ",",
    "mean:", round(statistics.mean(times) * 1000, 1))
