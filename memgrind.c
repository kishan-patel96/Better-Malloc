#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"
#include "mymalloc.c"
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

 double A(){

 	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	
	int i = 0;

	// mallocs and immediately frees 1 byte of memory (150 times)
	while(i<150){
		char * a = (char*)malloc(1*sizeof(char)); // mallocs 1 byte of mem
		free(a); // immediately frees it
		i++;
	}
	
	gettimeofday(&t2, NULL);
	
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;

}

double B(){


 	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	char * a [150]; // creates array of size 150
	int i; 
	for(i = 0; i < 150; i++){
		a[i] = (char*)malloc(1); // calls malloc 150 times, each time asking for 1 byte
	}

	for(i = 0; i < 150; i++){
		free(a[i]); // frees all 150 bytes, one by one
	}

	gettimeofday(&t2, NULL);
	
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;

	
}

double C() {
 	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	srand(time(NULL));
	int *p[150]; //intiatize array of size 150
	int malloced = 0;
    int mal_count = 0;
    int mal_total = 0; // amount of memory allocated
    int freed = 0;
    int random = 0;

    while(mal_count < 150 || freed < 150){
        random = rand() % 2; // randomly picks 0 or 1
        if(random == 0 && mal_count < 150){ //rand() % 2 can only be 0 or 1, so 50% chance of free or malloc
                        
            mal_total = mal_total + 1 + MD_SIZE; // add 1 and metadata size to mal_total
                if(mal_total > 5000){ // malloc cant go over 5000 bytes
                    mal_total = mal_total - 1 - MD_SIZE; // undo the malloc!! if it exceeds 5000 bytes
                }
                else{
                    p[malloced] = malloc(1); // malloc one byte of memory to pertaining index of the array
                    // increase counts
                    malloced++;
                    mal_count++;
                }
        }
        
        else{
            // checks if there are pointers to free and frees them!
            if(malloced != 0){
                // increase/decrease counts
                malloced--;
                freed++;
                mal_total = mal_total - 1 - MD_SIZE;
                free(p[malloced]);

             }
        }
    }

	gettimeofday(&t2, NULL);
	
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;

	
}

double D() {
 	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	srand(time(NULL));
	
	int *p[150]; //intiatize array of size 150
	int s[150];
	int malloced = 0;
    int mal_count = 0;
    int mal_total = 0; // amount of memory allocated
    int freed = 0;
    int random = 0;

    while(mal_count < 150 || freed < 150){
        
        if(rand() % 2 == 0 && mal_count < 150){ //rand() % 2 can only be 0 or 1, so 50% chance of free or malloc
           	random = (rand() % 64) + 1; // randomly picks numbers between 1-64            
            mal_total = mal_total + random + MD_SIZE; // add random size + Metadata size + mal_total
                if(mal_total > 5000){ // malloc cant go over 5000 bytes
                    mal_total = mal_total - random - MD_SIZE; // undo the malloc!! if it exceeds 5000 bytes
                }
                else{
                    p[malloced] = malloc(random); // malloc one byte of memory to pertaining index of the array
                    s[malloced] = random;
                    // increase counts
                    malloced++;
                    mal_count++;
                }
        }
        
        else{
            // checks if there are pointers to free and frees them!
            if(malloced != 0){
                // increase/decrease counts
                malloced--;
                freed++;
                mal_total = mal_total - s[malloced] - MD_SIZE;
                free(p[malloced]); // free it

             }
        }
    }

	gettimeofday(&t2, NULL);
	
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;
	

}

double E(){
 	// this shows that our malloc function allocates the maximum data that it can!
 	// 5000 - sizeof(md) == max memory available - size of meta data
 	struct timeval t1, t2;
	gettimeofday(&t1, NULL);

	int k = 0;
	
	// do this 150 times also
	for(k = 0; k<150; k++){
		char * a = (char*)malloc(5000-sizeof(md));
		free(a);
	}	


	gettimeofday(&t2, NULL);
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;
}

double F(){
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);

	// this will malloc p and free it
	char* p = (char *)malloc( 100 );
	free( p );

	//then it'll malloc and free it again
	p = (char *)malloc( 100 );
	free( p );

	// line 200 should give an error bc we're freeing it twice in a row!!
	char * r = (char*)malloc(100);

	free(r);
	// this should show an error!! because it's freeing the same thing twice in a row, without it being malloced
	free(r);
	
	int x = 0;
	// line 204 is an error because you can't free an address that's not a pointer
	free((int*)x);
	
	char * s = (char*)malloc(200);
	// line 208 is an error because pointer s+10 was not returned by malloc
	free(s+10);
	
	// both lines 211 and 212 will return an error because saturation of dynamic memory is reached
	char * t = (char*)malloc(5000*sizeof(char));
	int * u = (int*)malloc(1250*sizeof(int));
	// the user does not have 5000 bytes available because the meta-data for the free block of memory or md node takes up sizeof(struct md) bytes
	// the following free statement will return an error because null pointers can't be freed (note that memory was never allocated because of the errors mentioned above)
	free(t);
	free(u);
	
	gettimeofday(&t2, NULL);
	double time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	time += (t2.tv_usec - t1.tv_usec);

	return time;
}


 
int main() {
	double sumA, sumB, sumC, sumD, sumE, sumF;
	int i =0;

	while(i<100){
		sumA += A();
		sumB += B();
		sumC += C();
		sumD += D();
		sumE += E();
		sumF += F();

		i++;
	}

	printf("Time A: %f microseconds\n", sumA/100);
	printf("Time B: %f microseconds\n", sumB/100);
	printf("Time C: %f microseconds\n", sumC/100);
	printf("Time D: %f microseconds\n", sumD/100);
	printf("Time E: %f microseconds\n", sumE/100);
	printf("Time F: %f microseconds\n", sumF/100);

	return 0;
}
