#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <queue>
#include <limits.h>
using namespace std;

class Process { // Process class representing a stimulated process
public:
	int pid , process_time , period , deadline , arrival_time , interuppted , nowaiting_time;

	Process(int pid,int process_time,int period,int arrival_time){
		this->pid = pid; // process id
		this->process_time = process_time; // processing time
		this->period =  period; // period of arrival
		this->arrival_time = arrival_time; // arrival time in system
		this->deadline = this->arrival_time + this->period ; // dealine timestamp
		this->interuppted = false; // whether it was preempted
		this->nowaiting_time = this->arrival_time + this->process_time; // completion time without waiting in ready queue
	}

};

bool operator<(const Process& p1, const Process& p2) { // sort all the future process in order of their arrival time
	return p1.arrival_time > p2.arrival_time; 
}

Process schedule(map<int,Process>* ready_queue){ // scheduler to take out highest priority process and return it
	int min = INT_MAX;
	map<int, Process>::iterator min_itr;
	min_itr = ready_queue->begin();
	map<int, Process>::iterator itr; 
	
	for (itr = ready_queue->begin(); itr != ready_queue->end(); itr++) {  // less deadline means high priority
        if ((itr->second).deadline < min){
        	min_itr = itr;
        	min = (itr->second).deadline;
        }
    }

    Process p = min_itr->second;
    ready_queue->erase(min_itr); // erase the selected process from ready queue

    return p;
}

int main(){
	ifstream in; // input file stream
	ofstream out , out2; // output file stream
	in.open("in.txt");
	out.open("EDF-Log.txt");
	out2.open("EDF-Stats.txt");

	int n;
	in>>n;
	priority_queue<Process> processes; // all processes sorted with arrival time
	map<int,Process> ready_queue;

	int time, next_arrival , missed_deadlines =0 , waiting_time=0 , num_processes =0; // program variables

	for(int i=0;i<n;i++){ // fill the priority queue with all processes that will come in order of arrival time
		int pid , process_time , period , k;
		in>>pid>>process_time>>period>>k;
		out<<"Process P"<<pid<<" : processing time="<<process_time<<"; deadline:"<<period<<"; period:"<<period<<" joined the system at time 0"<<endl;
		num_processes+=k;
		for (int j=0;j<k;j++){
			processes.push(Process(pid,process_time,period,j*period));
		}

		processes.push(Process(-1,0,period,k*period));

	}

	Process prev_running(-1,0,INT_MAX,0); // store the current running process if an event occurs
	bool cpu_idle =  false; // whether cpu is in idle state or not

	while(!processes.empty()){ // do till all process are not considered
		time = (processes.top()).arrival_time; // current time

		map<int, Process>::iterator itr , itr2; 
	
		for (itr = ready_queue.begin(); itr != ready_queue.end();) {  // check if any process in ready queue misses deadlines
	        if ((itr->second).deadline <= time && (itr->second).pid > 0){
	        	missed_deadlines++; 
				out<<"Deadline missed by P"<<(itr->second).pid<<" at time "<<time<<endl;
				waiting_time += time - (itr->second).nowaiting_time + (itr->second).process_time;
				itr2 = itr;
				itr++;
				ready_queue.erase(itr2); // remove form ready queue if deadline expires

	        }
	        else itr++;
	    }

	    if (prev_running.deadline <= time && prev_running.pid > 0){ // check the previously running processs for deadline miss
	    	out<<"Deadline missed by P"<<prev_running.pid<<" at time "<<time<<endl;
			waiting_time += time - prev_running.nowaiting_time + prev_running.process_time;
			prev_running =  Process(-1,0,INT_MAX,0);
			missed_deadlines++;	
		}


		while ((processes.top()).arrival_time==time && !processes.empty()){ // fill in the ready queue with all processes arriving at current time
			Process p = processes.top();
			processes.pop();
			if (p.pid > 0)
				ready_queue.insert(pair<int,Process>(p.pid,p));	
		}

		if(!processes.empty()) // set the next event time 
			next_arrival = (processes.top()).arrival_time;
		else
			next_arrival = INT_MAX; // let all process in ready queue complete execution

		Process running(-1,0,INT_MAX,0);
		if (!ready_queue.empty())
			running = schedule(&ready_queue); // schedule a process from ready queue
		else if (prev_running.pid > 0)
			running = prev_running;
		else {
			time = next_arrival; // if no process is their in queue hop to next event
			continue;
		}

		if (cpu_idle){ // if cpu was idle till this time 
			out<<"CPU is idle till time "<<time<<endl;
			cpu_idle = false;
		}
	
		if (running.deadline<prev_running.deadline){ // compare the selected process from prev running process
			
			if(prev_running.pid >0){
				prev_running.interuppted = true;
				ready_queue.insert(pair<int,Process>(prev_running.pid,prev_running));
				
				out<<"Process P"<<prev_running.pid<<" is preempted by Process P"<<running.pid<<" at time "<<time
				<<".Remaining processing time:"<<prev_running.process_time<<endl;
			}

			if(running.interuppted ){
				out<<"Process P"<<running.pid<<" resumes execution at time "<<time<<endl;
				running.interuppted=false;
			}
			else 
				out<<"Process P"<<running.pid<<" starts execution at time "<<time<<endl;
		}
		else if (running.pid != prev_running.pid){
			ready_queue.insert(pair<int,Process>(running.pid,running));
			running = prev_running;
		}

		bool preempted = false;
		while (preempted = (time + running.process_time <= next_arrival)){ // run till a process can't finish its execution till next event time
			time = time + running.process_time;
			out<<"Process P"<<running.pid<<" finishes at time "<<time<<endl; // finish processs
			waiting_time += time -running.nowaiting_time;

			if (!ready_queue.empty() && time < next_arrival){ // schedule another if ready queue is not empty
				running = schedule(&ready_queue);
				if(running.interuppted){
					out<<"Process P"<<running.pid<<" resumes execution at time "<<time<<endl;
					running.interuppted=false;
				}
				else
					out<<"Process P"<<running.pid<<" starts execution at time "<<time<<endl;
			}
			else{ // otherwise mark cpu as idle
				if(next_arrival==INT_MAX)
					break;
				if (time < next_arrival){
					time = next_arrival;
					cpu_idle = true;
				}
				prev_running = Process(-1,0,INT_MAX,0);
				break;
			}	
		}

		if(!preempted){ // to hop to next even time save the current running process
			running.process_time = time + running.process_time - next_arrival;
			prev_running = running;
		}
	
	}

	// print the output stats

	out2<<"Number of Processes : "<<num_processes<<endl<<"Number of completed processes : "<<num_processes-missed_deadlines<<endl
	<<"Number of processes that missed deadlines : "<<missed_deadlines<<endl<<"Average waiting time :"<<float(waiting_time)/num_processes<<endl;


	return 0;
}
