import matplotlib.pyplot as plt 
  
# line 1 points 
x = [100,200,300,400,500] 
y1 = [0.464,1.124,1.797,2.393,3.13] 
# plotting the line 1 points  
plt.plot(x, y1, label = "TAS") 
  
y2 = [0.484,1.092,1.766,2.4885,3.3035]  
# plotting the line 2 points  
plt.plot(x, y2, label = "CAS") 

y2 = [0.542,1.416,2.302,3.337,5.238]  
# plotting the line 2 points  
plt.plot(x, y2, label = "CAS Bounded") 

# naming the x axis 
plt.xlabel('Number of Threads') 
# naming the y axis 
plt.ylabel('Average waiting time (s)') 
# giving a title to my graph 
plt.title('Average waiting time to enter CS') 
  
# show a legend on the plot 
plt.legend() 
plt.grid()
# function to show the plot 
plt.savefig('./avgtime.png')
