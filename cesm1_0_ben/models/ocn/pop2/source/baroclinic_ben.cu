#include <stdio.h>

#include "timer.h"


createTimer(timer);

extern "C"
{
void print_ben(int n);
void ben_init(int id);
void ben_timer_start();
void ben_timer_end(char* name);
}

int rank;

__global__ void doNothing(float *nothing) {
	int tx = threadIdx.x;

	nothing += tx;
	
	*nothing = tx;
}


void print_ben(int n) {

	printf("PRINT_BEN IN C CALLED FROM FORTRAN! N=%d\n",n);
	
	float* d_nothing = 0;
	
	cudaMalloc((void**) &d_nothing, 256*sizeof(float));
	
	dim3 threads(256, 1);
	dim3 grid(1,1);
	
	doNothing<<<grid, threads>>>(d_nothing);
	
	cudaDeviceSynchronize();

	cudaError_t err = cudaGetLastError();
	printf("CUDA STATUS = %s\n", cudaGetErrorString(err));
	
}


void ben_init(int id) {
	
	rank = id;

	
}


void ben_timer_start() {
	//if (rank == 0) {
		timer.reset();
		timer.start();
	//}
}



void ben_timer_end(char *name) {
	//if (rank == 0) {
		timer.stop();
		float ktime = timer.getTimeInMilliSeconds();
		
		printf("BEN Process %d: timer %s took %f ms\n",rank,name,ktime);
		timer.reset();
	//}
}
