#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int a[100000]={0};
int n=0;

typedef struct node {
    int start;
    int end;
} NODE;

void merge(int low, int high)
{
    int i=0;
    int j=0;
    int k=0;
    int mid = (low+high)/2;
    int left = mid - low + 1;
    int right =  high - mid;
 
    int t1[100000], t2[100000];
 
    for (i = 0; i < left; i++)
        t1[i] = a[low + i];
    for (j = 0; j < right; j++)
        t2[j] = a[mid + 1+ j];
 
    i = 0; 
    j = 0;
    k = low;
    while (i < left && j < right)
    {
        if (t1[i] <= t2[j])a[k++] = t1[i++];
        else a[k++] = t2[j++];
    }
 
    while (i < left)a[k++] = t1[i++]; 
    while (j < right)a[k++] = t2[j++];
}
void * mergesort(void *a)
{
	pthread_t tid1, tid2;
        NODE *p = (NODE *)a;
        NODE node1, node2;
        int low = p->start;
        int high = p->end;
        int mid = (low+high)/2;
        int ret;
        node1.start = p->start;
        node2.start = mid+1;
        node1.end = mid;
        node2.end = p->end;

        if (low >= high) return;


        ret = pthread_create(&tid2, NULL, mergesort, &node2);
        if(ret){printf("Error\n");exit(1);}

        
        ret = pthread_create(&tid1, NULL, mergesort, &node1);
        if(ret){printf("Error\n");exit(1);}


        pthread_join(tid2, NULL);pthread_join(tid1, NULL);
        
        merge(low, high);
        pthread_exit(NULL);
}


int main()
{
        int i;
        scanf("%d",&n);
        for(i=0;i<n;i++) scanf("%d",&a[i]);
        NODE m;
        m.start = 0;
        pthread_t tid;
        m.end = n-1;

        int ret; 
        ret=pthread_create(&tid, NULL, mergesort, &m);
        if(ret){printf("Error\n");exit(1);}
        pthread_join(tid, NULL);
        for (i = 0; i < n; i++)
            printf ("%d\n", a[i]);
        return 0;
}
