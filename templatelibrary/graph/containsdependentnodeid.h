/*
	check if a node contains a node-id in its dependants list
*/

#pragma once

#include "../templatelist.h"
#include "../templatelistfunctions.h"

namespace template_graph {

template <typename GraphNodeT, typename DependentNodeIdT>
struct ContainsDependentNodeId
{
	static const bool yes = GetIndexOfElement<GraphNodeT::dependent_list_t, DependentNodeIdT>::Index > -1;
};

namespace internal {


template <typename CurrentGraphNodeT, typename RestListT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId
{
	static const bool yes = 
		ContainsDependentNodeId<CurrentGraphNodeT, DependentNodeIdT>::yes ||
		NodeListContainsDependentNodeId<RestListT::current_t, RestListT::rest_t, DependentNodeIdT>::yes
		;
};

template <typename CurrentGraphNodeT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId<CurrentGraphNodeT, EmptyTemplateList, DependentNodeIdT>
{
	static const bool yes = ContainsDependentNodeId<CurrentGraphNodeT, DependentNodeIdT>::yes;
};

}

template <typename GraphNodeListT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId
{
	static const bool yes = internal::NodeListContainsDependentNodeId<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, DependentNodeIdT>::yes;
};

}
