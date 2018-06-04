#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "task_handler.h"

void dummyHandler( TaskContext_t* context )
{
  printf( "Dummy handler called with context: %s\n", (char*) (context->params) );
}

int main(void)
{
  List_t* task_list = (List_t*) malloc( sizeof(List_t) );
  memset( task_list, 0, sizeof(List_t) );
  task_list->head = NULL;
  task_list->tail = NULL;

  char post[20];
  strncpy( post, "Prateek died", 20 );
  createTask( task_list, dummyHandler, (void*) post, sizeof(post), PRIORITY_REALTIME, 0, 0 );
  strncpy( post, "Shreyas died", 20 );
  createTask( task_list, dummyHandler, (void*) post, sizeof(post), PRIORITY_LOW, 0, 0 );
  strncpy( post, "Everybody died", 20 );
  createTask( task_list, dummyHandler, (void*) post, sizeof(post), PRIORITY_NORMAL, 0, 0 );
  strncpy( post, "I am god! >:)", 20 );
  createTask( task_list, dummyHandler, (void*) post, sizeof(post), PRIORITY_NOW, 0, 0 );
  printTaskList( task_list );

  beginScheduler( task_list );
}
