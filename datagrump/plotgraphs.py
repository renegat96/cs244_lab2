import matplotlib.pyplot as plt

def fixed():
    data = [
     [
      (136, 0.22, "fixed1"),
      (120, 0.44, "fixed2"),
      (146, 0.83, "fixed4"),
      (155, 1.57, "fixed8"),
      (226, 2.76, "fixed16"),
      (421, 4.03, "fixed32"),
      (608, 4.55, "fixed50"),
      (747, 4.65, "fixed64"),
     ],
    ]
    labels = [None]
    return data, labels, 9, -0.15

def all_mechanisms():
    data = \
    [
     [
      (119, 3.04, "rtt-60-80"),
      (121, 3.52, "rtt-60-100-0.5"),
      (136, 3.96, "rtt-60-100"),
      (141, 4.18, "rtt-60-100-5"),
      (153, 4.22, "rtt-80-100"),
     ],
     [
      (94 , 3.83, "tangra"),
     ],
     [
      (185, 4.25, "aimd-simp-0.5"),
      (197, 4.39, "aimd-simp-0.75"),
      (178, 3.83, "aimd-simp-0.25"),
     ],
     [
      (104, 2.90, "tcpfast-5"),
      (137, 3.56, "tcpfast-7.5"),
      (170, 4.01, "tcpfast-10"),
     ],
     [
      (101, 3.49, "aimd-soph-0.5"),
      (99 , 2.97, "aimd-soph-0.75"),
      (111, 3.80, "aimd-soph-0.85"),
     ]
    ]
    labels = [None] * len(data)
    return data, labels, 2, -0.05

def low_pass_at_1_4():
    data = [
     [
      (98 , 3.89, ''),
      (96 , 3.86, ''),
      (99 , 3.86, ''),
     ],
     [
      (96 , 3.85, ''),
      (98 , 3.80, ''),
      (98 , 3.85, ''),
     ],
     [
      (97 , 3.70, ''),
      (96 , 3.77, ''),
      (96 , 3.79, ''),
      (97 , 3.82, ''),
     ],
     [
      (94 , 3.61, ''),
      (94 , 3.66, ''),
      (96 , 3.74, ''),
      (97 , 3.79, ''),
     ],
     [
      (101, 3.29, ''),
      (96 , 3.55, ''),
      (96 , 3.58, ''),
     ],
    ]
    labels = ['Low-pass at 0.5', 'Low-pass at 0.6', 'Low-pass at 0.7', 'Low-pass at 0.8', 'Low-pass at 0.9']
    return data, labels, 0, 0

data, labels, xanon, yanon = fixed()

colors = ["bo", "ro", "go", "yo", "mo", "co"]

for results, color, label in zip(data, colors, labels):
    delays      = map(lambda x: x[0], results)
    throughputs = map(lambda x: x[1], results)
    labels      = map(lambda x: x[2], results)

    plt.plot(delays, throughputs, color, label=label)
    for i in range(len(labels)):
        plt.annotate(labels[i], xy = (delays[i] + xanon, throughputs[i] + yanon))
plt.ylabel('throughput(Mbits/s)')
plt.xlabel('delay(ms)')
plt.legend()
#plt.xscale("log")
plt.gca().invert_xaxis()
plt.show()
