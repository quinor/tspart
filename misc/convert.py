#!/usr/bin/env python3

l = open("out.plt", "r").read().split("\n")
l = [e.split() for e in l]
l = [(int(e[0]), int(e[1])) for e in l if e]

s = """<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg height="{}" width="{}" xmlns="http://www.w3.org/2000/svg" version="1.1"><polyline
points="{}" style="fill:none;stroke:#4b2877;stroke-width:4" />
</svg>"""

xs = [e[0] for e in l]
ys = [e[1] for e in l]

mx = min(xs)
my = min(ys)

xs = [e-mx for e in xs]
ys = [e-my for e in ys]

w = max(xs)
h = max(ys)

out = s.format(h, w, " ".join("{},{}".format(*e) for e in zip(xs, ys)))

open("out.svg", "w").write(out)
