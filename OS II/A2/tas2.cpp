#include <pthread.h>
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <fstream>
#include <random>
#include <mutex>
#include <thread>
using namespace std;

int n=3 , k=4 , lambda1 , lambda2 ;
//atomic_flag lock = ATOMIC_FLAG_INIT;
FILE* f=fopen("TAS-Log.txt","w");

string getTime(time_t input)
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}

bool locki = false;
mutex mtx;

bool test_and_set(bool *lo){
	synchronized {
		//mtx.lock();
		bool ret = *lo;
		*lo = true;
		//mtx.unlock();
		return ret;
	}
}

void *testTAS(void *param){

	int tid = *(int*)param;
	default_random_engine generator;
	exponential_distribution<double> d1(lambda1);
	exponential_distribution<double> d2(lambda2);

	for(int i=0;i<k;i++){
		time_t reqEnterTime = time(NULL);
		printf(" %d CS Request at %s by thread %d \n" , i+1 , getTime(reqEnterTime).c_str() , tid);
		while (test_and_set(&locki));		
  		time_t accEnterTime = time(NULL);
  		printf(" %d CS Entry at %s by thread %d \n" , i+1 , getTime(accEnterTime).c_str() , tid);
  		sleep(0.01);
  		locki= false;
  		time_t exitTime = time(NULL);
  		printf(" %d CS Exit at %s by thread %d \n" , i+1 , getTime(exitTime).c_str() , tid);
  		sleep(0.1);
	}

}

int main(){

	pthread_t tid[n];
	pthread_attr_t attr[n];
	for(int i=0;i<n;i++){
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i] ,&attr[i],testTAS,&i);
	}

	for(int i=0;i<n;i++){
		pthread_join(tid[i],NULL);
	}

	return 0;
}