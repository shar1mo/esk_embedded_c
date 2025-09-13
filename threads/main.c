#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE 1
#define STORE_COUNT 5
#define BUYER_COUNT 3

volatile int buyer_done_flag = FALSE;
int store[STORE_COUNT];
pthread_mutex_t store_mutex[STORE_COUNT];
pthread_mutex_t rand_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_random(int min, int max) {
  pthread_mutex_lock(&rand_mutex);
  int result = rand() % (max - min + 1) + min;
  pthread_mutex_unlock(&rand_mutex);
  return result;
}

void *thread_buyer(void *args) {
  int *buyer_id = (int *)args;
  printf("THREAD[%d] CREATE \n", *buyer_id);

  int buyer_need = get_random(9000, 11000);

  while(buyer_need > 0) {
    int store_index = get_random(0, STORE_COUNT - 1);
    if(pthread_mutex_trylock(&store_mutex[store_index]) == 0) {
      printf("thread[%d] | buyer need = %d | store[%d] = %d\n", *buyer_id, buyer_need, store_index, store[store_index]);
    if(store[store_index] > buyer_need) {
      store[store_index] -= buyer_need;
      buyer_need = 0;
      printf("thread[%d] filed a need | store[%d] = %d\n", *buyer_id, store_index, store[store_index]);
    } else {
      printf("thread[%d] buyer took %d stuff from store[%d]\n", *buyer_id, store[store_index], store_index);
      buyer_need -= store[store_index];
      store[store_index] = 0;
    }
      pthread_mutex_unlock(&store_mutex[store_index]);
      sleep(3);
    } else {
      usleep(1000);
      continue;
    }
  }
  return NULL;
}

void *thread_loader(void *args) {
  printf("LOADER THREAD CREATE\n");
  while(buyer_done_flag == FALSE) {
    int store_index = get_random(0, STORE_COUNT - 1);
    pthread_mutex_lock(&store_mutex[store_index]);
    store[store_index] += 1000;
    printf("loader put 1000 stuff to store[%d]\n", store_index);
    pthread_mutex_unlock(&store_mutex[store_index]);
    sleep(1);
  }
  return NULL;
}

int main(void) 
{
  srand(time(NULL));

  for(int i = 0; i < STORE_COUNT; i++) {
    pthread_mutex_init(&store_mutex[i], NULL);
  }

  for(int i = 0; i < STORE_COUNT; i++) {
    store[i] = get_random(400, 600);
    printf("STORE[%d] CREATE: %d\n", i, store[i]);
  }

  int *status;
  pthread_t buyer_tid[BUYER_COUNT];
  pthread_t loader_tid;

  int v[BUYER_COUNT];
  for(int i = 0; i < BUYER_COUNT; i++) {
    v[i] = i;
    if(pthread_create(&buyer_tid[i], NULL, thread_buyer, (void *)&v[i]) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  if(pthread_create(&loader_tid, NULL, thread_loader, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }
    
  for(int i = 0; i < BUYER_COUNT; i++) {
    pthread_join(buyer_tid[i], (void **)&status);
    printf("THREAD[%d] STOP\n", i);
  }

  buyer_done_flag = TRUE;
  pthread_join(loader_tid, (void **)&status);

  for(int i = 0; i < STORE_COUNT; i++) {
    pthread_mutex_destroy(&store_mutex[i]);
  }
  pthread_mutex_destroy(&rand_mutex);

  return EXIT_SUCCESS;
}