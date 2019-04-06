#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <random>
#include <limits.h>
#include <cstdlib>
#include <semaphore.h>
#include<sys/time.h>
using namespace std;

// input parameters
int n_w , n_r , k_w , k_r ; 
float mu_cs , mu_rem ;
int read_count = 0;
FILE* f=fopen("FairRW-log.txt","w"); // file pointer
FILE *f2=fopen("Average_time.txt","a");

default_random_engine gen1 , gen2 ;
exponential_distribution<double> cs;
exponential_distribution<double> rem;
// semaphores 
sem_t mutex , rw_mutex , queue;

string getTime(time_t input) // converts rawtime to readable string
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}

double reader_avg_time = 0 , writer_avg_time = 0 , reader_max_time =0 , writer_max_time = 0;

// readers code
void *reader(void *param){
	int tid = *(int*)param; // reader thread ID

	for(int i=0;i<k_r;i++){
		struct timeval reqTime;
		gettimeofday(&reqTime,NULL);
		// entry section
		fprintf(f," %d CS Request by Reader Thread %d at %s \n" , i+1 , tid , getTime(reqTime.tv_sec).c_str());

		sem_wait(&queue); // block other threads 
		sem_wait(&mutex);
		read_count++;
		if (read_count == 1) // block writers thread 
			sem_wait(&rw_mutex);
		sem_post(&queue); // release semaphore
		sem_post(&mutex);

		struct timeval enterTime;
		gettimeofday(&enterTime,NULL);

		double wait = (enterTime.tv_sec - reqTime.tv_sec)*1e6 + (enterTime.tv_usec - reqTime.tv_usec);
		reader_avg_time += wait;
		reader_max_time =  max(reader_max_time, wait);
		// critical section
		fprintf(f," %d CS Entry by Reader Thread %d at %s \n" , i+1 , tid , getTime(enterTime.tv_sec).c_str());
		gen1.seed(tid*n_r + i);
		usleep(cs(gen1)*1e6); // stimmulate critical section

		sem_wait(&mutex);
		read_count--;
		if (read_count == 0)
			sem_post(&rw_mutex);
		sem_post(&mutex);

		struct timeval exitTime;
		gettimeofday(&exitTime,NULL);
		// exit section
		fprintf(f," %d CS Exit by Reader Thread %d at %s \n" , i+1 , tid , getTime(exitTime.tv_sec).c_str());	
		gen2.seed(tid*n_r + i);
		usleep(rem(gen2)*1e6); // stimulate remainder section.
	}
}

// writers code
void *writer(void *param){
	int tid = *(int*)param; // writer thread ID

	for(int i=0;i<k_w;i++){

		struct timeval reqTime;
		gettimeofday(&reqTime,NULL);
		// entry section
		fprintf(f," %d CS Request by Writer Thread %d at %s \n" , i+1 , tid , getTime(reqTime.tv_sec).c_str());
		
		sem_wait(&queue); // block queue
		sem_wait(&rw_mutex); // block readers and writers
		sem_post(&queue);

		struct timeval enterTime;
		gettimeofday(&enterTime,NULL);

		double wait = (enterTime.tv_sec - reqTime.tv_sec)*1e6 + (enterTime.tv_usec - reqTime.tv_usec);
		writer_avg_time += wait;
		writer_max_time =  max(writer_max_time, wait);
		// critical section
		fprintf(f," %d CS Entry by Writer Thread %d at %s \n" , i+1 , tid , getTime(enterTime.tv_sec).c_str());
		gen1.seed(tid*n_w + i);
		usleep(cs(gen1)*1e6); // stimulate critical section

		sem_post(&rw_mutex);

		struct timeval exitTime;
		gettimeofday(&exitTime,NULL);
		// exit section
		fprintf(f," %d CS Exit by Writer Thread %d at %s \n" , i+1 , tid , getTime(exitTime.tv_sec).c_str());
		gen2.seed(tid*n_w + i);
		usleep(rem(gen2)*1e6); // stimulate remainder section.
	}
}

int main(){

	ifstream in;
	in.open("inp-params.txt");
	in >> n_w >> n_r >> k_w >> k_r >> mu_cs >> mu_rem ; // take inp parameters
	cs = exponential_distribution<double>(1/mu_cs);
	rem = exponential_distribution<double>(1/mu_rem);
	// writer threads
	pthread_t writer_tid[n_w];
	int writer_ids[n_w];
	pthread_attr_t writer_attr[n_w];

	// reader threads
	pthread_t reader_tid[n_r];
	int reader_ids[n_r];
	pthread_attr_t reader_attr[n_r];

	// semaphores initialisations
	sem_init(&mutex, 0, 1);
	sem_init(&rw_mutex, 0, 1);
	sem_init(&queue,0,1);

	// spawn writer threads
	for(int i=0;i<n_w;i++){ 
		writer_ids[i] = i+1;
		pthread_attr_init(&writer_attr[i]);
		pthread_create(&writer_tid[i] ,&writer_attr[i],writer,&writer_ids[i]);
	}

	// spawn readers threads
	for(int i=0;i<n_r;i++){ 
		reader_ids[i] = i+1;
		pthread_attr_init(&reader_attr[i]);
		pthread_create(&reader_tid[i] ,&reader_attr[i],reader,&reader_ids[i]);
	}

	// wait till all threads are finished
	for(int i=0;i<n_w;i++){ 
		pthread_join(writer_tid[i],NULL);
	}

	for(int i=0;i<n_r;i++){ 
		pthread_join(reader_tid[i],NULL);
	}
	
	fprintf(f2,"Average time in fair reader-wrters is %lf miliseconds\n" , (writer_avg_time + reader_avg_time)/(n_r*k_r*1e3 + n_w*k_w*1e3));
	// cout<<reader_avg_time/(n_r*k_r*1e3)<<endl;
	// cout<<writer_avg_time/(n_w*k_w*1e3)<<endl;
	// cout<<reader_max_time*1e-3<<endl;
	// cout<<writer_max_time*1e-3<<endl;
	return 0;
}