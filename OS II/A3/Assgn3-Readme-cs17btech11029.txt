## Solving Producer Consumer Problem using Semaphores and Locks

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "inp-params.txt". Give the input in following format :

   capacity, np, nc, cntp, cntc, μp, μc.

    where  capacity : Buffer capacity
           np / nc : number of producers/consumers threads.
           cntp / cntc: frequency of each producers/consumers
           μp /μc.: Average time delays in producing/consuming item by producer/consumer respectively.
    
    Eg:
    100 10 15 15 10 8 12


2. Compile the source code files as follows :

    $ g++ prod_cons-locks-cs17btech11029.cpp -pthread -o mex
    $ g++ prod_cons-sems-cs17btech11029.cpp -pthread -o sem

3. Run the generated binaries as follows : 

    $ ./mex && ./sem 

4. Check "Semaphore-Log.txt" for log corresponding to semaphore implementation.

5. Check "Mutex-Log.txt" for log corresponding to mutex locks implementation.

----------------------------------------------------------------------------