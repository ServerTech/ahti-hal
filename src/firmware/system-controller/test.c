#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PRIORITY_REALTIME 0
#define PRIORITY_HIGH     1
#define PRIORITY_NORMAL   2
#define PRIORITY_LOW      3

typedef struct Node_t Node_t;
typedef struct List_t List_t;
typedef struct Task_t Task_t;
typedef struct TaskContext_t TaskContext_t;

void initTaskList( List_t* task_list );
void createTask( List_t* task_list, void (*taskHandler)( TaskContext_t* ),
                 void* params, uint32_t priority, uint32_t period );
void printTaskList( List_t* task_list );
void dummyHandler( TaskContext_t* context );

typedef struct List_t
{
  Node_t* head;
  Node_t* tail;
} List_t;

typedef struct Node_t
{
  Task_t* task;
  Node_t* next;
  Node_t* prev;
} Node_t;

typedef struct Task_t
{
  void (*taskHandler)( TaskContext_t* );
  // void (*task_callback)( TaskContext_t* );

  TaskContext_t* context;

  bool periodic;
  uint32_t period;

  uint32_t priority;
} Task_t;

typedef struct TaskContext_t
{
  void* params;

  uint32_t issue_time;
  uint32_t start_time;
  uint32_t end_time;
} TaskContext_t;

void initTaskList( List_t* task_list )
{
  task_list = (List_t*) malloc( sizeof(List_t) );
  memset( task_list, 0, sizeof(List_t) );
  task_list->head = NULL;
  task_list->tail = NULL;
}

void createTask( List_t* task_list, void (*taskHandler)( TaskContext_t* ),
                 void* params, uint32_t priority, uint32_t period )
{
  Task_t*        new_task    = (Task_t*)        malloc( sizeof(Task_t) );
  TaskContext_t* new_context = (TaskContext_t*) malloc( sizeof(TaskContext_t) );
  Node_t*        new_node    = (Node_t*)        malloc( sizeof(Node_t) );
  memset( new_task,    0, sizeof(Task_t) );
  memset( new_context, 0, sizeof(TaskContext_t) );
  memset( new_node,    0, sizeof(Node_t) );

  new_task->taskHandler = taskHandler;
  new_task->priority = priority;
  if ( period )
  {
    new_task->period = period;
    new_task->periodic = true;
  }
  else
  { 
    new_task->period = 0;
    new_task->periodic = false;
  }

  new_task->context             = new_context;
  new_task->context->params     = params;
  new_task->context->issue_time = 10; // dummy

  new_node->task = new_task;

  if ( task_list->head == NULL )
  {
    //new_node->next = NULL;
    //new_node->prev = NULL;
    //task_list->head = new_node;
    //task_list->tail = new_node;
  }
/*
  else
  {
    Node_t* traverse = task_list->head;
    while ( traverse->next != NULL &&
            traverse->next->task->priority <= new_task->priority )
    {
      traverse = traverse->next;
    }

    if ( traverse->next == NULL )
    {
      traverse->next = new_node;
      new_node->next = NULL;
      new_node->prev = traverse;
      task_list->tail = new_node;
    }
    else
    {
      new_node->next = traverse->next;
      new_node->prev = traverse;
      traverse->next->prev = new_node;
      traverse->next = new_node;
    }
  }*/
}

void printTaskList( List_t* task_list )
{
  Node_t* traverse = task_list->head;
  int i = 0;

  while ( traverse != NULL )
  {
    printf( "Queue number: %d\n", i++ );
    traverse = traverse->next;
  }
}

void dummyHandler( TaskContext_t* context )
{
  printf( "Dummy handler called with context: %s\n", (char*) (context->params) );
}

int main(void)
{
  List_t* task_list;

  initTaskList( task_list );

  char post[20] = "Prateek died";
  createTask( task_list, dummyHandler, (void*) post, PRIORITY_REALTIME, 0 );
  //printTaskList( task_list );
}
