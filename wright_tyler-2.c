#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct node  
{
   int val;
   struct node *next;
    struct node *last;
};

struct node *head = NULL;
struct node *tail = NULL;

int generateOdd()
{
	int num; 
	do{
		num = rand() % 41;
	} while(num == 0 || num % 2 == 0); 
	return num; 
}

int generateEven()
{
	int num; 
	do{
		num = rand() % 41;
	} while(num == 0 || num % 2 != 0);
	return num; 
}

void addEvenToEnd() 
{
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->val = generateEven(); 
	newNode->next = NULL; 
	
	if(head == NULL)
	{
        newNode->last = NULL;
		head = newNode; 
		tail = newNode; 
	}
	else
	{
		tail->next = newNode;
        newNode->last = tail;
		tail = newNode; 
	}
}

void addOddToEnd()
{
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->val = generateOdd(); 
	newNode->next = NULL; 
	
	if(head == NULL)
	{
        newNode->last = NULL;
		head = newNode; 
		tail = newNode; 
	}
	else
	{
		tail->next = newNode;
        newNode->last = tail;
		tail = newNode; 
	}
}

void deleteNode(int val) {
    
    //start from the first link
    struct node* current = head;
    struct node* previous = NULL;
    
    //if list is empty
    if(head == NULL) {
    }
    
    //navigate through list
    while(current->val != val) {
        
        //if it is last node
        if(current->next == NULL) {
        } else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }
    
    //found a match, update the link
    if(current == head) {
        //change first to point to next link
        head = head->next;
    } else {
        //bypass the current link
        previous->next = current->next;
    }
}

void deleteEven()
{
    int done = 0;
    struct node *temp = head;
    while(done == 0)
    {
        if(temp->val % 2 == 0)
        {
            deleteNode(temp->val);
            done = 1;
        } else
        {
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

void deleteOdd()
{
    int done = 0;
    struct node *temp = head;
    while(done == 0)
    {
        if(temp->val % 2 != 0)
        {
            deleteNode(temp->val);
            done = 1;
        } else
        {
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

void printList() {
    struct node *ptr = head;
    printf("\n[ ");
    while(ptr != NULL) {
        printf("(%d) ",ptr->val);
        ptr = ptr->next;
    }
    
    printf(" ]\n");
}

void *threadAddEven(void *param)
{
	head = param; 
	
	while(size() < 20)
	{
		printf("Producer2");
		addEvenToEnd(); 
		printList(); 
		sleep(2); 
	}	
	return NULL; 
}

void *threadAddOdd(void *param)
{
	head = param; 
	
	while(size() < 20)
	{
		printf("Producer1");
		addOddToEnd(); 
		printList(); 
		sleep(2); 
	}	
	return NULL; 
}


int main()
{
    addEvenToEnd();
    addOddToEnd();
    addEvenToEnd();
	printf("Initial List");
    printList();
	
	pthread_t thread0;
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_create(&thread0, NULL, threadAddOdd, head);
	sleep(1); 
	pthread_create(&thread1, NULL, threadAddEven, head);
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL); 

	return 0; 
}