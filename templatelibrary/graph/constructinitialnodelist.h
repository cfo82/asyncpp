
#pragma once

#include "containsdependentnodeid.h"

namespace template_graph { namespace internal {

template <typename CurrentGraphNodeT, typename RestListT, typename GraphNodeListT>
struct ConstructInitialNodeList
{
	typedef typename If<
		template_graph::NodeListContainsDependentNodeId<GraphNodeListT, typename CurrentGraphNodeT::id_t>::yes,
		typename ConstructInitialNodeList<typename RestListT::current_t, typename RestListT::rest_t, GraphNodeListT>::type_t,
		TemplateList<CurrentGraphNodeT, typename ConstructInitialNodeList<typename RestListT::current_t, typename RestListT::rest_t, GraphNodeListT>::type_t>
	>::type_t type_t;
};

template <typename CurrentGraphNodeT, typename GraphNodeListT>
struct ConstructInitialNodeList<CurrentGraphNodeT, EmptyTemplateList, GraphNodeListT>
{
	typedef typename If<
		template_graph::NodeListContainsDependentNodeId<GraphNodeListT, typename CurrentGraphNodeT::id_t>::yes,
		EmptyTemplateList,
		TemplateList<CurrentGraphNodeT, EmptyTemplateList>
	>::type_t type_t;

};

} // namespace internal

template <typename GraphNodeListT>
struct ConstructInitialNodeList
{
	typedef typename internal::ConstructInitialNodeList<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, GraphNodeListT>::type_t type_t;
};

} // namespace template_graph