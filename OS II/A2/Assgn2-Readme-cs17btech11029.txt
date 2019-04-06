## Implementing TAS, CAS and Bounded Waiting CAS

Mutual Exclusion Algorithms

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "inp-params.txt". Give the input in following format :

    N K L1 L2

    where  N : number of threads
           K : frequency of each thread
           L1 : Average of exponential distribution for critical section
           L2 : Average of exponential distribution for remainder section
    
    Eg:
    10 10 5 20


2. Compile the source code files as follows :

    $ g++ SrcAssgn2-tas-cs17btech11029.cpp -pthread -o tas
    $ g++ SrcAssgn2-cas-cs17btech11029.cpp -pthread -o cas
    $ g++ SrcAssgn2-cas-bounded-cs17btech11029.cpp -pthread -o casb


3. Run the generated binaries as follows : 

    $ ./tas && ./cas && ./casb

4. Check "TAS-Log.txt" for log corresponding to TAS

5. Check "CAS-Log.txt" for log corresponding to CAS

6. Check "CAS-Bounded-Log.txt" for log corresponding to CAS Bounded.

----------------------------------------------------------------------------