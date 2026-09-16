import matplotlib.pyplot as plt

threads = []
times = []

with open("times.txt") as file:
    for line in file:
        p, time = line.split()
        threads.append(int(p))
        times.append(float(time) * 1000)

plt.plot(threads, times, "o-")
plt.xticks(threads)
plt.xlabel("Количество потоков")
plt.ylabel("Среднее время, мс")
plt.title("Время вычисления интеграла")
plt.grid()
plt.tight_layout()
plt.savefig("graph.png", dpi=150)
