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
 
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <typeinfo>

#pragma warning(disable:4503)

#include "tasksystem/task.h"
#include "tasksystem/taskinstance.h"
#include "tasksystem/taskgraph.h"
#include "tasksystem/taskgraphinstance.h"
#include "tasksystem/executioncontext.h"
#include "tasksystem/rungraphtask.h"
#include "tasksystem/taskqueue.h"
#include "tasksystem/worker.h"
#include "tasksystem/tokenstreamgenerator.h"
#include "tasksystem/graphnode.h"

// -------------------- some helper templates

using namespace tasks;

class ExampleTask : public Task
{
public:
	ExampleTask(int index_in)
	:	index(index_in)
	{
	}

	virtual ~ExampleTask()
	{
	}

	virtual void Run(TaskInstance* instance)
	{
		printf("%08x: executing task %d\n", ::GetCurrentThreadId(), index);
	}

	int index;
};

template <int index>
class ExampleTask2 : public Task
{
public:
	virtual void Run(TaskInstance* instance)
	{
		printf("%08x: executing task %d\n", ::GetCurrentThreadId(), index);
	}
};

/*typedef GraphNode<ExampleTask2<0>, TemplateListDeclaration<ExampleTask2<1>, ExampleTask2<2> >::type_t> node_0_t;
typedef GraphNode<ExampleTask2<1>, EmptyTemplateList> node_1_t;
typedef GraphNode<ExampleTask2<2>, TemplateListDeclaration<ExampleTask2<3> >::type_t> node_2_t;
typedef GraphNode<ExampleTask2<3>, EmptyTemplateList> node_3_t;*/

typedef GraphNode<ExampleTask2<0>, TemplateListDeclaration<ExampleTask2<3> >::type_t> node_0_t;
typedef GraphNode<ExampleTask2<1>, TemplateListDeclaration<ExampleTask2<4>, ExampleTask2<5> >::type_t> node_1_t;
typedef GraphNode<ExampleTask2<2>, TemplateListDeclaration<ExampleTask2<10> >::type_t> node_2_t;
typedef GraphNode<ExampleTask2<3>, TemplateListDeclaration<ExampleTask2<6> >::type_t> node_3_t;
typedef GraphNode<ExampleTask2<4>, TemplateListDeclaration<ExampleTask2<6>, ExampleTask2<9> >::type_t> node_4_t;
typedef GraphNode<ExampleTask2<5>, TemplateListDeclaration<ExampleTask2<7> >::type_t> node_5_t;
typedef GraphNode<ExampleTask2<6>, TemplateListDeclaration<ExampleTask2<8> >::type_t> node_6_t;
typedef GraphNode<ExampleTask2<7>, TemplateListDeclaration<ExampleTask2<9> >::type_t> node_7_t;
typedef GraphNode<ExampleTask2<8>, EmptyTemplateList> node_8_t;
typedef GraphNode<ExampleTask2<9>, TemplateListDeclaration<ExampleTask2<10> >::type_t> node_9_t;
typedef GraphNode<ExampleTask2<10>, EmptyTemplateList> node_10_t;

typedef TemplateListDeclaration<node_0_t, node_1_t, node_2_t, node_3_t, node_4_t, node_5_t, node_6_t, node_7_t, node_8_t, node_9_t, node_10_t>::type_t GraphNodes;

TaskQueue queue;
//TokenStream tokens;
TokenStreamGenerator<GraphNodes>::type_t tokens;
ExampleTask task_0(0);
ExampleTask task_1(1);
ExampleTask task_2(2);
ExampleTask task_3(3);
ExampleTask task_4(4);
ExampleTask task_5(5);
ExampleTask task_6(6);
ExampleTask task_7(7);
ExampleTask task_8(8);
ExampleTask task_9(9);
ExampleTask task_10(10);
Task* task_instances[] = {
	&task_0,
	&task_1,
	&task_2,
	&task_3,
	&task_4,
	&task_5,
	&task_6,
	&task_7,
	&task_8,
	&task_9,
	&task_10
};
TaskGraph graph(reinterpret_cast<ubyte_t*>(&tokens), 11, task_instances, 4, 0, 0);
TaskGraphInstance* graph_instance = TaskGraphInstance::Create(&queue, &graph);
RunGraphTask initial_task;

typedef ConstructInitialNodeList<GraphNodes>::type_t initial_nodes_t;

/*int main(const int argc, const char* argv[])
{
	/*int i0 = GetIndexOfElement<GraphNodes, node_0_t>::Index;
	int i1 = GetIndexOfElement<GraphNodes, node_1_t>::Index;
	int i2 = GetIndexOfElement<GraphNodes, node_2_t>::Index;
	int i3 = GetIndexOfElement<GraphNodes, node_3_t>::Index;
	printf("node indices in list: %d, %d, %d, %d\n", i0, i1, i2, i3);

	printf("contains node 0 task 0 %s\n", ContainsDependantTask<node_0_t, ExampleTask2<0> >::yes ? "true" : "false");
	printf("contains node 0 task 1 %s\n", ContainsDependantTask<node_0_t, ExampleTask2<1> >::yes ? "true" : "false");
	printf("contains node 0 task 2 %s\n", ContainsDependantTask<node_0_t, ExampleTask2<2> >::yes ? "true" : "false");
	printf("contains node 0 task 3 %s\n", ContainsDependantTask<node_0_t, ExampleTask2<3> >::yes ? "true" : "false");
	printf("contains node 1 task 0 %s\n", ContainsDependantTask<node_1_t, ExampleTask2<0> >::yes ? "true" : "false");
	printf("contains node 1 task 1 %s\n", ContainsDependantTask<node_1_t, ExampleTask2<1> >::yes ? "true" : "false");
	printf("contains node 1 task 2 %s\n", ContainsDependantTask<node_1_t, ExampleTask2<2> >::yes ? "true" : "false");
	printf("contains node 1 task 3 %s\n", ContainsDependantTask<node_1_t, ExampleTask2<3> >::yes ? "true" : "false");
	printf("contains node 2 task 0 %s\n", ContainsDependantTask<node_2_t, ExampleTask2<0> >::yes ? "true" : "false");
	printf("contains node 2 task 1 %s\n", ContainsDependantTask<node_2_t, ExampleTask2<1> >::yes ? "true" : "false");
	printf("contains node 2 task 2 %s\n", ContainsDependantTask<node_2_t, ExampleTask2<2> >::yes ? "true" : "false");
	printf("contains node 2 task 3 %s\n", ContainsDependantTask<node_2_t, ExampleTask2<3> >::yes ? "true" : "false");
	printf("contains node 3 task 0 %s\n", ContainsDependantTask<node_3_t, ExampleTask2<0> >::yes ? "true" : "false");
	printf("contains node 3 task 1 %s\n", ContainsDependantTask<node_3_t, ExampleTask2<1> >::yes ? "true" : "false");
	printf("contains node 3 task 2 %s\n", ContainsDependantTask<node_3_t, ExampleTask2<2> >::yes ? "true" : "false");
	printf("contains node 3 task 3 %s\n", ContainsDependantTask<node_3_t, ExampleTask2<3> >::yes ? "true" : "false");

	i0 = GetIndexOfElement<initial_nodes_t, node_0_t>::Index;
	i1 = GetIndexOfElement<initial_nodes_t, node_1_t>::Index;
	i2 = GetIndexOfElement<initial_nodes_t, node_2_t>::Index;
	i3 = GetIndexOfElement<initial_nodes_t, node_3_t>::Index;
	printf("node indices in list: %d, %d, %d, %d\n", i0, i1, i2, i3);
	//printf("%s\n", typeid(initial_nodes_t).name());
	//printf("%s\n", typeid(node_0_t).name());

	printf("token stream: %s\n", typeid(TokenStreamGenerator2<GraphNodes>::type_t).name());
	printf("parent set 0: %s\n", typeid(ConstructParentNodeList<node_0_t, GraphNodes>::type_t).name());
	printf("parent set 1: %s\n", typeid(ConstructParentNodeList<node_1_t, GraphNodes>::type_t).name());
	printf("parent set 2: %s\n", typeid(ConstructParentNodeList<node_2_t, GraphNodes>::type_t).name());
	printf("parent set 3: %s\n", typeid(ConstructParentNodeList<node_3_t, GraphNodes>::type_t).name());

	printf("node from task 0: %s\n\n", typeid(GetNodeFromTask<typename node_0_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 1: %s\n\n", typeid(GetNodeFromTask<typename node_1_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 2: %s\n\n", typeid(GetNodeFromTask<typename node_2_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 3: %s\n\n", typeid(GetNodeFromTask<typename node_3_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 4: %s\n\n", typeid(GetNodeFromTask<typename node_4_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 5: %s\n\n", typeid(GetNodeFromTask<typename node_5_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 6: %s\n\n", typeid(GetNodeFromTask<typename node_6_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 7: %s\n\n", typeid(GetNodeFromTask<typename node_7_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 8: %s\n\n", typeid(GetNodeFromTask<typename node_8_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 9: %s\n\n", typeid(GetNodeFromTask<typename node_9_t::task_t, GraphNodes>::type_t).name());
	printf("node from task 10: %s\n\n", typeid(GetNodeFromTask<typename node_10_t::task_t, GraphNodes>::type_t).name());

	printf("children 0: %s\n\n", typeid(ConstructChildList<node_0_t, GraphNodes>::type_t).name());
	printf("children 1: %s\n\n", typeid(ConstructChildList<node_1_t, GraphNodes>::type_t).name());
	printf("children 2: %s\n\n", typeid(ConstructChildList<node_2_t, GraphNodes>::type_t).name());
	printf("children 3: %s\n\n", typeid(ConstructChildList<node_3_t, GraphNodes>::type_t).name());
	printf("children 4: %s\n\n", typeid(ConstructChildList<node_4_t, GraphNodes>::type_t).name());
	printf("children 5: %s\n\n", typeid(ConstructChildList<node_5_t, GraphNodes>::type_t).name());
	printf("children 6: %s\n\n", typeid(ConstructChildList<node_6_t, GraphNodes>::type_t).name());
	printf("children 7: %s\n\n", typeid(ConstructChildList<node_7_t, GraphNodes>::type_t).name());
	printf("children 8: %s\n\n", typeid(ConstructChildList<node_8_t, GraphNodes>::type_t).name());
	printf("children 9: %s\n\n", typeid(ConstructChildList<node_9_t, GraphNodes>::type_t).name());
	printf("children 10: %s\n\n", typeid(ConstructChildList<node_10_t, GraphNodes>::type_t).name());

	printf("parents 10: %s\n\n", typeid(ConstructParentNodeList<node_10_t, GraphNodes>::type_t).name())/

	//typedef TokenStreamGenerator3<GraphNodes>::graph_interpreter_t graph_interpreter_t;
	typedef TokenStreamGenerator<GraphNodes>::graph_traversal_t::graph_interpreter_t graph_interpreter_t;
	typedef graph_interpreter_t node_0_interpreter_t;
	typedef node_0_interpreter_t::recursive_graph_interpreter_t node_1_interpreter_t;
	typedef node_1_interpreter_t::recursive_graph_interpreter_t node_2_interpreter_t;
	typedef node_0_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_3_interpreter_t;
	typedef node_1_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_4_interpreter_t;
	typedef node_4_interpreter_t::recursive_graph_interpreter_t node_5_interpreter_t;
	typedef node_3_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_61_interpreter_t;
	typedef node_4_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_62_interpreter_t;
	typedef node_5_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_7_interpreter_t;
	typedef node_61_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_8_interpreter_t;
	typedef node_62_interpreter_t::recursive_graph_interpreter_t node_91_interpreter_t;
	typedef node_7_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_92_interpreter_t;
	typedef node_91_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_101_interpreter_t;
	typedef node_2_interpreter_t::internal_graph_interpreter_t::recursive_graph_interpreter_t node_102_interpreter_t;

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node interpreter current_graph_node_t...\n");
	printf("   0: %s\n", typeid(node_0_interpreter_t::current_graph_node_t).name());
	printf("   1: %s\n", typeid(node_1_interpreter_t::current_graph_node_t).name());
	printf("   2: %s\n", typeid(node_2_interpreter_t::current_graph_node_t).name());
	printf("   3: %s\n", typeid(node_3_interpreter_t::current_graph_node_t).name());
	printf("   4: %s\n", typeid(node_4_interpreter_t::current_graph_node_t).name());
	printf("   5: %s\n", typeid(node_5_interpreter_t::current_graph_node_t).name());
	printf(" 6_1: %s\n", typeid(node_61_interpreter_t::current_graph_node_t).name());
	printf(" 6_2: %s\n", typeid(node_62_interpreter_t::current_graph_node_t).name());
	printf("   7: %s\n", typeid(node_7_interpreter_t::current_graph_node_t).name());
	printf("   8: %s\n", typeid(node_8_interpreter_t::current_graph_node_t).name());
	printf(" 9_1: %s\n", typeid(node_91_interpreter_t::current_graph_node_t).name());
	printf(" 9_2: %s\n", typeid(node_92_interpreter_t::current_graph_node_t).name());
	printf("10_1: %s\n", typeid(node_101_interpreter_t::current_graph_node_t).name());
	printf("10_2: %s\n", typeid(node_102_interpreter_t::current_graph_node_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node interpreter traversal_predecessor_t...\n");
	printf("   0: %s\n", typeid(node_0_interpreter_t::traversal_predecessor_t).name());
	printf("   1: %s\n", typeid(node_1_interpreter_t::traversal_predecessor_t).name());
	printf("   2: %s\n", typeid(node_2_interpreter_t::traversal_predecessor_t).name());
	printf("   3: %s\n", typeid(node_3_interpreter_t::traversal_predecessor_t).name());
	printf("   4: %s\n", typeid(node_4_interpreter_t::traversal_predecessor_t).name());
	printf("   5: %s\n", typeid(node_5_interpreter_t::traversal_predecessor_t).name());
	printf(" 6_1: %s\n", typeid(node_61_interpreter_t::traversal_predecessor_t).name());
	printf(" 6_2: %s\n", typeid(node_62_interpreter_t::traversal_predecessor_t).name());
	printf("   7: %s\n", typeid(node_7_interpreter_t::traversal_predecessor_t).name());
	printf("   8: %s\n", typeid(node_8_interpreter_t::traversal_predecessor_t).name());
	printf(" 9_1: %s\n", typeid(node_91_interpreter_t::traversal_predecessor_t).name());
	printf(" 9_2: %s\n", typeid(node_92_interpreter_t::traversal_predecessor_t).name());
	printf("10_1: %s\n", typeid(node_101_interpreter_t::traversal_predecessor_t).name());
	printf("10_2: %s\n", typeid(node_102_interpreter_t::traversal_predecessor_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node interpreter to_node_visited_node_list_t...\n");
	printf("   0: %s\n", typeid(node_0_interpreter_t::to_node_visited_node_list_t).name());
	printf("   1: %s\n", typeid(node_1_interpreter_t::to_node_visited_node_list_t).name());
	printf("   2: %s\n", typeid(node_2_interpreter_t::to_node_visited_node_list_t).name());
	printf("   3: %s\n", typeid(node_3_interpreter_t::to_node_visited_node_list_t).name());
	printf("   4: %s\n", typeid(node_4_interpreter_t::to_node_visited_node_list_t).name());
	printf("   5: %s\n", typeid(node_5_interpreter_t::to_node_visited_node_list_t).name());
	printf(" 6_1: %s\n", typeid(node_61_interpreter_t::to_node_visited_node_list_t).name());
	printf(" 6_2: %s\n", typeid(node_62_interpreter_t::to_node_visited_node_list_t).name());
	printf("   7: %s\n", typeid(node_7_interpreter_t::to_node_visited_node_list_t).name());
	printf("   8: %s\n", typeid(node_8_interpreter_t::to_node_visited_node_list_t).name());
	printf(" 9_1: %s\n", typeid(node_91_interpreter_t::to_node_visited_node_list_t).name());
	printf(" 9_2: %s\n", typeid(node_92_interpreter_t::to_node_visited_node_list_t).name());
	printf("10_1: %s\n", typeid(node_101_interpreter_t::to_node_visited_node_list_t).name());
	printf("10_2: %s\n", typeid(node_102_interpreter_t::to_node_visited_node_list_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node interpreter recursive_visited_node_list_t...\n");
	printf("   0: %s\n", typeid(node_0_interpreter_t::recursive_visited_node_list_t).name());
	printf("   1: %s\n", typeid(node_1_interpreter_t::recursive_visited_node_list_t).name());
	printf("   2: %s\n", typeid(node_2_interpreter_t::recursive_visited_node_list_t).name());
	printf("   3: %s\n", typeid(node_3_interpreter_t::recursive_visited_node_list_t).name());
	printf("   4: %s\n", typeid(node_4_interpreter_t::recursive_visited_node_list_t).name());
	printf("   5: %s\n", typeid(node_5_interpreter_t::recursive_visited_node_list_t).name());
	printf(" 6_1: %s\n", typeid(node_61_interpreter_t::recursive_visited_node_list_t).name());
	printf(" 6_2: %s\n", typeid(node_62_interpreter_t::recursive_visited_node_list_t).name());
	printf("   7: %s\n", typeid(node_7_interpreter_t::recursive_visited_node_list_t).name());
	printf("   8: %s\n", typeid(node_8_interpreter_t::recursive_visited_node_list_t).name());
	printf(" 9_1: %s\n", typeid(node_91_interpreter_t::recursive_visited_node_list_t).name());
	printf(" 9_2: %s\n", typeid(node_92_interpreter_t::recursive_visited_node_list_t).name());
	printf("10_1: %s\n", typeid(node_101_interpreter_t::recursive_visited_node_list_t).name());
	printf("10_2: %s\n", typeid(node_102_interpreter_t::recursive_visited_node_list_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node interpreter visited_node_list_t...\n");
	printf("   5: %s\n", typeid(node_5_interpreter_t::visited_node_list_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("node list with end node: %s\n\n", typeid(TokenStreamGenerator<GraphNodes>::node_list_with_end_node_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("start node: %s\n\n", typeid(TokenStreamGenerator<GraphNodes>::start_node_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("counter list: %s\n\n", typeid(ConstructMergeCounterList<GraphNodes>::type_t).name());

	printf("%s\n\n", typeid(node_1_interpreter_t::internal_graph_interpreter_t::recursive_visited_node_list_t).name());

	printf("parent list of start node: %s\n\n", typeid(ConstructParentNodeList<typename TokenStreamGenerator3<GraphNodes>::start_node_t, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::type_t).name());

	printf("skip node 0 after start %s\n", typeid(SkipGraphNode<node_0_t, typename TokenStreamGenerator3<GraphNodes>::start_node_t, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::internal_skip_graph_node_t).name());

	printf("skip start node? %s\n", SkipGraphNode<typename TokenStreamGenerator3<GraphNodes>::start_node_t, NoNode, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::yes ? "true" : "false");
	printf("skip node 0 after start? %s\n", SkipGraphNode<node_0_t, typename TokenStreamGenerator3<GraphNodes>::start_node_t, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::yes ? "true" : "false");
	printf("skip node 1 after start? %s\n", SkipGraphNode<node_1_t, typename TokenStreamGenerator3<GraphNodes>::start_node_t, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::yes ? "true" : "false");
	printf("skip node 2 after start? %s\n", SkipGraphNode<node_2_t, typename TokenStreamGenerator3<GraphNodes>::start_node_t, typename TokenStreamGenerator3<GraphNodes>::node_list_t>::yes ? "true" : "false");
	printf("skip node 3 after 0? %s\n", SkipGraphNode<node_3_t, node_0_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 4 after 1? %s\n", SkipGraphNode<node_4_t, node_1_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 6 after 3? %s\n", SkipGraphNode<node_6_t, node_3_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 6 after 4? %s\n", SkipGraphNode<node_6_t, node_4_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 8 after 6? %s\n", SkipGraphNode<node_8_t, node_6_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 9 after 4? %s\n", SkipGraphNode<node_9_t, node_4_t, GraphNodes>::yes ? "true" : "false");
	printf("skip node 9 after 7? %s\n", SkipGraphNode<node_9_t, node_7_t, GraphNodes>::yes ? "true" : "false");

	printf("modified_node_list: %s\n\n", typeid(TokenStreamGenerator3<GraphNodes>::node_list_t).name());/
	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("tokens: %s\n\n", typeid(TokenStreamGenerator<GraphNodes>::instructions_t).name());

	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("verify node addresses...\n");
	printf(" 0: %d\n", GetNodeAddress<node_0_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 1: %d\n", GetNodeAddress<node_1_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 2: %d\n", GetNodeAddress<node_2_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 3: %d\n", GetNodeAddress<node_3_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 4: %d\n", GetNodeAddress<node_4_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 5: %d\n", GetNodeAddress<node_5_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 6: %d\n", GetNodeAddress<node_6_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 7: %d\n", GetNodeAddress<node_7_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 8: %d\n", GetNodeAddress<node_8_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf(" 9: %d\n", GetNodeAddress<node_9_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);
	printf("10: %d\n", GetNodeAddress<node_10_t, TokenStreamGenerator<GraphNodes>::intermediate_instructions_t>::Address);/

	Worker w1 = Worker(&queue);
	Worker w2 = Worker(&queue);

	TaskInstance initial_instance;
	initial_instance.graph_instance = graph_instance;
	initial_instance.task = &initial_task;
	initial_instance.next_token = 0;

	w1.Create();
	w2.Create();

	queue.Add(&initial_instance);

	::SleepEx(1000, TRUE);

	w1.Terminate();
	w2.Terminate();

	return EXIT_SUCCESS;
}*/
