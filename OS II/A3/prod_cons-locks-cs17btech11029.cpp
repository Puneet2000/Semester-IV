#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <random>
#include <limits.h>
#include <cstdlib>
#include <mutex> 
#include <sys/time.h>
using namespace std;

// input parameters
int BUFFER_SIZE , n_p , n_c , cnt_p , cnt_c; 
double mu1 , mu2 ; 

FILE* f=fopen("Mutex-Log.txt","w"); // file pointer
int *buffer;  // buffer for storing item produced
int count = 0 ,out =0 ,in=0; // variables for producer and consumer

// mutex locks
mutex mtx , mtx2 ;
double pro_time = 0, cons_time = 0;

string getTime(time_t input) // converts rawtime to readable string
{
  struct tm * timeinfo;
  timeinfo = localtime (&input);
  char buffer[10];
  sprintf(buffer,"%.2d:%.2d:%.2d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
  string tim(buffer);
  return tim;
}

// producer code
void *producer(void *param){
	int tid = *(int*)param; // producer thread ID

	default_random_engine generator(tid); // random generation for stimulating generation of item
	exponential_distribution<double> d1(1.0/mu1);

	struct timeval  tv1, tv2;
	for(int i=0;i<cnt_p;i++){
		srand(tid*cnt_p + i); // seed the random function
		int item = rand()%BUFFER_SIZE; // generate item
		usleep(d1(generator)*1e6); // stimulation
		gettimeofday(&tv1, NULL);
		while (true){ // wait for any space to be left
			mtx2.lock();
			if (count < BUFFER_SIZE){
				count++;
				break;
			}
			mtx2.unlock();
		}
		mtx.lock();
		mtx2.unlock(); // // place the item in buffer
		gettimeofday(&tv2, NULL);
		pro_time += (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)/1e6; // add waiting time
		buffer[in] = item;
		fprintf(f," %dth item: %d produced by thread %d at %s into buffer location %d \n" , i+1 ,item , tid, getTime(time(NULL)).c_str(),in);
		in = (in + 1) % BUFFER_SIZE;
		fflush(f);
		mtx.unlock(); // relaese lock

	}

}

// consumer code
void *consumer(void *param){
	int tid = *(int*)param;(tid); // consumer thread ID

	default_random_engine generator(tid); // random generation for stimulating after consumption time
	exponential_distribution<double> d2(1.0/mu2);
	struct timeval  tv1, tv2;
	for(int i=0;i<cnt_c;i++){
		gettimeofday(&tv1, NULL);
		while (true){
			mtx2.lock();
			if (count > 0){ // wait till atleast one valid item is in buffer
				count--;
				break;
			}
			mtx2.unlock();
		}
		mtx.lock(); // consume the item
		mtx2.unlock();
		gettimeofday(&tv2, NULL);
		cons_time += (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)/1e6; // adding waiting time
		int item = buffer[out];
		fprintf(f," %dth item: %d consumed by thread %d at %s from buffer location %d \n" , i+1 ,item , tid, getTime(time(NULL)).c_str() , out);
		out = (out + 1) % BUFFER_SIZE; // update shard variables
		fflush(f);
		mtx.unlock();
		usleep(d2(generator)*1e6); // stimulation
	}

}

int main(){
	fflush(stdout);
	ifstream in;
	in.open("inp-params.txt");
	in >> BUFFER_SIZE >> n_p >> n_c >> cnt_p >> cnt_c >> mu1 >> mu2 ; 
	// producer threads
	pthread_t producer_tid[n_p];
	int producer_ids[n_p];
	pthread_attr_t producer_attr[n_p];

	// consumer threads
	pthread_t consumer_tid[n_c];
	int consumer_ids[n_c];
	pthread_attr_t consumer_attr[n_c];

	buffer = (int*)malloc(BUFFER_SIZE*sizeof(int));
	struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);

	// spawn producer threads
	for(int i=0;i<n_p;i++){ 
		producer_ids[i] = i+1;
		pthread_attr_init(&producer_attr[i]);
		pthread_create(&producer_tid[i] ,&producer_attr[i],producer,&producer_ids[i]);
	}

	// spawn consumer threads
	for(int i=0;i<n_c;i++){ 
		consumer_ids[i] = i+1;
		pthread_attr_init(&consumer_attr[i]);
		pthread_create(&consumer_tid[i] ,&consumer_attr[i],consumer,&consumer_ids[i]);
	}

	// wait till all threads are finished
	for(int i=0;i<n_p;i++){ 
		pthread_join(producer_tid[i],NULL);
	}

	for(int i=0;i<n_c;i++){ 
		pthread_join(consumer_tid[i],NULL);
	}
	gettimeofday(&tv2, NULL);
	free(buffer);
	double elapsed = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)/1e6;
	// cout<<"Time taken "<<elapsed<<endl;
	// cout<<"Producer time "<<pro_time/(cnt_p*n_p)<<endl;
	// cout<<"Consumer time "<<cons_time/(cnt_c*n_c)<<endl;

	return 0;
}