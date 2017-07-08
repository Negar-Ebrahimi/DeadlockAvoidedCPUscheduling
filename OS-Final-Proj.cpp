#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;


pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond_var=PTHREAD_COND_INITIALIZER;

// int time=0;
// int quantum=2; // It can varry by the OS based on the situation-including average process length and ...
int heldRes1[3]; //each process has a held a number of resource.

const int totalRes1=12;
int availableRes1=3;
int flagOfdeadlock=0;
int deadlockAvoidance=0;

class process{
public:
	bool terminated=0;
	int pid;
	int remainedLength;
	int maxNeedRes1;
	int currentlyAllocatedRes1;

process(int indx, int maxn1, int initiallyAllocated1=0 ){
	pid=indx;
	//remainedLength=length;
	maxNeedRes1=maxn1;
	heldRes1[indx]=maxn1;
	currentlyAllocatedRes1=initiallyAllocated1;
	cout<<"Process "<<pid<<" with a maximum needed resource 1 instances of "<<maxNeedRes1<<" and initially allocated resource 1 instances of "<<currentlyAllocatedRes1<<" has been created."<<endl;
}

void running(){
cout<<"available resources are:  "<<availableRes1<<endl;
	if(currentlyAllocatedRes1+availableRes1>=maxNeedRes1){
		pthread_mutex_lock(&mutex);
		availableRes1-=maxNeedRes1-currentlyAllocatedRes1;// allocated needed instances
		// remainedLength-=quantum;
		// time+=quantum;
		availableRes1+=maxNeedRes1; // release all allocated instances
		terminated=1;
		cout<<"Process "<<pid<<" terminated."<<endl;
		pthread_mutex_unlock(&mutex);
	}
	else{
		int sumOfCurrentlyAllocatedInstances=0;
		for(int i=0; i<pid; i++){
			sumOfCurrentlyAllocatedInstances+=heldRes1[i];
		}
		if(currentlyAllocatedRes1+sumOfCurrentlyAllocatedInstances>=maxNeedRes1 && flagOfdeadlock==0){ // It should wait for previouse processes to terminate and release the instances
		cout<<"Process "<<pid<<" will be running soon"<<endl;
		flagOfdeadlock=1;
		}
		else if(currentlyAllocatedRes1+sumOfCurrentlyAllocatedInstances>=maxNeedRes1 && flagOfdeadlock==1){
		    deadlockAvoidance=1;
		    }
		else
			;//cout<<"A deadlock will occure! Therefor process "<<pid<<" request for resource allocation is denied."<<endl; // and there it gets out of the running function with no change
	}
	
}




}; 


int main(){

	pthread_mutex_init(&mutex,NULL);
	//pthread_cond_init(&cond_var,NULL);

	process *p0=new process(0, 10, 5);
	process *p1=new process(1, 4, 2);
	process *p2=new process(2, 9, 2);

	while((!p0->terminated || !p1->terminated || !p2->terminated) && !deadlockAvoidance){
		if(!p0->terminated){
			p0->running();
		}
		if(!p1->terminated){
			p1->running();
		}
		if(!p2->terminated){
			p2->running();
		}
	}
	if(deadlockAvoidance==1){
	    cout<<"An unsafe state occured."<<endl;
	    }

	return 0;
}




