/*
 * Tyler Wright
 * October 24, 2017
 *
 * Program uses pthread generate producer/consumer threads.
 * Threads concurrently interact with list to add and delete nodes. 
 *
 * compiled with: gcc wright_tyler-2.c -o wright_tyler-2 -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Node for linked list
struct node  
{
   int val;
   struct node *next;
    struct node *last;
};

struct node *head = NULL;
struct node *tail = NULL;

//generate odd int
int generateOdd()
{
	int num; 
	do{
		num = rand() % 41;
	} while(num == 0 || num % 2 == 0); 
	return num; 
}

//generate even int
int generateEven()
{
	int num; 
	do{
		num = rand() % 41;
	} while(num == 0 || num % 2 != 0);
	return num; 
}

//add even to end of list
void addEvenToEnd() 
{
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->val = generateEven(); 
	newNode->next = NULL; 
	//list empty, set head and tail to new
	if(head == NULL)
	{
        newNode->last = NULL;
		head = newNode; 
		tail = newNode; 
	}
	else
	{
        //add to end of existing
		tail->next = newNode;
        newNode->last = tail;
		tail = newNode; 
	}
}

//add odd to end of list
void addOddToEnd()
{
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->val = generateOdd(); 
	newNode->next = NULL; 
	//list empty, set head and tail to new
	if(head == NULL)
	{
        newNode->last = NULL;
		head = newNode; 
		tail = newNode; 
	}
	else
	{
        //add to end of existing list
		tail->next = newNode;
        newNode->last = tail;
		tail = newNode; 
	}
}

//delete node with given val
void deleteNode(int val) {
    struct node* current = head;
    struct node* previous = NULL;
    
    while(current->val != val) {
        //remove if match found
        if(current->next == NULL) {
        } else {
            previous = current;
            current = current->next;
        }
    }
    if(current == head) {
        head = head->next;
    } else {
        previous->next = current->next;
    }
}

//delete first even found from front of list
void deleteEven()
{
    int done = 0;
    struct node *temp = head;
    while(done == 0)
    {
        //even node found
        if(temp->val % 2 == 0)
        {
            deleteNode(temp->val);
            done = 1;
        } else
        {
            //move to next if not at end
            if(temp->next != NULL)
            {
                temp = temp->next;
            }
            else
            {
                done = 1;
            }
        }
    }
}

//delete first odd from front of list
void deleteOdd()
{
    int done = 0;
    struct node *temp = head;
    while(done == 0)
    {
        //odd node found
        if(temp->val % 2 != 0)
        {
            deleteNode(temp->val);
            done = 1;
        } else
        {
            //move to next if not at end
            if(temp->next != NULL)
            {
                temp = temp->next;
            }
            else
            {
                done = 1;
            }
        }
    }
}

//return size of list
int size()
{
	int count = 0; 
	struct node *current = head;
	while(current != NULL)
	{
		current = current->next; 
		count++; 
	}
	return count; 
}

//print contents of list
void printList() {
    struct node *ptr = head;
    printf("\n[ ");
    while(ptr != NULL) {
        printf("(%d) ",ptr->val);
        ptr = ptr->next;
    }
    
    printf(" ]\n");
}

//add even method to pass to thread
void *threadAddEven(void *param)
{
	head = param; 
	
	while(size() < 20)
	{
		printf("Producer2 before access");
        printList();
		addEvenToEnd();
        printf("Producer2 after access");
		printList(); 
		sleep(2); 
	}
    printf("Producer2 buffer full\n");
	return NULL; 
}

//add odd method to pass to thread
void *threadAddOdd(void *param)
{
	head = param; 
	
	while(size() < 20)
	{
		printf("Producer1 before access");
        printList();
		addOddToEnd();
        printf("Producer1 after access");
		printList(); 
		sleep(2); 
	}	
	printf("Producer1 buffer full\n");
	return NULL; 
}

//delete odd method to pass to thread
void *threadDeleteOdd(void *param)
{
	head = param; 
	while(head != NULL)
	{
		printf("Consumer1 before access");
        printList();
		deleteOdd();
        printf("Consumer1 after access");
		printList(); 
		sleep(2); 
	}
    printf("Consumer1 buffer empty\n");
	return NULL; 
}

//delete even method to add to thread
void *threadDeleteEven(void* param)
{
	head = param; 
	while(head != NULL)
	{
		printf("Consumer2 before access");
        printList();
		deleteEven();
        printf("Consumer2 after access");
		printList(); 
		sleep(2); 
	}
    printf("Consumer2 buffer empty\n");
	return NULL;
}


int main()
{
    //add initial 3 nodes to list
    addEvenToEnd();
    addOddToEnd();
    addEvenToEnd();
	printf("Initial List");
    printList();
	//create threads
	pthread_t thread0;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_create(&thread0, NULL, threadAddOdd, head);
	sleep(1);
	pthread_create(&thread1, NULL, threadAddEven, head);
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    
	pthread_create(&thread2, NULL, threadDeleteOdd, head);
	sleep(1); 
	pthread_create(&thread3, NULL, threadDeleteEven, head);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	return 0; 
}