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
