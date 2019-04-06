#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
ofstream out;
int main(int argc , char* argv[]){
    out.open("in.txt");
    int n;
    n=stoi(argv[1]);
    out<<n<<endl;
    srand(time(NULL));
    int arr[n*3];
    for(int i=0;i<n*3;i++) arr[i]=rand();
    for(int i=0;i<n*3;i+=3){
        out<<i/3+1<<" "<<10 + arr[i]%20<<" "<<150 + arr[i+1]%100<<" "<<10<<endl;
    }
}