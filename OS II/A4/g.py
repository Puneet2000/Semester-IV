import matplotlib.pyplot as plt 
fig = plt.figure(figsize=(20,10))
plt.subplot(2, 2, 1)
# line 1 points 
x1 = [1,5,10,15,20] 
y1 = [
966.417,
238.144,
58.1797,
83.1937,
60.8161
] 

y2 = [
982.229,
1204.94,
1475.74,
1708.53,
1729.24
] 

y3 = [
890.004,
986.39,
1071.53,
1144.24,
1072.96
] 

y4 = [
903.475,
1142.21,
1418.68,
1705.73,
1665.92
]  

plt.plot(x1, y1, label = "Simple RW (Readers)") 
plt.plot(x1, y2, label = "Fair RW (Readers)")
plt.plot(x1, y3, label = "Simple RW (Writers)") 
plt.plot(x1, y4, label = "Fair RW (Writers)")
plt.xlabel('Number of Readers') 
plt.ylabel('Average waiting time (ms)')
plt.legend()
plt.title("Average waiting time ( Writers Fixed )") 

plt.subplot(2, 2, 2)
y1 = [
1439.93,
1044.62,
775.861,
1124.23,
835.448

] 

y2 = [
982.229,
1204.94,
1475.74,
1708.53,
1729.24
] 

y3 = [
1637.68,
1888.6,
3405.3,
4351.06,
3310.19
] 

y4 = [
1608.67,
2346.9,
2776.03,
3164.48,
2802.76
]  

plt.plot(x1, y1, label = "Simple RW (Readers)") 
plt.plot(x1, y2, label = "Fair RW (Readers)")
plt.plot(x1, y3, label = "Simple RW (Writers)") 
plt.plot(x1, y4, label = "Fair RW (Writers)")
plt.xlabel('Number of Readers') 
plt.ylabel('Worst waiting time (ms)')
plt.legend()
plt.title("Worst case waiting time ( Writers Fixed )") 

plt.subplot(2, 2, 3)
y1 = [
2.30593,
55.053,
63.3404,
251.822,
155.924

] 

y2 = [
150.787,
946.295,
1571.56,
2328.04,
2295.91
] 

y3 = [
239.244,
528.359,
1071.99,
1693.64,
1990.75
] 

y4 = [
251.826,
931.417,
1556.06,
2240.1,
2196.19
]  

plt.plot(x1, y1, label = "Simple RW (Readers)") 
plt.plot(x1, y2, label = "Fair RW (Readers)")
plt.plot(x1, y3, label = "Simple RW (Writers)") 
plt.plot(x1, y4, label = "Fair RW (Writers)")
plt.xlabel('Number of Writers') 
plt.ylabel('Average waiting time (ms)')
plt.legend()
plt.title("Average waiting time ( Readers Fixed )") 


plt.subplot(2, 2, 4)
y1 = [
275.253,
509.915,
836.375,
2519.2,
1848.59

] 

y2 = [
650.891,
1629.68,
2797.33,
3593.12,
3259.93
] 

y3 = [
2386.17,
2974.33,
3459.74,
4935.8,
4544.03
] 

y4 = [
640.722,
1389.14,
3563.07,
3512.33,
4365.52
]  

plt.plot(x1, y1, label = "Simple RW (Readers)") 
plt.plot(x1, y2, label = "Fair RW (Readers)")
plt.plot(x1, y3, label = "Simple RW (Writers)") 
plt.plot(x1, y4, label = "Fair RW (Writers)")
plt.xlabel('Number of Writers') 
plt.ylabel('Worst waiting time (ms)')
plt.legend()
plt.title("Worst case waiting time ( Readers Fixed )") 

plt.savefig('./g.png')