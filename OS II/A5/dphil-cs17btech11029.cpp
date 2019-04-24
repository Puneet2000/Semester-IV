#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <random>
#include <limits.h>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
using namespace std;

// input parameters
int n, h; 
float mu_cs , mu_rem ;
FILE* f=fopen("dphil-log.txt","w");
FILE *f2=fopen("Times.txt","w");

default_random_engine gen1 , gen2 ;
exponential_distribution<double> cs; // exponential generator for eating and thinking
exponential_distribution<double> rem; 

string getTime(time_t input) // converts rawtime to readable string
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}

enum state {THINKING, HUNGRY, EATING}; // States of the philosopher
pthread_mutex_t mutex; // mutex lock for condition variable
pthread_cond_t *condition; // array of condition variables
enum state *status;
double avg_time = 0 , max_time = 0; // metrics

void test (int i){ // test function for pickup and pushdown
	if ((status[(i+ n-1)%n] != EATING) && (status[i] == HUNGRY) && (status[(i+1)%n] != EATING)){
		status[i] = EATING;
		pthread_cond_signal(&condition[i]); //signal thread if it can eat
	}
}

void pickup(int i){
	pthread_mutex_lock(&mutex); // aqquire lock first
    status[i] = HUNGRY;
    test(i);
    if (status[i] != EATING) { // wait in condition queue if thread can't pick chopsticks
       pthread_cond_wait(&condition[i], &mutex);
   	}
    pthread_mutex_unlock(&mutex); // release mutex after picking chopsticks.
}

void putdown(int i){
	pthread_mutex_lock(&mutex); // aqquire lock
    status[i] = THINKING; // change state and check if neighbour threads can eat.
    test((i+1)%n);
    test((i+n-1)%n);
    pthread_mutex_unlock(&mutex); // release lock
}

void *philosopher(void *param){ // stimulation of philosopher
	int tid = *(int*)param; 

	for(int i=0;i<h;i++){
		struct timeval reqTime;
		gettimeofday(&reqTime,NULL);
		// entry section
		fprintf(f," %d eat request by Thread %d at %s \n" , i+1 , tid , getTime(reqTime.tv_sec).c_str());

		pickup(tid-1); // pick the chopsticks

		struct timeval enterTime;
		gettimeofday(&enterTime,NULL);
		double wait = (enterTime.tv_sec - reqTime.tv_sec)*1e6 + (enterTime.tv_usec - reqTime.tv_usec);
		avg_time += wait;
		max_time =  max(max_time, wait);

		// critical section
		fprintf(f," %d CS Entry by Philosopher Thread %d at %s \n" , i+1 , tid , getTime(enterTime.tv_sec).c_str());
		gen1.seed(tid*n + i);
		usleep(cs(gen1)*1e6); // stimulate eating section

		putdown(tid-1); // put the chopsticks down

		struct timeval exitTime;
		gettimeofday(&exitTime,NULL);

		// remainder section
		fprintf(f," %d CS Exit by Philosopher Thread %d at %s \n" , i+1 , tid , getTime(exitTime.tv_sec).c_str());
		gen2.seed(tid*n + i);
		usleep(rem(gen2)*1e6); // stimulate thinking section
	}
}

int main(){

	ifstream in;
	in.open("inp-params.txt");
	in >> n >> h >> mu_cs >> mu_rem ; 
	cs = exponential_distribution<double>(1/mu_cs);
	rem = exponential_distribution<double>(1/mu_rem);

	pthread_t tid[n]; // thread ids's
	int ids[n];
	pthread_attr_t attr[n];
	pthread_mutex_init(&mutex, NULL); // initialise mutex
	condition = (pthread_cond_t*)malloc(n*sizeof(pthread_cond_t)); // intialise condition variables
	status = (enum state*)malloc(n*sizeof(enum state));

	for(int i=0;i<n;i++){
		status[i] = THINKING;
		pthread_cond_init(&condition[i], NULL); // set status of all thread to THINKING.
	}

	for(int i=0;i<n;i++){ //spwan threads
		ids[i] = i+1;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i] ,&attr[i],philosopher,&ids[i]);
	}

	for(int i=0;i<n;i++){ // wait for them to finish
		pthread_join(tid[i],NULL);
	}

	fprintf(f2,"Average time is %lf miliseconds\n" ,avg_time*1e-3/(n*h));
	fprintf(f2,"Worst case time is %lf miliseconds\n" ,max_time*1e-3);

	// cout<<avg_time*1e-3/(n*h)<<endl;
	// cout<<max_time*1e-3<<endl;

	return 0;
}