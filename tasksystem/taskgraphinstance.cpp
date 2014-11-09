
#include "taskgraphinstance.h"
#include "executioncontext.h"

namespace tasks {

TaskGraphInstance::TaskGraphInstance(
	TaskQueue* task_queue_in,
	TaskGraph* graph_in,
	TaskInstance* task_instances_in,
	InterlockedInteger* counters_in,
	Dependency* dependencies_in,
	Dependency::WaitContext* wait_contexts_in
)
:	done(false),
	task_instances(task_instances_in),
	counters(counters_in),
	graph(graph_in),
	dependencies(dependencies_in),
	task_queue(task_queue_in),
	wait_contexts(wait_contexts_in)
{
	for (int i = 0; i < graph->num_tasks; ++i)
	{
		task_instances[i].graph_instance = this;
		task_instances[i].task = graph->tasks[i];
	}
	for (int i = 0; i < graph->num_dependencies; ++i)
		{ new(&dependencies[i]) Dependency(); }
}

void TaskGraphInstance::SatisfyDependency(
	const int dependency_index
)
{
	dependencies[dependency_index].Satisfy(this);
}

TaskGraphInstance* TaskGraphInstance::Create(
	TaskQueue* task_queue,
	TaskGraph* graph
)
{
	return TaskGraphInstance::InternalCreate<TaskGraphInstance>(task_queue, graph);
}

} // namespace tasks
