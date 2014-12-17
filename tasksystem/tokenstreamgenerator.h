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
 
#ifndef TOKENSTREAMGENERATOR_H
#define TOKENSTREAMGENERATOR_H

#include "tokens.h"
#include "graphnode.h"
#include "task.h"
#include "../templatelibrary/templatelist.h"
#include "../templatelibrary/templatelistfunctions.h"
#include "../templatelibrary/templatelistdeclaration.h"
#include "../templatelibrary/inttemplatelist.h"

namespace tasks {

//-----------------------------------------------------------------------------------------------------------
/*
	check if a node contains a task in its dependants list
*/
template <typename GraphNodeT, typename DependantTaskT>
struct ContainsDependantTask
{
	static const bool yes = GetIndexOfElement<GraphNodeT::dependent_list_t, DependantTaskT>::Index > -1;
};

template <typename CurrentGraphNodeT, typename RestListT, typename DependantTaskT>
struct ListContainsDependantTask
{
	static const bool yes = 
		ContainsDependantTask<CurrentGraphNodeT, DependantTaskT>::yes ||
		ListContainsDependantTask<RestListT::current_t, RestListT::rest_t, DependantTaskT>::yes
		;
};

template <typename CurrentGraphNodeT, typename DependantTaskT>
struct ListContainsDependantTask<CurrentGraphNodeT, EmptyTemplateList, DependantTaskT>
{
	static const bool yes = ContainsDependantTask<CurrentGraphNodeT, DependantTaskT>::yes;
};




//-----------------------------------------------------------------------------------------------------------
/*
	Construct a list of initial nodes. These are nodes which do not depend from any other node
*/

template <typename CurrentGraphNodeT, typename RestListT, typename GraphNodeListT>
struct InternalConstructInitialNodeList
{
	typedef typename If<
		ListContainsDependantTask<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, typename CurrentGraphNodeT::task_t>::yes,
		typename InternalConstructInitialNodeList<typename RestListT::current_t, typename RestListT::rest_t, GraphNodeListT>::type_t,
		TemplateList<CurrentGraphNodeT, typename InternalConstructInitialNodeList<typename RestListT::current_t, typename RestListT::rest_t, GraphNodeListT>::type_t>
	>::type_t type_t;
};

template <typename CurrentGraphNodeT, typename GraphNodeListT>
struct InternalConstructInitialNodeList<CurrentGraphNodeT, EmptyTemplateList, GraphNodeListT>
{
	typedef typename If<
		ListContainsDependantTask<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, typename CurrentGraphNodeT::task_t>::yes,
		EmptyTemplateList,
		TemplateList<CurrentGraphNodeT, EmptyTemplateList>
	>::type_t type_t;

};

template <typename GraphNodeListT>
struct ConstructInitialNodeList
{
	typedef typename InternalConstructInitialNodeList<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, GraphNodeListT>::type_t type_t;
};




//-----------------------------------------------------------------------------------------------------------

template <typename GraphNodeT, typename CurrentGraphNodeT, typename RestListT>
struct InternalConstructParentNodeList
{
	typedef typename If<
		ContainsDependantTask<CurrentGraphNodeT, typename GraphNodeT::task_t>::yes,
		TemplateList<CurrentGraphNodeT, typename InternalConstructParentNodeList<GraphNodeT, typename RestListT::current_t, typename RestListT::rest_t>::type_t>,
		typename InternalConstructParentNodeList<GraphNodeT, typename RestListT::current_t, typename RestListT::rest_t>::type_t
	>::type_t type_t;
};

template <typename GraphNodeT, typename CurrentGraphNodeT>
struct InternalConstructParentNodeList<GraphNodeT, CurrentGraphNodeT, EmptyTemplateList>
{
	typedef typename If<
		ContainsDependantTask<CurrentGraphNodeT, typename GraphNodeT::task_t>::yes,
		TemplateList<CurrentGraphNodeT, EmptyTemplateList>,
		EmptyTemplateList
	>::type_t type_t;
};

template <typename GraphNodeT, typename GraphNodeListT>
struct ConstructParentNodeList
{
	typedef typename InternalConstructParentNodeList<GraphNodeT, typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t>::type_t type_t;
};




//-----------------------------------------------------------------------------------------------------------

template <typename TaskT, typename CurrentNodeT, typename RestNodeListT>
struct InternalGetNodeFromTask
{
	typedef typename If<
		IsSame<TaskT, typename CurrentNodeT::task_t>::yes,
		CurrentNodeT,
		typename InternalGetNodeFromTask<TaskT, typename RestNodeListT::current_t, typename RestNodeListT::rest_t>::type_t
	>::type_t type_t;
};
template <typename TaskT, typename CurrentNodeT>
struct InternalGetNodeFromTask<TaskT, CurrentNodeT, EmptyTemplateList>
{
	typedef typename If<
		IsSame<TaskT, typename CurrentNodeT::task_t>::yes,
		CurrentNodeT,
		EmptyTemplateList
	>::type_t type_t;
};

template <typename TaskT, typename GraphNodeListT>
struct GetNodeFromTask
{
	typedef typename InternalGetNodeFromTask<TaskT, typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t>::type_t type_t;
};





//-----------------------------------------------------------------------------------------------------------

template <typename CurrentDependentTaskT, typename RestDependentTaskListT, typename GraphNodeListT>
struct InternalConstructChildListFromNonEmptyList
{
	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<typename GetNodeFromTask<CurrentDependentTaskT, GraphNodeListT>::type_t>::type_t,
		typename InternalConstructChildListFromNonEmptyList<typename RestDependentTaskListT::current_t, typename RestDependentTaskListT::rest_t, GraphNodeListT>::type_t
	>::type_t type_t;
};
template <typename CurrentDependentTaskT, typename GraphNodeListT>
struct InternalConstructChildListFromNonEmptyList<CurrentDependentTaskT, EmptyTemplateList, GraphNodeListT>
{
	typedef typename TemplateListDeclaration<typename GetNodeFromTask<CurrentDependentTaskT, GraphNodeListT>::type_t>::type_t type_t;
};

template <typename DependentListT, typename GraphNodeListT>
struct InternalConstructChildList
{
	typedef typename InternalConstructChildListFromNonEmptyList<typename DependentListT::current_t, typename DependentListT::rest_t, GraphNodeListT>::type_t type_t;
};
template <typename GraphNodeListT>
struct InternalConstructChildList<EmptyTemplateList, GraphNodeListT>
{
	typedef EmptyTemplateList type_t;
};

template <typename GraphNodeT, typename GraphNodeListT>
struct ConstructChildList
{
	typedef typename InternalConstructChildList<typename GraphNodeT::dependent_list_t, GraphNodeListT>::type_t type_t;
};
template <typename GraphNodeT>
struct ConstructChildList<GraphNodeT, EmptyTemplateList>
{
	typedef EmptyTemplateList type_t;
};





//-----------------------------------------------------------------------------------------------------------

struct NoNode { };
struct StartTask { };
struct EndTask { };





//-----------------------------------------------------------------------------------------------------------

template <typename CurrentInitialNodeT, typename RestInitialNodeListT>
struct GenerateStartNodeDependentList
{
	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<typename CurrentInitialNodeT::task_t>::type_t,
		typename GenerateStartNodeDependentList<typename RestInitialNodeListT::current_t, typename RestInitialNodeListT::rest_t>::type_t
	>::type_t type_t;
};
template <typename CurrentInitialNodeT>
struct GenerateStartNodeDependentList<CurrentInitialNodeT, EmptyTemplateList>
{
	typedef typename TemplateListDeclaration<typename CurrentInitialNodeT::task_t>::type_t type_t;
};

template <typename InitialNodeListT>
struct GenerateStartNode
{
	typedef GraphNode<
		StartTask,
		typename GenerateStartNodeDependentList<typename InitialNodeListT::current_t, typename InitialNodeListT::rest_t>::type_t
	> type_t;
};

//-----------------------------------------------------------------------------------------------------------

template <typename EndNodeT, typename CurrentNodeT, typename RestNodesT>
struct InternalInsertEndNode
{
	static const bool NeedsAddedTask = CurrentNodeT::dependent_list_t::Size == 0;

	typedef typename If<
		NeedsAddedTask,
		GraphNode<
			typename CurrentNodeT::task_t,
			typename TemplateListDeclaration<EndTask>::type_t,
			typename CurrentNodeT::dependency_list_t
		>,
		CurrentNodeT
	>::type_t new_node_type_t;

	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<new_node_type_t>::type_t,
		typename InternalInsertEndNode<EndNodeT, typename RestNodesT::current_t, typename RestNodesT::rest_t>::type_t
	>::type_t type_t;
};
template <typename EndNodeT, typename CurrentNodeT>
struct InternalInsertEndNode<EndNodeT, CurrentNodeT, EmptyTemplateList>
{
	static const bool NeedsAddedTask = CurrentNodeT::dependent_list_t::Size == 0;

	typedef typename If<
		NeedsAddedTask,
		GraphNode<
			typename CurrentNodeT::task_t,
			typename TemplateListDeclaration<EndTask>::type_t,
			typename CurrentNodeT::dependency_list_t
		>,
		CurrentNodeT
	>::type_t new_node_type_t;

	typedef typename TemplateListDeclaration<new_node_type_t>::type_t type_t;
};

template <typename EndNodeT, typename GraphNodeListT>
struct InsertEndNode
{
	typedef typename MergeTemplateList<
		typename InternalInsertEndNode<EndNodeT, typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t>::type_t,
		typename TemplateListDeclaration<EndNodeT>::type_t
	>::type_t type_t;
};

//-----------------------------------------------------------------------------------------------------------

template <int CounterIdIn, int MergeCountIn, typename GraphNodeT>
struct MergeCounterAssignment
{
	static const int CounterId = CounterIdIn;
	static const int MergeCount = MergeCountIn;
	typedef GraphNodeT node_t;
};

template <typename CurrentGraphNodeT, typename RestListT, typename GraphNodesT>
struct InternalConstructMergeCounterList
{
	typedef typename InternalConstructMergeCounterList<typename RestListT::current_t, typename RestListT::rest_t, GraphNodesT>::type_t recursive_counter_list_t;

	static const int CounterId = recursive_counter_list_t::Size;

	typedef typename ConstructParentNodeList<CurrentGraphNodeT, GraphNodesT>::type_t parent_node_list_t;

	static const bool NeedsCounter = parent_node_list_t::Size > 1;

	typedef typename If<
		NeedsCounter,
		TemplateList<MergeCounterAssignment<CounterId, parent_node_list_t::Size, CurrentGraphNodeT>, recursive_counter_list_t>,
		recursive_counter_list_t
	>::type_t type_t;
};
template <typename CurrentGraphNodeT, typename GraphNodesT>
struct InternalConstructMergeCounterList<CurrentGraphNodeT, EmptyTemplateList, GraphNodesT>
{
	static const int CounterId = 0;

	typedef typename ConstructParentNodeList<CurrentGraphNodeT, GraphNodesT>::type_t parent_node_list_t;

	static const bool NeedsCounter = parent_node_list_t::Size > 1;

	typedef typename If<
		NeedsCounter,
		typename TemplateListDeclaration<MergeCounterAssignment<0, parent_node_list_t::Size, CurrentGraphNodeT> >::type_t,
		EmptyTemplateList
	>::type_t type_t;
};
template <typename GraphNodesT>
struct ConstructMergeCounterList
{
	typedef typename InternalConstructMergeCounterList<typename GraphNodesT::current_t, typename GraphNodesT::rest_t, GraphNodesT>::type_t type_t;
};

template <int ValueIn>
struct IntContainer
{
	static const int Value = ValueIn;
};

template <typename SearchedGraphNodeT, typename CurrentMergeCounterT, typename RestListT>
struct InternalGetMergeCounterIdFromNode
{
	static const bool Hit = IsSame<SearchedGraphNodeT, typename CurrentMergeCounterT::node_t>::yes;

	typedef typename If<
		Hit,
		IntContainer<CurrentMergeCounterT::CounterId>,
		IntContainer<InternalGetMergeCounterIdFromNode<SearchedGraphNodeT, typename RestListT::current_t, typename RestListT::rest_t>::CounterId>
	>::type_t if_t;

	static const int CounterId = if_t::Value;
};
template <typename SearchedGraphNodeT, typename CurrentMergeCounterT>
struct InternalGetMergeCounterIdFromNode<SearchedGraphNodeT, CurrentMergeCounterT, EmptyTemplateList>
{
	static const bool Hit = IsSame<SearchedGraphNodeT, typename CurrentMergeCounterT::node_t>::yes;

	typedef typename If<
		Hit,
		IntContainer<CurrentMergeCounterT::CounterId>,
		IntContainer<-1>
	>::type_t if_t;

	static const int CounterId = if_t::Value;
};

template <typename SearchedGraphNodeT, typename MergeCounterListT>
struct GetMergeCounterIdFromNode
{
	static const int CounterId = InternalGetMergeCounterIdFromNode<SearchedGraphNodeT, typename MergeCounterListT::current_t, typename MergeCounterListT::rest_t>::CounterId;
};
template <typename SearchedGraphNodeT>
struct GetMergeCounterIdFromNode<SearchedGraphNodeT, EmptyTemplateList>
{
	static const int CounterId = -1;
};

//-----------------------------------------------------------------------------------------------------------

/*
	template to walk the graph in depth first order. 
*/

template <
	typename CurrentGraphNodeT,
	typename TraversalPredecessorT,
	typename RestListT,
	typename GraphNodeListT,
	typename VisitedNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct InternalDepthFirstGraphTraversalHorizontal;

template <
	typename CurrentGraphNodeT,
	typename TraversalPredecessorT,
	typename ChildrenT,
	typename GraphNodeListT,
	typename VisitedNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct InternalDepthFirstGraphTraversalVertical
{
	typedef CurrentGraphNodeT current_graph_node_t;
	typedef TraversalPredecessorT traversal_predecessor_t;
	typedef typename VisitedNodeListT visited_node_list_t;

	static const bool SkipNode = GetIndexOfElement<VisitedNodeListT, CurrentGraphNodeT>::Index >= 0;

	typedef typename MergeTemplateListNoDuplicates<
		VisitedNodeListT,
		typename TemplateListDeclaration<CurrentGraphNodeT>::type_t
	>::type_t to_node_visited_node_list_t;

	typedef typename InternalDepthFirstGraphTraversalHorizontal<
		typename ChildrenT::current_t,
		CurrentGraphNodeT,
		typename ChildrenT::rest_t,
		GraphNodeListT,
		to_node_visited_node_list_t,
		VisitNodeCallbackT,
		SkipNodeCallbackT,
		CustomArgumentsT
	> recursive_graph_interpreter_t;

	typedef typename MergeTemplateListNoDuplicates<
		to_node_visited_node_list_t,
		typename recursive_graph_interpreter_t::recursive_visited_node_list_t
	>::type_t recursive_visited_node_list_t;

	typedef VisitNodeCallbackT<
		CurrentGraphNodeT,
		GraphNodeListT,
		CustomArgumentsT
	> node_callback_t;

	typedef typename If<
		SkipNode,
		typename SkipNodeCallbackT<
			CurrentGraphNodeT,
			GraphNodeListT,
			CustomArgumentsT
		>::type_t,
		typename MergeTemplateList<
			typename node_callback_t::type_t,
			typename recursive_graph_interpreter_t::type_t
		>::type_t
	>::type_t type_t;
};
template <
	typename CurrentGraphNodeT,
	typename TraversalPredecessorT,
	typename GraphNodeListT,
	typename VisitedNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct InternalDepthFirstGraphTraversalVertical<CurrentGraphNodeT, TraversalPredecessorT, EmptyTemplateList, GraphNodeListT, VisitedNodeListT, VisitNodeCallbackT, SkipNodeCallbackT, CustomArgumentsT>
{
	typedef CurrentGraphNodeT current_graph_node_t;
	typedef TraversalPredecessorT traversal_predecessor_t;
	typedef typename VisitedNodeListT visited_node_list_t;

	static const bool SkipNode = GetIndexOfElement<VisitedNodeListT, CurrentGraphNodeT>::Index >= 0;

	typedef typename MergeTemplateListNoDuplicates<
		VisitedNodeListT,
		typename TemplateListDeclaration<CurrentGraphNodeT>::type_t
	>::type_t to_node_visited_node_list_t;

	typedef to_node_visited_node_list_t recursive_visited_node_list_t;

	typedef VisitNodeCallbackT<
		CurrentGraphNodeT,
		GraphNodeListT,
		CustomArgumentsT
	> node_callback_t;

	typedef typename If<
		SkipNode,
		typename SkipNodeCallbackT<
			CurrentGraphNodeT,
			GraphNodeListT,
			CustomArgumentsT
		>::type_t,
		typename node_callback_t::type_t
	>::type_t type_t;
};

template <
	typename CurrentGraphNodeT,
	typename TraversalPredecessorT,
	typename RestListT,
	typename GraphNodeListT,
	typename VisitedNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct InternalDepthFirstGraphTraversalHorizontal
{
	typedef CurrentGraphNodeT current_graph_node_t;
	typedef TraversalPredecessorT traversal_predecessor_t;
	typedef typename VisitedNodeListT visited_node_list_t;

	typedef InternalDepthFirstGraphTraversalVertical<
		CurrentGraphNodeT,
		TraversalPredecessorT,
		typename ConstructChildList<CurrentGraphNodeT, GraphNodeListT>::type_t,
		GraphNodeListT,
		VisitedNodeListT,
		VisitNodeCallbackT,
		SkipNodeCallbackT,
		CustomArgumentsT
	> internal_graph_interpreter_t;

	typedef InternalDepthFirstGraphTraversalHorizontal<
		typename RestListT::current_t,
		TraversalPredecessorT,
		typename RestListT::rest_t,
		GraphNodeListT,
		typename internal_graph_interpreter_t::recursive_visited_node_list_t,
		VisitNodeCallbackT,
		SkipNodeCallbackT,
		CustomArgumentsT
	> recursive_graph_interpreter_t;

	typedef typename internal_graph_interpreter_t::to_node_visited_node_list_t to_node_visited_node_list_t;
	typedef typename MergeTemplateListNoDuplicates<
		typename internal_graph_interpreter_t::recursive_visited_node_list_t,
		typename recursive_graph_interpreter_t::recursive_visited_node_list_t
	>::type_t recursive_visited_node_list_t;

	typedef typename MergeTemplateList<
		typename internal_graph_interpreter_t::type_t,
		typename recursive_graph_interpreter_t::type_t
	>::type_t type_t;
};

template <
	typename CurrentGraphNodeT,
	typename TraversalPredecessorT,
	typename GraphNodeListT,
	typename VisitedNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct InternalDepthFirstGraphTraversalHorizontal<CurrentGraphNodeT, TraversalPredecessorT, EmptyTemplateList, GraphNodeListT, VisitedNodeListT, VisitNodeCallbackT, SkipNodeCallbackT, CustomArgumentsT>
{
	typedef CurrentGraphNodeT current_graph_node_t;
	typedef TraversalPredecessorT traversal_predecessor_t;
	typedef typename VisitedNodeListT visited_node_list_t;

	typedef typename InternalDepthFirstGraphTraversalVertical<
		CurrentGraphNodeT,
		TraversalPredecessorT,
		typename ConstructChildList<CurrentGraphNodeT, GraphNodeListT>::type_t,
		GraphNodeListT,
		VisitedNodeListT,
		VisitNodeCallbackT,
		SkipNodeCallbackT,
		CustomArgumentsT
	> internal_graph_interpreter_t;

	typedef typename internal_graph_interpreter_t::to_node_visited_node_list_t to_node_visited_node_list_t;
	typedef typename internal_graph_interpreter_t::recursive_visited_node_list_t recursive_visited_node_list_t;

	typedef typename internal_graph_interpreter_t::type_t type_t;
};


template <
	typename GraphNodeListT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class VisitNodeCallbackT,
	template <typename NodeT, typename NodesT, typename ArgumentsT> class SkipNodeCallbackT,
	typename CustomArgumentsT
>
struct DepthFirstGraphTraversal
{
	typedef typename ConstructInitialNodeList<GraphNodeListT>::type_t initial_node_list_t;

	typedef typename initial_node_list_t::current_t start_node_t;

	typedef typename ConstructChildList<
		start_node_t,
		GraphNodeListT
	>::type_t start_node_children_t;

	typedef InternalDepthFirstGraphTraversalHorizontal<
		typename start_node_children_t::current_t,
		start_node_t,
		typename start_node_children_t::rest_t,
		GraphNodeListT,
		EmptyTemplateList,
		VisitNodeCallbackT,
		SkipNodeCallbackT,
		CustomArgumentsT
	> graph_interpreter_t;

	typedef typename MergeTemplateList<
		typename VisitNodeCallbackT<
			start_node_t,
			GraphNodeListT,
			CustomArgumentsT
		>::type_t,
		typename graph_interpreter_t::type_t
	>::type_t type_t;
};


//-----------------------------------------------------------------------------------------------------------

template <typename OwnerNodeT, typename TokenT>
struct DefaultIntermediateInstruction
{
	typedef OwnerNodeT owner_node_t;
	typedef TokenT token_t;
	static const int InstructionSize = sizeof(TokenT);
};

template <typename OwnerNodeT, typename BranchTargetListT>
struct BranchIntermediateInstruction
{
	typedef OwnerNodeT owner_node_t;
	typedef BranchTargetListT branch_target_list_t;
	static const int InstructionSize = sizeof(BranchTokenHeader) + sizeof(int) * BranchTargetListT::Size;
};

template <typename OwnerNodeT, typename JumpTargetT>
struct JumpIntermediateInstruction
{
	typedef OwnerNodeT owner_node_t;
	typedef JumpTargetT jump_target_t;
	static const int InstructionSize = sizeof(JumpTokenTemplate<0>);
};

template <typename OwnerNodeT, int DependencyIndexIn, int WaitContextIndexIn>
struct WaitIntermediateInstruction
{
	typedef OwnerNodeT owner_node_t;
	static const int DependencyIndex = DependencyIndexIn;
	static const int WaitContextIndex = WaitContextIndexIn;
	static const int InstructionSize = sizeof(JumpTokenTemplate<0>);
};



//-----------------------------------------------------------------------------------------------------------

template <typename OwnerNodeT, int CurrentDependency, typename RestDependenciesT>
struct InnerWaitDependencyListGenerator
{
	static const bool Hit = IsSame<SearchedGraphNodeT, typename CurrentMergeCounterT::node_t>::yes;

	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<WaitIntermediateInstruction<OwnerNodeT, CurrentDependency, 0> >::type_t,
		typename InnerWaitDependencyListGenerator<OwnerNodeT, RestDependenciesT::Value, typename RestDependenciesT::rest_t>::type_t
	>::type_t type_t;
};
template <typename OwnerNodeT, int CurrentDependency>
struct InnerWaitDependencyListGenerator<OwnerNodeT, CurrentDependency, EmptyTemplateList>
{
	typedef typename TemplateListDeclaration<WaitIntermediateInstruction<OwnerNodeT, CurrentDependency, 0> >::type_t type_t;
};

template <typename OwnerNodeT, typename DependencyListT>
struct WaitDependencyListGenerator
{
	typedef typename InnerWaitDependencyListGenerator<OwnerNodeT, DependencyListT::Value, typename DependencyListT::rest_t>::type_t type_t;
};

template <typename OwnerNodeT>
struct WaitDependencyListGenerator<OwnerNodeT, EmptyTemplateList>
{
	typedef EmptyTemplateList type_t;
};

template <typename CurrentGraphNodeT, typename GraphNodeListT, typename ArgumentsT>
struct NodeInterpreter2
{
	static const int NodeIndex = GetIndexOfElement<GraphNodeListT, CurrentGraphNodeT>::Index - 1;

	typedef typename ConstructParentNodeList<CurrentGraphNodeT, GraphNodeListT>::type_t parent_node_list_t;
	typedef typename ConstructChildList<CurrentGraphNodeT, GraphNodeListT>::type_t child_node_list_t;
	typedef typename CurrentGraphNodeT::dependency_list_t dependency_list_t;

	static const int MergeCounterId = GetMergeCounterIdFromNode<CurrentGraphNodeT, typename ArgumentsT::merge_counter_list_t>::CounterId;
	static const bool IsEndNode = IsSame<CurrentGraphNodeT, typename ArgumentsT::end_node_t>::yes;

	static const bool MergeTokenCondition = parent_node_list_t::Size > 1;
	typedef typename If<
		MergeTokenCondition,
		typename TemplateListDeclaration<DefaultIntermediateInstruction<CurrentGraphNodeT, MergeTokenTemplate<MergeCounterId> > >::type_t,
		EmptyTemplateList
	>::type_t merge_token_t;

	static const bool WaitDependencyCondition = dependency_list_t::Size > 0;
	typedef typename If<
		WaitDependencyCondition,
		typename WaitDependencyListGenerator<CurrentGraphNodeT, dependency_list_t>::type_t,
		EmptyTemplateList
	>::type_t wait_list_t;

	static const bool BranchTokenCondition = CurrentGraphNodeT::dependent_list_t::Size > 1;
	typedef typename If<
		BranchTokenCondition,
		typename TemplateListDeclaration<BranchIntermediateInstruction<CurrentGraphNodeT, child_node_list_t> >::type_t,
		EmptyTemplateList
	>::type_t branch_token_t;

	typedef typename If<
		NodeIndex >= 0,
		typename If<
			IsEndNode,
			typename TemplateListDeclaration<DefaultIntermediateInstruction<CurrentGraphNodeT, EndToken> >::type_t,
			typename TemplateListDeclaration<DefaultIntermediateInstruction<CurrentGraphNodeT, QueueTaskTokenTemplate<NodeIndex> > >::type_t // 1-based list because 0 is the start task
		>::type_t,
		EmptyTemplateList // start task!
	>::type_t queue_token_t;

	typedef typename MergeTemplateList<
		merge_token_t,
		typename MergeTemplateList<
			typename MergeTemplateList<
				wait_list_t,
				queue_token_t
			>::type_t,
			branch_token_t
		>::type_t
	>::type_t type_t;
};
template <typename CurrentGraphNodeT, typename GraphNodeListT, typename ArgumentsT>
struct SkipNodeInterpreter
{
	typedef typename TemplateListDeclaration<
		JumpIntermediateInstruction<NoNode, CurrentGraphNodeT>
	>::type_t type_t;
};

//-----------------------------------------------------------------------------------------------------------

/*
	check if the node should be skipped from the graph interpreter
*/


template <typename CurrentGraphNodeT, typename TraversalPredecessorT, typename ParentListT>
struct InternalSkipGraphNode
{
	static const bool yes = !IsSame<
		TraversalPredecessorT,
		typename ParentListT::current_t
	>::yes;
};

template <typename CurrentGraphNodeT, typename TraversalPredecessorT>
struct InternalSkipGraphNode<CurrentGraphNodeT, TraversalPredecessorT, EmptyTemplateList>
{
	static const bool yes = false;
};


template <typename CurrentGraphNodeT, typename TraversalPredecessorT, typename GraphNodeListT>
struct SkipGraphNode
{
	typedef InternalSkipGraphNode<
		CurrentGraphNodeT,
		TraversalPredecessorT,
		typename ConstructParentNodeList<CurrentGraphNodeT, GraphNodeListT>::type_t
	> internal_skip_graph_node_t;

	static const bool yes = internal_skip_graph_node_t::yes;
};

//-----------------------------------------------------------------------------------------------------------
template <typename GraphNodeT, int CurrentAddress, typename CurrentInstructionT, typename RestInstructionsT>
struct InternalGetNodeAddress
{
	static const bool NodeDiscovered = IsSame<GraphNodeT, typename CurrentInstructionT::owner_node_t>::yes;

	typedef typename If<
		NodeDiscovered,
		IntContainer<CurrentAddress>,
		IntContainer<
			InternalGetNodeAddress<GraphNodeT, CurrentAddress + CurrentInstructionT::InstructionSize, typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t>::Address
		>
	>::type_t if_t;

	static const int Address = if_t::Value;
};
template <typename GraphNodeT, int CurrentAddress, typename CurrentInstructionT>
struct InternalGetNodeAddress<GraphNodeT, CurrentAddress, CurrentInstructionT, EmptyTemplateList>
{
	static const bool NodeDiscovered = IsSame<GraphNodeT, typename CurrentInstructionT::owner_node_t>::yes;

	typedef typename If<
		NodeDiscovered,
		IntContainer<CurrentAddress>,
		IntContainer<-1>
	>::type_t if_t;

	static const int Address = if_t::Value;
};

template <typename GraphNodeT, typename InstructionListT>
struct GetNodeAddress
{
	static const int Address = InternalGetNodeAddress<GraphNodeT, 0, typename InstructionListT::current_t, typename InstructionListT::rest_t>::Address;
};

//-----------------------------------------------------------------------------------------------------------

template <typename CurrentBranchTargetT, typename RestBranchTargetsT, typename InstructionListT>
struct IntermediateBranchTargetTranslation
{
	typedef IntTemplateList<
		GetNodeAddress<CurrentBranchTargetT, InstructionListT>::Address,
		typename IntermediateBranchTargetTranslation<typename RestBranchTargetsT::current_t, typename RestBranchTargetsT::rest_t, InstructionListT>::type_t
	> type_t;
};
template <typename CurrentBranchTargetT, typename InstructionListT>
struct IntermediateBranchTargetTranslation<CurrentBranchTargetT, EmptyTemplateList, InstructionListT>
{
	typedef IntTemplateList<
		GetNodeAddress<CurrentBranchTargetT, InstructionListT>::Address,
		EmptyTemplateList
	> type_t;
};


template <typename InstructionT, typename InstructionListT>
struct TranslateInstruction
{
	typedef typename InstructionT::token_t type_t;
};
template <typename OwnerNodeT, typename BranchTargetListT, typename InstructionListT>
struct TranslateInstruction<BranchIntermediateInstruction<OwnerNodeT, BranchTargetListT>, InstructionListT>
{
	typedef BranchTokenTemplate<typename IntermediateBranchTargetTranslation<typename BranchTargetListT::current_t, typename BranchTargetListT::rest_t, InstructionListT>::type_t> type_t;
};
template <typename OwnerNodeT, typename JumpTargetT, typename InstructionListT>
struct TranslateInstruction<JumpIntermediateInstruction<OwnerNodeT, JumpTargetT>, InstructionListT>
{
	typedef JumpTokenTemplate<GetNodeAddress<JumpTargetT, InstructionListT>::Address> type_t;
};
template <typename OwnerNodeT, int DependencyIndexIn, int WaitContextIndexIn, typename InstructionListT>
struct TranslateInstruction<WaitIntermediateInstruction<OwnerNodeT, DependencyIndexIn, WaitContextIndexIn>, InstructionListT>
{
	typedef WaitDependencyTokenTemplate<DependencyIndexIn, WaitContextIndexIn> type_t;
};

template <typename CurrentInstructionT, typename RestInstructionsT, typename InstructionListT>
struct InternalTranslateInstructions
{
	typedef TemplateList<
		typename TranslateInstruction<CurrentInstructionT, InstructionListT>::type_t,
		typename InternalTranslateInstructions<typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t, InstructionListT>::type_t
	> type_t;
};
template <typename CurrentInstructionT, typename InstructionListT>
struct InternalTranslateInstructions<CurrentInstructionT, EmptyTemplateList, InstructionListT>
{
	typedef TemplateList<
		typename TranslateInstruction<CurrentInstructionT, InstructionListT>::type_t,
		EmptyTemplateList
	> type_t;
};
template <typename InstructionListT>
struct TranslateInstructions
{
	typedef typename InternalTranslateInstructions<typename InstructionListT::current_t, typename InstructionListT::rest_t, InstructionListT>::type_t type_t;
};




//-----------------------------------------------------------------------------------------------------------

/*
	The intermediate code generator did not know how many wait contexts are needed and therefore was not
	able to assign the id of the wait context to use when issuing wait instructions. This template fixes
	that problem by assigning a unique wait context ids for each wait instruction. In parallel it counts
	the number of wait contexts which are needed to execute the given token stream. The count can then
	then be used when creating the graph instance.
*/

template <typename CurrentInstructionT, typename RestInstructionsT, int CurrentIndex>
struct InnerAssignWaitContextIndices
{
	typedef TemplateList<
		CurrentInstructionT,
		typename InnerAssignWaitContextIndices<typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t, CurrentIndex>::type_t
	> type_t;

	static const int WaitContextCount = InnerAssignWaitContextIndices<typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t, CurrentIndex>::WaitContextCount;
};
template <typename OwnerNodeT, int DependencyIndexIn, int WaitContextIndexIn, typename RestInstructionsT, int CurrentIndex>
struct InnerAssignWaitContextIndices<WaitIntermediateInstruction<OwnerNodeT, DependencyIndexIn, WaitContextIndexIn>, RestInstructionsT, CurrentIndex>
{
	typedef TemplateList<
		WaitIntermediateInstruction<OwnerNodeT, DependencyIndexIn, CurrentIndex>,
		typename InnerAssignWaitContextIndices<typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t, CurrentIndex+1>::type_t
	> type_t;

	static const int WaitContextCount = InnerAssignWaitContextIndices<typename RestInstructionsT::current_t, typename RestInstructionsT::rest_t, CurrentIndex+1>::WaitContextCount + 1;
};
template <typename CurrentInstructionT, int CurrentIndex>
struct InnerAssignWaitContextIndices<CurrentInstructionT, EmptyTemplateList, CurrentIndex>
{
	typedef TemplateList<
		CurrentInstructionT,
		EmptyTemplateList
	> type_t;
	static const int WaitContextCount = 0;
};
template <typename OwnerNodeT, int DependencyIndexIn, int WaitContextIndexIn, int CurrentIndex>
struct InnerAssignWaitContextIndices<WaitIntermediateInstruction<OwnerNodeT, DependencyIndexIn, WaitContextIndexIn>, EmptyTemplateList, CurrentIndex>
{
	typedef TemplateList<
		WaitIntermediateInstruction<OwnerNodeT, DependencyIndexIn, CurrentIndex>,
		EmptyTemplateList
	> type_t;
	static const int WaitContextCount = 1;
};
template <typename InstructionListT>
struct AssignWaitContextIndices
{
	typedef typename InnerAssignWaitContextIndices<typename InstructionListT::current_t, typename InstructionListT::rest_t, 0>::type_t type_t;
	static const int WaitContextCount = InnerAssignWaitContextIndices<typename InstructionListT::current_t, typename InstructionListT::rest_t, 0>::WaitContextCount;
};

//-----------------------------------------------------------------------------------------------------------

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------------------------------------
template <typename CurrentTokenT, typename RestTokenListT>
struct TokenContainer
{
	TokenContainer()
	{
	}
	CurrentTokenT token;
	TokenContainer<typename RestTokenListT::current_t, typename RestTokenListT::rest_t> rest;
};
template <typename CurrentTokenT>
struct TokenContainer<CurrentTokenT, EmptyTemplateList>
{
	CurrentTokenT token;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------------------------------

/*
	walk the graph in depth first order to generate tokens
*/

template <typename StartNodeT, typename CurrentMergeCounterT, typename RestListT>
struct InternalConstructMergeCounterInitializations
{
	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<
			DefaultIntermediateInstruction<StartNodeT, InitCounterTokenTemplate<CurrentMergeCounterT::CounterId, CurrentMergeCounterT::MergeCount> >
		>::type_t,
		typename InternalConstructMergeCounterInitializations<StartNodeT, typename RestListT::current_t, typename RestListT::rest_t>::type_t
	>::type_t type_t;
};
template <typename StartNodeT, typename CurrentMergeCounterT>
struct InternalConstructMergeCounterInitializations<StartNodeT, CurrentMergeCounterT, EmptyTemplateList>
{
	typedef typename TemplateListDeclaration<
		DefaultIntermediateInstruction<StartNodeT, InitCounterTokenTemplate<CurrentMergeCounterT::CounterId, CurrentMergeCounterT::MergeCount> >
	>::type_t type_t;
};
template <typename StartNodeT, typename MergeCounterListT>
struct ConstructMergeCounterInitializations
{
	typedef typename InternalConstructMergeCounterInitializations<StartNodeT, typename MergeCounterListT::current_t, typename MergeCounterListT::rest_t>::type_t type_t;
};
template <typename StartNodeT>
struct ConstructMergeCounterInitializations<StartNodeT, EmptyTemplateList>
{
	typedef EmptyTemplateList type_t;
};

template <typename MergeCounterListT, typename EndNodeT>
struct GraphTraversalArgument
{
	typedef MergeCounterListT merge_counter_list_t;
	typedef EndNodeT end_node_t;
};

template <typename GraphNodeListT>
struct TokenStreamGenerator
{
	typedef typename GenerateStartNode<typename ConstructInitialNodeList<GraphNodeListT>::type_t>::type_t start_node_t;

	typedef typename GraphNode<EndTask, EmptyTemplateList> end_node_t;

	typedef typename MergeTemplateList<typename TemplateListDeclaration<start_node_t>::type_t, GraphNodeListT>::type_t node_list_with_start_node_t;

	typedef typename InsertEndNode<end_node_t, node_list_with_start_node_t>::type_t node_list_with_end_node_t;

	typedef node_list_with_end_node_t node_list_t;

	typedef typename ConstructMergeCounterList<node_list_t>::type_t merge_counter_list_t;

	typedef DepthFirstGraphTraversal<node_list_t, NodeInterpreter2, SkipNodeInterpreter, GraphTraversalArgument<merge_counter_list_t, end_node_t> > graph_traversal_t;

	typedef typename MergeTemplateList<
		typename TemplateListDeclaration<DefaultIntermediateInstruction<start_node_t, StartToken> >::type_t,
		typename MergeTemplateList<
			typename ConstructMergeCounterInitializations<start_node_t, merge_counter_list_t>::type_t,
			typename graph_traversal_t::type_t
		>::type_t
	>::type_t intermediate_instructions_before_wait_context_assignment_t;

	typedef typename AssignWaitContextIndices<intermediate_instructions_before_wait_context_assignment_t>::type_t intermediate_instructions_t;
	static const int WaitContextCount = AssignWaitContextIndices<intermediate_instructions_before_wait_context_assignment_t>::WaitContextCount;

	typedef typename TranslateInstructions<
		intermediate_instructions_t
	>::type_t instructions_t;

	typedef typename TokenContainer<typename instructions_t::current_t, typename instructions_t::rest_t> type_t;
};

} // namespace tasks

#endif // TOKENSTREAMGENERATOR_H
