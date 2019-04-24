#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
int main(){
	int iters = 0,j;
	char *ptr;
	while(1){
		printf("iteration %d\n",iters);
		sleep(1);
		ptr = (char*)malloc(100*1024*1024*sizeof(char));
		if (ptr == NULL)
			break;
		for(j=0;j<100*1024*1024;j++)
			ptr[j] = '0';
		iters++;
	}
	printf("Malloc failure at iteration %d\n",iters);
	char c;
	scanf("%c",&c);
	return 0;
}