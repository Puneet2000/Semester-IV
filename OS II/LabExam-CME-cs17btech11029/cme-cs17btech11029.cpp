#include <pthread.h>
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <fstream>
#include <random>
#include <limits.h>
#include <algorithm>
#include <sys/time.h>
using namespace std;

bool USE_FFLUSH = 0;
int n , k, S; // number of threads and their frequency
float lambda1 , lambda2 ; // average time for critical and remainder section
FILE* f=fopen("CME-Log.txt","w");
pthread_mutex_t mutex,mtx; // mutex lock for condition variable
pthread_cond_t condition; 
int curr_class;
int entry_count=0;
int request_time = 0;

string getTime(time_t input) // convert  rawtime to time format
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}

void entry_sec(int s){
	pthread_mutex_lock(&mutex); // aqquire lock first
    if (entry_count>0 && s!=curr_class) { // wait in condition current session is different
       pthread_cond_wait(&condition, &mutex);
   	}
   	entry_count++;
   	if(entry_count==1)
   		curr_class = s;
    pthread_mutex_unlock(&mutex); // release mutex .
}

void exit_sec(){
	pthread_mutex_lock(&mutex); // aqquire lock
	entry_count--;
	if(entry_count==0)
		pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex); // release lock
}

void *testCS(void *param){

	int tid = *(int*)param; // thread ID

	default_random_engine generator;
	exponential_distribution<double> d1(1.0/lambda1); // critical section time
	exponential_distribution<double> d2(1.0/lambda2); // remainder section time

	for(int i=0;i<k;i++){

		int s = rand()%S +1;

		struct timeval reqTime;
		gettimeofday(&reqTime,NULL);
		fprintf(f," %d CS Request at %s by thread %d for session %d\n" , i+1 , getTime(reqTime.tv_sec).c_str() , tid,s);
		if (USE_FFLUSH) fflush(f);
		entry_sec(s);

		// critical section stimulation		
		struct timeval enterTime;
		gettimeofday(&enterTime,NULL);
		double wait = (enterTime.tv_sec - reqTime.tv_sec)*1e6 + (enterTime.tv_usec - reqTime.tv_usec);
		pthread_mutex_lock(&mtx);
		request_time+=wait;
		pthread_mutex_unlock(&mtx);

  		fprintf(f," %d CS Entry at %s by thread %d for session %d\n" , i+1 , getTime(enterTime.tv_sec).c_str() , tid,s); // print log
  		if (USE_FFLUSH) fflush(f);
  		usleep(d1(generator)*1e6);

  		struct timeval exitTime;
		gettimeofday(&exitTime,NULL);
  		fprintf(f," %d CS Exit at %s by thread %d for session %d \n" , i+1 , getTime(exitTime.tv_sec).c_str() , tid,s);
  		if (USE_FFLUSH) fflush(f);

  		exit_sec();

  		// remainder section
  		usleep(d2(generator)*1e6);
	}

}

int main(){
	ifstream in;
	in.open("inp-params.txt");
	in>>n>>k>>S>>lambda1>>lambda2; // get input parameters
	pthread_t tid[n];
	int ids[n];
	pthread_attr_t attr[n];
	pthread_mutex_init(&mutex, NULL); 
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&condition, NULL);
	for(int i=0;i<n;i++){ // crrate n threads
		ids[i] = i+1;
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i] ,&attr[i],testCS,&ids[i]);
	}

	for(int i=0;i<n;i++){ // wait for all threads to finish
		pthread_join(tid[i],NULL);
	}

	//cout<<"Average request time is : "<<(request_time*1e-6/(n*k))<<endl;


	return 0;
}