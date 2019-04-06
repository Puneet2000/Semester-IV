import matplotlib.pyplot as plt 
  
# line 1 points 
x = [20,30,40,50,60,70,80,90,100] 
y1 = [0,0,0,0,0,0,1,2,8] 
# plotting the line 1 points  
plt.plot(x, y1, label = "RMS") 
  
y2 = [0,0,0,0,0,0,0,0,0] 
# plotting the line 2 points  
plt.plot(x, y2, label = "EDFS") 
  
# naming the x axis 
plt.xlabel('Nummber of processes') 
# naming the y axis 
plt.ylabel('Number of missed deadlines') 
# giving a title to my graph 
plt.title('Missed Deadlines ( CPU utilization < 1)') 
  
# show a legend on the plot 
plt.legend() 
plt.grid()
# function to show the plot 
plt.savefig('./deadlinescpu.png')
