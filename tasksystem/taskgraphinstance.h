/*
 * This file is part of the asyncpp Library release under the MIT license.
 *
 * Copyright (c) 2014 Christian Oberholzer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#ifndef TASKGRAPHINSTANCE_H
#define TASKGRAPHINSTANCE_H

#include "taskgraph.h"
#include "taskinstance.h"
#include "taskqueue.h"
#include "dependency.h"

#include "../concurrency/interlockedinteger.h"
#include "../concurrency/mutex.h"

#include <new>

namespace tasks {

class TaskGraphInstance
{
public:
	bool done;
	TaskInstance* task_instances;
	InterlockedInteger* counters;
	Dependency* dependencies;
	TaskGraph* graph;
	TaskQueue* task_queue;
	Dependency::WaitContext* wait_contexts;

	TaskGraphInstance(TaskQueue* task_queue_in, TaskGraph* graph_in, TaskInstance* task_instances_in, InterlockedInteger* counters_in, Dependency* dependencies_in, Dependency::WaitContext* wait_contexts_in);

	void SatisfyDependency(const int dependency_index);

	static TaskGraphInstance* Create(TaskQueue* task_queue, TaskGraph* graph);

	template <typename GaskGraphInstanceT>
	static GaskGraphInstanceT* InternalCreate(TaskQueue* task_queue, TaskGraph* graph)
	{
		ubyte_t* data = new ubyte_t[
			sizeof(GaskGraphInstanceT) +
			sizeof(TaskInstance) * graph->num_tasks +
			sizeof(int) * graph->num_counters + 
			sizeof(Dependency) * graph->num_dependencies + 
			sizeof(Dependency::WaitContext) * graph->num_wait_contexts
			];
		GaskGraphInstanceT* instance = new(data) GaskGraphInstanceT(
			task_queue,
			graph,
			reinterpret_cast<TaskInstance*>(data + sizeof(GaskGraphInstanceT)),
			reinterpret_cast<InterlockedInteger*>(data + sizeof(GaskGraphInstanceT) + sizeof(TaskInstance) * graph->num_tasks),
			reinterpret_cast<Dependency*>(data + sizeof(GaskGraphInstanceT) + sizeof(TaskInstance) * graph->num_tasks + sizeof(int) * graph->num_counters),
			reinterpret_cast<Dependency::WaitContext*>(data + sizeof(GaskGraphInstanceT) + sizeof(TaskInstance) * graph->num_tasks + sizeof(int) * graph->num_counters + sizeof(Dependency) * graph->num_dependencies)
			);
		return instance;
	}
};


template <typename ContextDataT>
struct TaskGraphInstanceTemplate : public TaskGraphInstance
{
	TaskGraphInstanceTemplate(TaskQueue* task_queue_in, TaskGraph* graph_in, TaskInstance* task_instances_in, InterlockedInteger* counters_in, Dependency* dependencies_in, Dependency::WaitContext* wait_contexts_in)
	:	TaskGraphInstance(task_queue_in, graph_in, task_instances_in, counters_in, dependencies_in, wait_contexts_in),
		context_data()
	{
	}

	static TaskGraphInstanceTemplate<ContextDataT>* Create(TaskQueue* task_queue, TaskGraph* graph)
	{
		return TaskGraphInstance::InternalCreate<TaskGraphInstanceTemplate<ContextDataT> >(task_queue, graph);
	}

	ContextDataT context_data;
};

} // namespace tasks

#endif // TASKGRAPHINSTANCE_H
