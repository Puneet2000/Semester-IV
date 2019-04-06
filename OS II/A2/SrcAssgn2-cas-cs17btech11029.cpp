// necessary libraries
#include <pthread.h>
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <fstream>
#include <random>
#include <limits.h>
using namespace std;

int n , k ; // number of threads and their frequency
float lambda1 , lambda2 ; // average time for critical and remainder section
atomic<int> lock (0); // lock variable
FILE* f=fopen("CAS-Log.txt","w");
time_t wait_time = 0 ; // total waiting time
time_t max_wait_time =  INT_MIN; // max waiting time

string getTime(time_t input) // convert  rawtime to time format
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}


void *testCAS(void *param){

	int tid = *(int*)param; // thread ID

	default_random_engine generator;
	exponential_distribution<double> d1(1.0/lambda1); // critical section time
	exponential_distribution<double> d2(1.0/lambda2); // remainder section time

	for(int i=0;i<k;i++){
		// entry section
		time_t reqEnterTime = time(NULL);
		fprintf(f," %d CS Request at %s by thread %d \n" , i+1 , getTime(reqEnterTime).c_str() , tid);
		while (1){
			int x =0 , y=1;
			if(lock.compare_exchange_strong(x,y)) break;
		}

		// critical section stimulation
  		time_t accEnterTime = time(NULL);
  		wait_time += accEnterTime - reqEnterTime; // calculate metrics
  		max_wait_time = max(max_wait_time,accEnterTime - reqEnterTime);

  		fprintf(f," %d CS Entry at %s by thread %d \n" , i+1 , getTime(accEnterTime).c_str() , tid); // print log
  		usleep(d1(generator)*1e6);

  		time_t exitTime = time(NULL);
  		fprintf(f," %d CS Exit at %s by thread %d \n" , i+1 , getTime(exitTime).c_str() , tid);

  		// exit section
  		lock = 0;	

  		// remainder section stimulation
  		usleep(d2(generator)*1e6);
	}

}

int main(){

	fprintf(f,"CAS ME Output\n");
	ifstream in;
	in.open("inp-params.txt");
	in>>n>>k>>lambda1>>lambda2; // get input parameters

	pthread_t tid[n];
	int ids[n];
	pthread_attr_t attr[n];

	for(int i=0;i<n;i++){ // crrate n threads
		ids[i] = i+1;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i] ,&attr[i],testCAS,&ids[i]);
	}

	for(int i=0;i<n;i++){ // wait for all threads to finish
		pthread_join(tid[i],NULL);
	}

	// cout<<"Average waiting time :" << float(wait_time) / (n*k) <<endl;
	// cout<<"Max waiting time :" << max_wait_time <<endl;


	return 0;
}