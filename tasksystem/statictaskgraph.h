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
 
#ifndef STATICTASKGRAPH_H
#define STATICTASKGRAPH_H

#include "../templatelibrary/templatelist.h"
#include "../templatelibrary/templatelistcontainer.h"
#include "../templatelibrary/inttemplatelist.h"
#include "../templatelibrary/inttemplatelistfunctions.h"

namespace tasks {

//-----------------------------------------------------------------------------------------------------------
template <typename TaskT, typename ContextDataT>
struct StaticTaskGraphNode : public Task
{
public:
	StaticTaskGraphNode() { }

	virtual void Run(TaskInstance* instance)
	{
		TaskGraphInstanceTemplate<ContextDataT>* context_instance = static_cast<TaskGraphInstanceTemplate<ContextDataT>*>(instance->graph_instance);
		TaskT::Run(instance, context_instance->context_data);
	}
};




//-----------------------------------------------------------------------------------------------------------
template <typename CurrentNodeT, typename RestListT, typename ContextDataT>
struct InnerComputeStaticTaskGraphList
{
	typedef TemplateList<
		StaticTaskGraphNode<typename CurrentNodeT::task_t, ContextDataT>,
		typename InnerComputeStaticTaskGraphList<typename RestListT::current_t, typename RestListT::rest_t, ContextDataT>::type_t
	> type_t;
};
template <typename CurrentNodeT, typename ContextDataT>
struct InnerComputeStaticTaskGraphList<CurrentNodeT, EmptyTemplateList, ContextDataT>
{
	typedef TemplateList<
		StaticTaskGraphNode<typename CurrentNodeT::task_t, ContextDataT>,
		EmptyTemplateList
	> type_t;
};
template <typename GraphNodesT, typename ContextDataT>
struct ComputeStaticTaskGraphList
{
	typedef typename InnerComputeStaticTaskGraphList<typename GraphNodesT::current_t, typename GraphNodesT::rest_t, ContextDataT>::type_t type_t;
};




//-----------------------------------------------------------------------------------------------------------
template <typename TaskContainerT, int N, typename CurrentNodeT, typename RestListT>
struct InnerTaskPointerContainer
{
	InnerTaskPointerContainer(TaskContainerT& container)
	:	current(&container.GetData<N>()),
		rest(container)
	{
	}

	Task* current;
	InnerTaskPointerContainer<TaskContainerT, N+1, typename RestListT::current_t, typename RestListT::rest_t> rest;
};
template <typename TaskContainerT, int N, typename CurrentNodeT>
struct InnerTaskPointerContainer<TaskContainerT, N, CurrentNodeT, EmptyTemplateList>
{
	InnerTaskPointerContainer(TaskContainerT& container)
	:	current(&container.GetData<N>())
	{
	}

	Task* current;
};
template <typename TaskContainerT, typename TaskListT>
struct TaskPointerContainer
{
	TaskPointerContainer(TaskContainerT& container)
	:	inner_container(container)
	{
	}
	InnerTaskPointerContainer<TaskContainerT, 0, typename TaskListT::current_t, typename TaskListT::rest_t> inner_container;
};




//-----------------------------------------------------------------------------------------------------------
template <typename CurrentNodeT, typename RestListT>
struct InnerComputeDependencyList
{
	typedef typename MergeIntTemplateListNoDuplicates<
		typename CurrentNodeT::dependency_list_t,
		typename InnerComputeDependencyList<typename RestListT::current_t, typename RestListT::rest_t>::type_t
	>::type_t type_t;
};
template <typename CurrentNodeT>
struct InnerComputeDependencyList<CurrentNodeT, EmptyTemplateList>
{
	typedef typename CurrentNodeT::dependency_list_t type_t;
};
template <typename GraphNodesT>
struct ComputeDependencyList
{
	typedef typename InnerComputeDependencyList<typename GraphNodesT::current_t, typename GraphNodesT::rest_t>::type_t type_t;
};




//-----------------------------------------------------------------------------------------------------------
template <typename GraphNodesT, typename ContextDataT>
struct StaticTaskGraph
{
	typedef typename ComputeStaticTaskGraphList<GraphNodesT, ContextDataT>::type_t task_list_t;

	typedef typename ComputeDependencyList<GraphNodesT>::type_t dependency_list_t;

	typedef TemplateListContainer<task_list_t> task_container_t;

	typedef TaskPointerContainer<task_container_t, task_list_t> task_pointer_container_t;

	typedef typename TokenStreamGenerator<GraphNodesT> token_stream_generator_t;

	typedef typename token_stream_generator_t::type_t tokens_t;

	static const int NumCounters = typename token_stream_generator_t::merge_counter_list_t::Size;

	static const int NumDependencies = dependency_list_t::Size;

	static const int NumWaitContexts = token_stream_generator_t::WaitContextCount;

	task_container_t task_container;
	task_pointer_container_t task_pointer_container;
	tokens_t tokens;
	TaskGraph graph;

	StaticTaskGraph()
	:	task_container(),
		task_pointer_container(task_container),
		tokens(),
		graph(reinterpret_cast<ubyte_t*>(&tokens), task_list_t::Size, reinterpret_cast<Task**>(&task_pointer_container), NumCounters, NumDependencies, NumWaitContexts)
	{
	}

	static StaticTaskGraph<GraphNodesT, ContextDataT>* GetInstance()
	{
		static StaticTaskGraph<GraphNodesT, ContextDataT> graph;
		return &graph;
	}

	static TaskGraphInstanceTemplate<ContextDataT>* CreateInstance(TaskQueue* task_queue)
	{
		return TaskGraphInstanceTemplate<context_data_t>::Create(task_queue, &GetInstance()->graph);
	}
};

} // namespace tasks

#endif // STATICTASKGRAPH_H
