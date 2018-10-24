/**
   Georgia Ma 
   V00849447
   CSC 360 
   Assignment 2
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include <readline/readline.h>
#include <readline/history.h>

#define TIME_CONVERSION 100000
#define MAX_FILE 1024
#define MAX_INPUT 256

// customer struct to keep track of each customer in the queue
typedef struct customer {
	int id;
	float arrival_time;
	float service_time;
	int class;	// 0 for Economy, 1 for Business
	struct customer* next;
} customer;


/** Global Variables **/

// queue heads for customer lines, business is the higher priority
struct customer* business_queue; 
struct customer* economy_queue; 

// queue heads for time waited
double business_time[MAX_INPUT];
double economy_time[MAX_INPUT];

// threads, mutex, and convar
pthread_t clerks[4];
pthread_t customers[MAX_INPUT];
pthread_mutex_t mutex;
pthread_cond_t convar;

// status variables
int serving = -1; 	// for telling which clerk is serving a queue
int busy = 0;		// for telling when a queue is busy being served by a clerk
int served = 0;		// for telling when a customer has left a queue and is being served

int total = 0; 		// total customers


/** Queue Functions **/

// inserts a customer into a queue
void enqueue(int id, float arrival_time, float service_time, int class) {
	struct customer* queue_head;

	if (class == 0) {
		queue_head = economy_queue;
	} else if (class == 1) {
		queue_head = business_queue;
	} else {
		fprintf(stderr, "Error: invalid class.\n");
		exit(1);
	}

	if (!queue_head) {
		queue_head = (struct customer*)malloc(sizeof(struct customer));
		
		queue_head->id = id;
		queue_head->arrival_time = arrival_time;
		queue_head->service_time = service_time;
		queue_head->class = class;
		queue_head->next = NULL;
				
	} else {
		struct customer *curr = queue_head;
		while (curr->next != NULL) {
			curr = curr->next;
		}

		curr->next = (struct customer*)malloc(sizeof(struct customer));
		curr->next->id = id;
		curr->next->arrival_time = arrival_time;
		curr->next->service_time = service_time;
		curr->next->class = class;
		curr->next->next = NULL;
		
	}
}

// deletes a customer from the start of a queue
void dequeue(int class) {
	struct customer* queue_head;

	if (class == 0) {
		queue_head = economy_queue;
	} else if (class == 1) {
		queue_head = business_queue;
	} else {
		fprintf(stderr, "Error: invalid class.\n");
		exit(1);
	}
	
	if (queue_head) {
		queue_head = queue_head->next;
	}

	return;
}


/** User Input Parsing Function **/

// reads input file and puts information into the global queues
void set_up_customers(char* to_read) {
	FILE* input = fopen(to_read, "r");
	if (!input)	{
		printf("Error: could not open input file.");
		exit(1);
	}
	char buffer[MAX_FILE];
	char contents[MAX_FILE][MAX_FILE];
	int i = 0;
	while (fgets(buffer, MAX_FILE-1, input)) {
		strncpy(contents[i], buffer, strlen(buffer)+1);
		i++;
	}
	fclose(input);	
	
	// parse first line for total number of customers
	total = atoi(contents[0]);
	
	int j;
	for (j = 1; j < total+1; j++) {
		struct customer temp = (struct customer)malloc(sizeof(struct customer));
		
		char* token = strtok(contents[j], ":");
		temp->id = atoi(token);
		
		token = strtok(NULL, ",");
		temp->class = atoi(token);
		
		token = strtok(NULL, ",");
		temp->arrival_time = atoi(token);
		
		token = strtok(NULL, ",");
		temp->service_time = atoi(token);
		
		printf("%d %d %d %d\n",temp->id, temp->class, stemp->arrival_time, temp->service_time);

		
		enqueue(temp->id, temp->arrival_time, temp->service_time, temp->class);
		free(temp);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Error: please include an input file name.\n");
		exit(1);
	}
	
	set_up_customers(argv[1]);
	
	
	
	exit (0);
}