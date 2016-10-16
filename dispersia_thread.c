#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

/*
 * array ... not "massiv"
 */
#define MASS_SIZE 10000000
#define THREAD_AMOUNT 4
#define MOD 1000000

/*
 * лучше минимизировать использование глобальных переменных
 */
float number[MASS_SIZE];
float average[THREAD_AMOUNT];

/*
 * dispersion
 */
float dispersia[THREAD_AMOUNT];
struct Segment
{
	int begin;
	int end;
	int index;
};

/*
 * замените уже наконец символы табуляции на пробелы в настройках текстового редактора.
 * т.к. они местами смешаны, то форматирование на других компьютерах выглядит съехавшим.
 */

void *mythread(void *dummy)
{
	float avg = 0.0;
	float disp = 0.0;
   	struct Segment* segment = (struct Segment*)(dummy); 
	int i = 0;
	float sum = 0;
	for (i = segment->begin; i < segment->end; i++)
	{	
		sum = sum + number[i];
	}
	avg = sum / (segment->end - segment->begin);

	for (i = segment->begin; i < segment->end; i++)
	{	
		disp = disp + (number[i] - avg) * (number[i] - avg);				
	}
	disp = sqrt(disp / (segment->end - segment->begin));

	average[segment->index] = avg;
	dispersia[segment->index] = disp;

   return NULL;
}

int main(int argc, char const *argv[])
{	
	float avg = 0.0;
	float disp = 0.0;
	int result;
	struct Segment segments[THREAD_AMOUNT];
	pthread_t thid[THREAD_AMOUNT]; 

	int i = 0;
  /*
   * Почему вы только первую половину чисел инициализируете?
   */
	for (i = 0; i < MASS_SIZE / 2; i++)
	{	
		number[i] = (float)(rand() % MOD) / MOD;
	}	

	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
    /*
     * FIXIT: если у вас размер массива не делится нацело на количество нитей, то код ниже неверный.
     */
		segments[i].begin = (MASS_SIZE / THREAD_AMOUNT) * i;
		segments[i].end = (MASS_SIZE / THREAD_AMOUNT) * (i + 1);
		segments[i].index = i;
	}

	clock_t begin = clock();

	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		result = pthread_create( &thid[i], (pthread_attr_t *)NULL, mythread, &segments[i]);
	}
	
	for (i = 0; i < THREAD_AMOUNT; ++i)
		pthread_join (thid[i], (void **) NULL);

	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		avg += average[i];
	}
	avg = avg / THREAD_AMOUNT;
	
	for (i = 0; i < THREAD_AMOUNT ; i++)
	{
		disp += (dispersia[i]) * (dispersia[i]);
	}
	disp = sqrt(disp / THREAD_AMOUNT);

	clock_t end = clock();

	printf("AVERAGE: %f\nDISPERSION: %f\n", avg, disp);
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("TIME TO COMPUTE IS %f\n", time_spent);

return 0;
}
