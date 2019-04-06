import matplotlib.pyplot as plt 
  
# line 1 points 
x = [0.1 , 0.2 , 0.4 , 0.6, 0.8 , 1 ,2 , 4 ,6 , 8 , 10] 
# y1 = [2.15e-06,2.35e-06,4.14e-06,2.36e-06,3.69e-06,2.11e-06,2.18e-06,2.76e-06,3.84e-06,8.19e-06,3.28e-06] 
# # plotting the line 1 points  
# plt.plot(x, y1, label = "Producer (Locks)") 
  
y2 = [0.00868451,0.019553,0.0413446,0.0875655,0.150276,0.224317,1.47158,4.23624,7.26563,9.36414,12.7646]  
# plotting the line 2 points  
plt.plot(x, y2, label = "Consumer (Locks)") 

# y3 = [1.37e-06,1.4e-06,2.88e-06,4.16e-06,1.65e-06,1.49e-06,1.78e-06,1.35e-06,1.57e-06,1.34e-06,1.36e-06]  
# # plotting the line 2 points  
# plt.plot(x, y3, label = "Producer (Semaphores)") 

y4 = [0.00880743,0.018791,0.0435606,0.0768925,0.137869,0.238513,1.72873,5.00351,8.13856,11.3794,14.6516]  
# plotting the line 2 points  
plt.plot(x, y4, label = "Consumer (Semaphores)")
# naming the x axis 
plt.xlabel('The ratio of the average delays ( u_p/u_c )') 
# naming the y axis 
plt.ylabel('Average waiting time (s)') 
# giving a title to my graph 
plt.title('Average waiting time for consumers \n ( locks and semaphores implemenation )') 
  
# show a legend on the plot 
plt.legend() 
plt.grid()
# function to show the plot 
plt.savefig('./cons.png')
