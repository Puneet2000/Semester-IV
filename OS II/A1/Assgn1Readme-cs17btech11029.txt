
## Implementing Rate-Monotonic Scheduling & Earliest Deadline First Scheduling through Discrete Event Simulation

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "in.txt". Give the input in following format :

    n  // number of processes
    // then n lines follow where ith line is :
    process_id[i] processing_time[i] period[i] repeats[i]
    
    Eg:
    2
    1 25 50 3
    2 35 80 4


2. Compile the source code files as follows :

    $ g++ Assgn1-RMScs17btech11029.cpp -o rms
    $ g++ Assgn1-EDFcs17btech11029.cpp -o edf


3. Run the generated binaries as follows : 

    $ ./rms
    $ ./edf

4. Check RMS-Log.txt and  EDF-Log.txt file for logs of RMS and EDF respectively .

5. Check RMS-Stats.txt and EDF-Stats.txt for stats of RMS and EDF respectively.

----------------------------------------------------------------------------