## Implement Class Mutual exculsion

Author : Puneet Mangla ( CS17BTECH11029 )

----------------------------------------------------------------------------

1. Create a file "inp-params.txt". Give the input in following format :

   n, k, S, μcs, μrem.

    where  capacity : Buffer capacity
           n : number threads.
           k : frequency .
           S : number of classes
           μcs /μrem.: Average time delays to stimulate critical and remainder section in **Seconds**.
    
    Eg:
    16 100 5 5 20


2. Compile the source code files as follows :

    $ g++ cme-cs17btech11029.cpp -pthread -o cme
    $ g++ sme-cs17btech11029.cpp -pthread -o sme

3. Run the generated binaries as follows : 

    $ ./cme && ./sme 

4. Check "CME-Log.txt" and "TAS-Log.txt" for logs.

----------------------------------------------------------------------------