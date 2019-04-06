import matplotlib.pyplot as plt 
fig = plt.figure()

x1 = ["3","5","10","15","20"] 
y1 = [
691.313,
717.957,
587.601,
642.646,
688.33
] 

y2 = [
127.919,
95.4846,
46.1939,
40.3655,
26.1091
] 

plt.plot(x1, y1, label = "Average waiting time") 
plt.plot(x1, y2, label = "Worst case waiting time")
plt.xlabel('Number of philosophers') 
plt.ylabel('Time (ms)')
plt.legend()
plt.title("Performance Graph") 

plt.savefig('./g.png')