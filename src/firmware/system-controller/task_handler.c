#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "task_handler.h"

uint32_t system_time = 0;

void initTaskList( List_t* task_list )
{
  task_list = (List_t*) malloc( sizeof(List_t) );
  memset( task_list, 0, sizeof(List_t) );
  task_list->head = NULL;
  task_list->tail = NULL;
}

void createTask( List_t* task_list, void (*taskHandler)( TaskContext_t* ),
                 const void* params, uint32_t params_size,
                 uint32_t priority, uint32_t deadline, uint32_t period )
{
  Task_t* new_task           = (Task_t*)        malloc( sizeof(Task_t) );
  TaskContext_t* new_context = (TaskContext_t*) malloc( sizeof(TaskContext_t) );
  memset( new_task,    0, sizeof( Task_t ) );
  memset( new_context, 0, sizeof(TaskContext_t) );

  new_task->taskHandler = taskHandler;
  new_task->priority    = priority;
  if ( period )
  {
    new_task->period   = period;
    new_task->periodic = true;
  }
  else
  {
    new_task->period   = 0;
    new_task->periodic = false;
  }

  new_task->context             = new_context;
  new_task->context->params     = malloc( params_size );
  memcpy( new_task->context->params, params, params_size );

  new_task->context->issue_time = system_time;
  new_task->context->start_time = 0;
  new_task->context->end_time   = 0;
  if ( deadline )
  {
    new_task->context->deadline     = deadline;
    new_task->context->has_deadline = true;
  }
  else
  {
    new_task->context->deadline     = 0;
    new_task->context->has_deadline = false;
  }

  createTaskExisting( task_list, new_task );
}

void createTaskExisting( List_t* task_list, Task_t* new_task )
{
  Node_t* new_node = (Node_t*) malloc( sizeof(Node_t) );
  memset( new_node,    0, sizeof(Node_t) );

  new_node->task = new_task;

  if ( task_list->head == NULL )
  {
    new_node->next = NULL;
    new_node->prev = NULL;
    task_list->head = new_node;
    task_list->tail = new_node;
  }
  else if ( new_task->priority == PRIORITY_NOW )
  {
    new_node->next = task_list->head;
    task_list->head->prev = new_node;
    task_list->head = new_node;
  }
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
  }
}

void printTaskList( List_t* task_list )
{
  Node_t* traverse = task_list->head;
  int i = 0;

  while ( traverse != NULL )
  {
    printf( "Queue number: %d\n", i++ );
    printf( "Params:       %s\n", (char*) traverse->task->context->params );
    traverse = traverse->next;
  }
}

Task_t* nextOnDeadline( List_t* task_list )
{
  Task_t* task;
  Node_t* priority_node = NULL;
  Node_t* node = task_list->head;

  while ( node != NULL )
  {
    task = node->task;
    if ( task->context->has_deadline )
    {
      if ( priority_node == NULL )
      {
        priority_node = node;
      }
      else
      {
        TaskContext_t* pcontext = priority_node->task->context;
        TaskContext_t* ncontext = node->task->context;

        /* may have to account for system_time overflow */
        if ( (ncontext->deadline - system_time - ncontext->issue_time) <
             (pcontext->deadline - system_time - pcontext->issue_time) )
        {
          /* if the time to deadline of new_node is less than that of priority_node */
          priority_node = node;
        }
      }
    }

    node = node->next;
  }

  return priority_node->task;
}

void doTask( Task_t* task )
{
  task->taskHandler( task->context );
}

void beginScheduler( List_t* task_list )
{
  Task_t* task;
  Node_t* node;
  while ( true )
  {
    if ( task_list->head != NULL )
    {
      task = task_list->head->task;
      node = task_list->head;
      task->context->start_time = system_time;
      doTask( task );
      task->context->end_time = system_time;
      if ( node->next != NULL )
        node->next->prev = NULL;
      free( task->context->params );
      free( task->context );
      free( task );
      task_list->head = node->next;
      free( node );
    }
  }
}
