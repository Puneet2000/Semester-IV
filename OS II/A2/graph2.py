import matplotlib.pyplot as plt 
  
# line 1 points 
x = [100,200,300,400,500] 
y1 = [3,8.2,11.8,18.4,23.75] 
# plotting the line 1 points  
plt.plot(x, y1, label = "TAS") 
  
y2 = [2.8,7.4,12.8,17.2,24.75]  
# plotting the line 2 points  
plt.plot(x, y2, label = "CAS") 

y2 = [2.4,5.4,8.6,11.6,15.5]  
# plotting the line 2 points  
plt.plot(x, y2, label = "CAS Bounded") 

# naming the x axis 
plt.xlabel('Number of Threads') 
# naming the y axis 
plt.ylabel('Worst case waiting time (s)') 
# giving a title to my graph 
plt.title('Worst case waiting time to enter CS') 
  
# show a legend on the plot 
plt.legend() 
plt.grid()
# function to show the plot 
plt.savefig('./worsttime.png')
