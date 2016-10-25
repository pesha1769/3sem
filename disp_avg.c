//* FIXIT: если у вас размер массива не делится нацело на количество нитей, то код ниже неверный.
//В предыдущей версии кода я складывал средние значения, полученные каждой нитью, 
///и результат был парвильном только когда каждая нить искала среднее одинакового количества чисел, в противном случае - нет:(
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#define ARRAY_SIZE 10000000
#define THREAD_AMOUNT 3
float number[ARRAY_SIZE];
float sum_of_segment[THREAD_AMOUNT];
float sum_square_of_segment[THREAD_AMOUNT];
struct Segment
{
	int begin;
	int end;
	int index;
};


void *mythread(void *dummy)
{
	struct Segment* segment = (struct Segment*)(dummy); 
	int i = 0;
	for (i = segment->begin; i < segment->end; i++)
	{	
	sum_of_segment[segment->index] += number[i] ;
        sum_square_of_segment[segment->index] += number[i] * number[i];      
	}
   return NULL;
}

int main(int argc, char const *argv[])
{	
	float sum = 0.0;
	float sum_square = 0.0;
	int result;
        float average;
	float average_square;
	struct Segment segments[THREAD_AMOUNT];
	pthread_t thid[THREAD_AMOUNT]; 
	int i = 0;
	for (i = 0; i < ARRAY_SIZE; i++)
	{			
		number[i] = (float)(rand() % ARRAY_SIZE) / ARRAY_SIZE;
	}	


	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		segments[i].begin = (ARRAY_SIZE / THREAD_AMOUNT) * i;
		segments[i].index = i;
	
		if (i != THREAD_AMOUNT - 1) 
		{
			segments[i].end = segments[i].begin + (ARRAY_SIZE / THREAD_AMOUNT);
		} 
		else 
		{
			segments[i].end = ARRAY_SIZE;
		}
	}

	clock_t begin = clock();

	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		result = pthread_create( &thid[i], (pthread_attr_t *)NULL, mythread, &segments[i]);
	}
	
	for (i = 0; i < THREAD_AMOUNT; i++)
		pthread_join (thid[i], (void **) NULL);
	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		sum += sum_of_segment[i];
                sum_square += sum_square_of_segment[i];
	}
	average = sum / ARRAY_SIZE;
	average_square = sum_square / ARRAY_SIZE;

	clock_t end = clock();

	printf("AVERAGE: %.2f\nDISPERSION: %.2f\nSUM: %.2f\n", average, average_square - average * average, sum);


	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("TIME TO COMPUTE IS %f\n", time_spent);

return 0;
}
