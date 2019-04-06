import matplotlib.pyplot as plt 
  
# line 1 points 
x = [0.1 , 0.2 , 0.4 , 0.6, 0.8 , 1 ,2 , 4 ,6 , 8 , 10] 
y1 = [2.15,2.35,4.14,2.36,3.69,2.11,2.18,2.76,3.84,8.19,3.28] 
# plotting the line 1 points  
plt.plot(x, y1, label = "Producer (Locks)") 
  
y3 = [1.37,1.4,2.88,4.16,1.65,1.49,1.78,1.35,1.57,1.34,1.36]  
# plotting the line 2 points  
plt.plot(x, y3, label = "Producer (Semaphores)") 

# naming the x axis 
plt.xlabel('The ratio of the average delays ( u_p/u_c )') 
# naming the y axis 
plt.ylabel('Average waiting time (us)') 
# giving a title to my graph 
plt.title('Average waiting time for producers\n( locks and semaphores implemenation )') 
  
# show a legend on the plot 
plt.legend() 
plt.grid()
# function to show the plot 
plt.savefig('./pro.png')
