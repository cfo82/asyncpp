
#ifndef TEMPLATELISTFUNCTIONS_H
#define TEMPLATELISTFUNCTIONS_H

#include "if.h"
#include "issame.h"
#include "templatelist.h"

//-----------------------------------------------------------------------------------------------------------
template <typename ListT>
struct IsEmptyList
{
	static const bool yes = false;
};
template <>
struct IsEmptyList<EmptyTemplateList>
{
	static const bool yes = true;
};




//-----------------------------------------------------------------------------------------------------------
template <int n, int ListSize, typename ListT, typename ElementT>
struct InternalGetIndexOfElement;
template <int n, typename ListT, typename ElementT>
struct InternalGetIndexOfElement<n, 0, ListT, ElementT>
{
	static const int Index = -1;
};
template <int n, int ListSize, typename ListT, typename ElementT>
struct InternalGetIndexOfElement
{
	typedef typename ListT::rest_t rest_t;

	static const int Index = IsSame<ListT::current_t, ElementT>::yes ? n : InternalGetIndexOfElement<n+1, rest_t::Size, rest_t, ElementT>::Index;
};
template <typename ListT, typename ElementT>
struct GetIndexOfElement
{
	static const int Index = InternalGetIndexOfElement<0, ListT::Size, ListT, ElementT>::Index;
};




//-----------------------------------------------------------------------------------------------------------
template <typename List1T, typename List2T>
struct MergeTemplateList
{
	typedef TemplateList<
		typename List1T::current_t,
		typename MergeTemplateList<
			typename List1T::rest_t,
			List2T
		>::type_t
	> type_t;
};
template <typename List2T>
struct MergeTemplateList<EmptyTemplateList, List2T>
{
	typedef List2T type_t;
};




//-----------------------------------------------------------------------------------------------------------
template <typename List1T, typename List2T>
struct MergeTemplateListNoDuplicates
{
	static const bool Contained = GetIndexOfElement<List2T, typename List1T::current_t>::Index >= 0;

	typedef typename MergeTemplateListNoDuplicates<typename List1T::rest_t, List2T>::type_t recursive_result_t;

	typedef typename If<
		Contained,
		recursive_result_t,
		TemplateList<
			typename List1T::current_t,
			recursive_result_t
		>
	>::type_t type_t;
};
template <typename List2T>
struct MergeTemplateListNoDuplicates<EmptyTemplateList, List2T>
{
	typedef List2T type_t;
};

#endif // TEMPLATELISTFUNCTIONS_H

