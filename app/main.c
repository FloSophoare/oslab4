#include "lib.h"
#include "types.h"
#define N 5

//#define testScanf
//#define testSemaphore
//#define testPhilosopher
//#define testReaderAndWriter
#define testProducerAndConsumer



#ifdef testPhilosopher
sem_t chopstick[N];

void philosopher(int i){
	while (1){
		if (i % 2 == 1){
			printf("Philosopher  %d  thinking\n", i);
			sleep(128);
			sem_wait(&chopstick[i]);
			sem_wait(&chopstick[(i+1) % N]);
			printf("Philosopher  %d  eating\n", i);
			sleep(128);
			sem_post(&chopstick[i]);
			sem_post(&chopstick[(i+1) % N]);
		}
		else {
			printf("Philosopher  %d  thinking\n", i);
			sleep(128);
			sem_wait(&chopstick[(i+1) % N]);
			sem_wait(&chopstick[i]);
			printf("Philosopher  %d  eating\n", i);
			sleep(128);
			sem_post(&chopstick[(i+1) % N]);
			sem_post(&chopstick[i]);
		}
	}
}
#endif


#ifdef testReaderAndWriter

sem_t rw_mutex;  //semaphore for reader and writer
sem_t mutex;  // semaphore for read_count
int read_count = 0;

void reader(){
	pid_t pid = getpid();
	while (1){
		sem_wait(&mutex);
		sleep(128);
		read_count++;
		if (read_count == 1){
			sleep(128);
            sem_wait(&rw_mutex);
		}
		sleep(128);
		sem_post(&mutex);
		sleep(128);
		printf("Reader %d read,  total %d reader\n", pid, read_count);
		sleep(128);
		sem_wait(&mutex);
		sleep(128);
		read_count--;
		if (read_count == 0){
            sleep(128);
			sem_post(&rw_mutex);
			//sleep(128);
		}
		sleep(128);
		sem_post(&mutex);
		sleep(128);

	}
}

void writer(){
	pid_t pid = getpid();
	while (1){
		sem_wait(&rw_mutex);
		sleep(128);
		printf("Writer %d write\n", pid);
		sleep(128);
		sem_post(&rw_mutex);
		sleep(128);
	}
}


#endif


#ifdef testProducerAndConsumer

sem_t full;
sem_t empty;
sem_t mutex;


void producer(){
	pid_t pid = getpid();
	while (1){
		sem_wait(&empty);
        sleep(128);
		sem_wait(&mutex);
        sleep(128);
		printf("producer %d produce\n", pid);
		sleep(128);
        sem_post(&mutex);
        sleep(128);
		sem_post(&full);
        sleep(128);
	}
}


void consumer(){
	pid_t pid  = getpid();
	while(1){
		sem_wait(&full);
        sleep(128);
		sem_wait(&mutex);
        sleep(128);
		printf("Consumer %d consume\n", pid);
        sleep(128);
		sem_post(&mutex);
        sleep(128);
		sem_post(&empty);
        sleep(128);
	}
}
#endif


int uEntry(void) {

	// 测试scanf
#ifdef testScanf
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while(1){
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf("Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}
#endif
	// 测试信号量
#ifdef testSemaphore
	printf("There is my semaphore test below! \n");
	int i = 4;
	sem_t sem;
#ifndef testScanf
    int ret = 0;
#else
    ret = 0;
#endif
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 0);
	if (ret == -1) {
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0) { //child process
		while( i != 0) {
			i --;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1) {
		while( i != 0) {
			i --;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		//exit();
	}

#endif
	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	
	//哲学家
#ifdef testPhilosopher
	printf("There is my philosopher test below!\n");
	for (int i = 0; i < N; i++){
		sem_init(&chopstick[i], 1); // set initial value be 1
	}
	int philosopherNum = 0;
	while (philosopherNum < 6){ // create 5 child process
		if (fork() == 0){ // child process
			philosopher(philosopherNum);
		}
		else philosopherNum++;
	}
#endif
	//生产者消费者问题

#ifdef testProducerAndConsumer
    printf("There is my producer and consumer test below!\n");
	sem_init(&mutex, 1);
	sem_init(&full, 0);
	sem_init(&empty, 5);
	int i = 0; 
    int order[] = {2, 0, 4, 1, 3};
	while (i < 5){
		if (fork() == 0 ){
			if (order[i] < 4) {
                sleep(10);
                producer();
                //sleep(10);
            }
			else {
                sleep(10);
                consumer();
                //sleep(10);
            }
		}
		else i++;
	}
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);


#endif

	//读者写者问题

#ifdef testReaderAndWriter
    sem_init(&mutex, 1);
	sem_init(&rw_mutex, 1);
    int order[] = {0, 3, 2, 4, 5, 1};
	for (int i = 0; i < 6; i++){
		if (fork() == 0){
			if (order[i] < 3) reader();
			else writer();
		}
	}
	sem_destroy(&mutex);
	sem_destroy(&rw_mutex);
#endif
	

	exit(0);
	return 0;
}
