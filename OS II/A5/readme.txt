## Implement Dining Philosopher’s using Conditional Variables

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "inp-params.txt". Give the input in following format :

   n, h, μcs, μrem.

    where  capacity : Buffer capacity
           n : number of philosophers threads.
           h : frequency of each philosopher thread.
           μcs /μrem.: Average time delays to stimulate critical(eating) and remainder(thinking) section.
    
    Eg:
    20 10 1 2


2. Compile the source code files as follows :

    $ g++ dphil-cs17btech11029.cpp -pthread -o phil

3. Run the generated binaries as follows : 

    $ ./phil 

4. Check "dphil-log.txt" for log.

5. Check "Times.txt" for log average waiting time and worst case waiting time of a thread.

----------------------------------------------------------------------------