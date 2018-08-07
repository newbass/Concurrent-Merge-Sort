#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>


// selection sort
void ssort(int arr[], int n)
{
	int i=0,j=0;
	for (i = 0; i < n-1; i++)
	{
		int midx = i;
		for (j = i+1; j < n; j++)
			if (arr[j] < arr[midx])
				midx = j;

		int temp = arr[i];
		arr[i] = arr[midx];
		arr[midx] = temp;
	}
	return;
}

// merge sorted arrays
void merge(int a[],int low,int mid,int high)
{
	int temp[count];
	int count=high-low+1;
	int i=low;
	int j=mid+1;
	int m=0;

	while(i<=mid && j<=high)
	{
		if (a[i]<a[j])temp[m++]=a[i++];
		else if (a[j]<=a[i])temp[m++]=a[j++];
	}

	while(i<=mid)temp[m++]=a[i++];

	while(j<=high)temp[m++]=a[j++];

	for(i=0;i<count;i++,low++)
		a[low] = temp[i];
}

void msort(int a[],int low,int high)
{
	int i=0;
	int len=(high-low+1);
	int mid=low+len/2-1;

	// Using selection sort for small sized array
	if (len<=5){ssort(a+low,len);return;}

	pid_t left=fork();
	if (left<0){printf("Error\n");_exit(-1);}

	else if (left==0){msort(a,low,mid);_exit(0);}

	else
	{
		pid_t right;
		right=fork();
		if(right<0){printf("Error\n");_exit(-1);}

		else if(right==0){msort(a,mid+1,high);_exit(0);}
	}

	int status;

	// Waiting for the processes to complete
	waitpid(right,&status,0);
	waitpid(left,&status,0);


	merge(a,low,mid,high);
}

int main()
{
	int shmid;
	int *shm_array;
	int length = 0;
	key_t key = IPC_PRIVATE;

	printf("No of elements:");
	scanf("%d",&length); 

	// This is the length of the shared memory array.
	size_t SHM_SIZE = sizeof(int)*length;

	// After creating memory, we will now create space in the memory.
	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0){printf("error in shmget\n");_exit(1);}

	// Now we will make the shared array.
	if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1){printf("error in shmat\n");_exit(1);}

	// Input array
	int i=0;
	for (i=0; i<length; i++)
	{
		scanf("%d",&shm_array[i]);
	}

	// Call merge sort function
	msort(shm_array, 0, length-1);

	for (i=0; i<length; i++)
		printf("%d\n",shm_array[i]);

	if (shmdt(shm_array) == -1){perror("error in shmdt");_exit(1);}

	// Delete the shared memory segment.
	if (shmctl(shmid, IPC_RMID, NULL) == -1){perror("error in shmctl");_exit(1);}

	return 0;
}
