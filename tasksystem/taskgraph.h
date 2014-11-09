
#ifndef TASKGRAPH_H
#define TASKGRAPH_H

#include "task.h"

namespace tasks {

typedef unsigned char ubyte_t;

class TaskGraph
{
public:
	ubyte_t* token_stream_;
	Task** tasks;
	int num_tasks;
	int num_counters;
	int num_dependencies;
	int num_wait_contexts;

	TaskGraph(ubyte_t* token_stream, int num_tasks_in, Task** tasks_in, int num_counters_in, int num_dependencies_in, int num_wait_contexts_in)
	:	token_stream_(token_stream),
		num_tasks(num_tasks_in),
		tasks(tasks_in),
		num_counters(num_counters_in),
		num_dependencies(num_dependencies_in),
		num_wait_contexts(num_wait_contexts_in)
	{
	}
};

} // namespace tasks

#endif // TASKGRAPH_H
