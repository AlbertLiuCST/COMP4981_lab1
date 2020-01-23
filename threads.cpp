#define HAVE_STRUCT_TIMESPEC
#include <time.h>
#include <string>
#include <iostream>
#include <pthread.h>
using namespace std;

//------------ Mutex Variables ---------------------
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function Prototypes

void* producer(void*);
void* consumer(void*);

typedef struct {
	string str;
	int i;
	double d;
	bool update;
	bool done;
}ThreadInfo;

int main (int argc, char **argv) 
{
  pthread_t thread1, thread2;
  ThreadInfo info_ptr;
  info_ptr.str = "Default";
  info_ptr.i = 1;
  info_ptr.d = 10.5;
  info_ptr.update = false;
  info_ptr.done = false;


  // Create and run the threads.
  pthread_create (&thread1, NULL, producer, static_cast<void*>(&info_ptr));
  pthread_create (&thread2, NULL, consumer, static_cast<void*>(&info_ptr));

  // And wait until they are both done
  pthread_join (thread1, NULL);
  pthread_join (thread2, NULL);  
  
  return 0;
}

void* producer(void* arg) {

	ThreadInfo* info = (ThreadInfo*)arg;

	char a = 'a';
	int i = 3;
	double d = 10.5;
	for (;;)
	{
		if (info->update) {
			pthread_mutex_lock(&lock);
			info->str = a++;
			info->i = i++;
			info->d = d++;
			info->update = true;

			if (i == 6)
				info->done = true;
			pthread_mutex_unlock(&lock);
		}
		if (info->done)
			break;
	}
	return NULL;
}
void* consumer(void* arg) {
	ThreadInfo* info = (ThreadInfo*)arg;

	for (;;)
	{
		if (info->update) {
			pthread_mutex_lock(&lock);
			cout << "String: " << info->str << " Int: " << info->i << "Double" << info->d;
			pthread_mutex_unlock(&lock);
		}
		if (info->done)
			break;

	}
	return NULL;
}


