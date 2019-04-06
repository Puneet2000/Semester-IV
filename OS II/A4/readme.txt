## Implement solutions to Readers-Writers and Fair Readers-Writers problems using Semaphores

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "inp-params.txt". Give the input in following format :

   nw, nr, kw , kr , μcs, μrem.

    where  capacity : Buffer capacity
           nw / nr : number of writers/readers threads.
           kw / kr: frequency of each writers/readers
           μcs /μrem.: Average time delays to stimulate critical and remainder section.
    
    Eg:
    10 10 10 10 0.1 0.1


2. Compile the source code files as follows :

    $ g++ rw-cs17btech11029.cpp -pthread -o rw
    $ g++ frw-cs17btech11029.cpp -pthread -o frw

3. Run the generated binaries as follows : 

    $ ./rw && ./frw 

4. Check "RW-log.txt" for log corresponding to standard implementation.

5. Check "FairRW-log.txt" for log corresponding to fair implementation.

6. Check "Average_time.txt" for log average waiting time.

----------------------------------------------------------------------------