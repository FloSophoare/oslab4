#include "lib.h"
#include "types.h"
#define N 5
#define N 5

void philosopher(int i){
	while (1){
		if (i % 2 == 0){
			printf("Philosopher %d thinking\n", i);
			sem_wait(i);
			sem_wait((i+1) % N);
			sleep(128);
			printf("Philosopher %d eating\n", i);
			sem_post(i);
			sem(post((i+1) % N));
		}
		else {
			printf("Philosopher %d thinking\n", i);
			sem_wait(i);
			sem_wait((i+1) % N);
			sleep(128);
			printf("Philosopher %d eating\n", i);
			sem_post(i);
			sem(post((i+1) % N));
		}
	}
}


int uEntry(void) {

	// 测试scanf	
	/*int dec = 0;
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
	}*/
	
	// 测试信号量
	int i = 4;
	sem_t sem;
	int ret = 0;
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
		exit();
	}

	// For lab4.3
	// TODO: You need to design and test the philosopher problem.
	// Note that you can create your own functions.
	// Requirements are demonstrated in the guide.
	
	//哲学家
	sem_t chopstick[N];
	for (int i = 0; i < N; i++){
		sem_init(&chopstick[i], 1); // set initial value be 1
	}
	int i = 0;
	while (i < 5){ // create 5 child process
		if (fork() == 0){ // child process
			philosopher(i);
		}
		else i++;
	}

	//生产者消费者问题

	//读者写者问题
	

	exit(0);
	return 0;
}
