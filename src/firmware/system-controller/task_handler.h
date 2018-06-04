#ifndef TASK_HANDLER_H_
#define TASK_HANDLER_H_

#include <stdint.h>

#define PRIORITY_NOW      0
#define PRIORITY_REALTIME 1
#define PRIORITY_HIGH     2
#define PRIORITY_NORMAL   3
#define PRIORITY_LOW      4

#ifdef bool
  typedef int bool;
  #define true  1
  #define false 0
#endif /* bool */

typedef struct Node_t Node_t;
typedef struct List_t List_t;
typedef struct Task_t Task_t;
typedef struct TaskContext_t TaskContext_t;

struct TaskContext_t
{
  void* params;

  uint32_t issue_time;
  uint32_t start_time;
  uint32_t end_time;

  bool has_deadline;
  uint32_t deadline;
};

struct Task_t
{
  void (*taskHandler)( struct TaskContext_t* );
  // void (*task_callback)( TaskContext_t* );

  struct TaskContext_t* context;

  bool periodic;
  uint32_t period;

  uint32_t priority;
};


struct Node_t
{
  struct Task_t* task;
  struct Node_t* next;
  struct Node_t* prev;
};

struct List_t
{
  struct Node_t* head;
  struct Node_t* tail;
};

extern void initTaskList( List_t* task_list );
extern void createTask( List_t* task_list, void (*taskHandler)( TaskContext_t* ),
                        const void* params, uint32_t params_size,
                        uint32_t priority, uint32_t deadline, uint32_t period );
extern void createTaskExisting( List_t* task_list, Task_t* task );
extern void printTaskList( List_t* task_list );
extern void doTask( Task_t* task );
extern void beginScheduler( List_t* task_list );
extern void dummyHandler( TaskContext_t* context );

#endif /* TASK_HANDLER_H_ */
