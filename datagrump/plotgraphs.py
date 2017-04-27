import matplotlib.pyplot as plt
data = \
[
 #[
 # (136, 0.22, "fixed1"),
 # (120, 0.44, "fixed2"),
 # (146, 0.83, "fixed4"),
 # (155, 1.57, "fixed8"),
 # (226, 2.76, "fixed16"),
 # (421, 4.03, "fixed32"),
 # (608, 4.55, "fixed50"),
 # (747, 4.65, "fixed64"),
 #],
 [
  (119, 3.04, "rtt-60-80"),
  (121, 3.52, "rtt-60-100-0.5"),
  (136, 3.96, "rtt-60-100"),
  (141, 4.18, "rtt-60-100-5"),
  (153, 4.22, "rtt-80-100"),
 ],
 [
  (96, 3.80, "tangra"),
  (104, 3.96, "tangra-1.4"),
  (99, 3.44, "tangra-1.3"),
 ],
 [
  (104, 2.90, "tcpfast-5"),
  (137, 3.56, "tcpfast-7.5"),
  (170, 4.01, "tcpfast-10"),
 ],
 [
  (105, 3.57, "aimd-soph"),
 ]
]
colors = ["bo", "ro", "go", "yo", "mo", "co"]
for results, color in zip(data, colors):
    delays      = map(lambda x: x[0], results)
    throughputs = map(lambda x: x[1], results)
    labels      = map(lambda x: x[2], results)

    plt.plot(delays, throughputs, color)
    for i in range(len(labels)):
        plt.annotate(labels[i], xy = (delays[i] + 2, throughputs[i] - 0.05))
plt.ylabel('throughput(Mbits/s)')
plt.xlabel('delay(ms)')
#plt.xscale("log")
plt.gca().invert_xaxis()
plt.show()
