
#pragma once

namespace template_graph {

namespace internal {

template <typename GraphNodeT, typename CurrentGraphNodeT, typename RestListT>
struct ConstructParentNodeList
{
	typedef typename If<
		ContainsDependentNodeId<CurrentGraphNodeT, typename GraphNodeT::id_t>::yes,
		TemplateList<CurrentGraphNodeT, typename ConstructParentNodeList<GraphNodeT, typename RestListT::current_t, typename RestListT::rest_t>::type_t>,
		typename ConstructParentNodeList<GraphNodeT, typename RestListT::current_t, typename RestListT::rest_t>::type_t
	>::type_t type_t;
};

template <typename GraphNodeT, typename CurrentGraphNodeT>
struct ConstructParentNodeList<GraphNodeT, CurrentGraphNodeT, EmptyTemplateList>
{
	typedef typename If<
		ContainsDependentNodeId<CurrentGraphNodeT, typename GraphNodeT::id_t>::yes,
		TemplateList<CurrentGraphNodeT, EmptyTemplateList>,
		EmptyTemplateList
	>::type_t type_t;
};

} // namespace internal

template <typename GraphNodeT, typename GraphNodeListT>
struct ConstructParentNodeList
{
	typedef typename internal::ConstructParentNodeList<GraphNodeT, typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t>::type_t type_t;
};

} // namespace template_graph
