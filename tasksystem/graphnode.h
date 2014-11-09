

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

namespace tasks {

template <
	typename TaskT,
	typename DependentListT,
	typename DependencyListT = EmptyTemplateList
>
struct GraphNode
{
	typedef TaskT task_t;
	typedef DependentListT dependent_list_t;
	typedef DependencyListT dependency_list_t;
};

} // namespace tasks

#endif // GRAPHNODE_H
