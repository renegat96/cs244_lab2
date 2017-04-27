import matplotlib.pyplot as plt
results = \
[(136, 0.22, "fixed1"),
 (120, 0.44, "fixed2"),
 (146, 0.83, "fixed4"),
 (155, 1.57, "fixed8"),
 (226, 2.76, "fixed16"),
 (421, 4.03, "fixed32"),
 (608, 4.55, "fixed50"),
 (747, 4.65, "fixed64"),
]
delays      = map(lambda x: x[0], results)
throughputs = map(lambda x: x[1], results)
labels      = map(lambda x: x[2], results)

plt.plot(delays, throughputs, 'bo')
plt.ylabel('throughput(Mbits/s)')
plt.xlabel('delay(ms)')
plt.gca().invert_xaxis()
for i in range(len(labels)):
    plt.annotate(labels[i], xy = (delays[i] - 5, throughputs[i] + 0.01)) 
plt.show()



## SIMPLE AIMD
## Delay - 186 
## Throughput - 4.19

## SOPHISTICATED AIMD
## Delay - 105
## Throughput - 3.57
