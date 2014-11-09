
#ifndef INTTEMPLATELISTFUNCTIONS_H
#define INTTEMPLATELISTFUNCTIONS_H

//-----------------------------------------------------------------------------------------------------------
template <int n, int ListSize, typename ListT, int Value>
struct InternalGetIndexOfInt;
template <int n, typename ListT, int Value>
struct InternalGetIndexOfInt<n, 0, ListT, Value>
{
	static const int Index = -1;
};
template <int n, int ListSize, typename ListT, int Value>
struct InternalGetIndexOfInt
{
	typedef typename ListT::rest_t rest_t;

	static const bool IsSame = ListT::Value == Value;

	static const int Index = IsSame ? n : InternalGetIndexOfInt<n+1, rest_t::Size, rest_t, Value>::Index;
};
template <typename ListT, int Value>
struct GetIndexOfInt
{
	static const int Index = InternalGetIndexOfInt<0, ListT::Size, ListT, Value>::Index;
};




//-----------------------------------------------------------------------------------------------------------
template <typename List1T, typename List2T>
struct MergeIntTemplateListNoDuplicates
{
	static const bool Contained = GetIndexOfInt<List2T, typename List1T::Value>::Index >= 0;

	typedef typename MergeIntTemplateListNoDuplicates<typename List1T::rest_t, List2T>::type_t recursive_result_t;

	typedef typename If<
		Contained,
		recursive_result_t,
		IntTemplateList<
			List1T::Value,
			recursive_result_t
		>
	>::type_t type_t;
};
template <typename List2T>
struct MergeIntTemplateListNoDuplicates<EmptyTemplateList, List2T>
{
	typedef List2T type_t;
};

#endif // INTTEMPLATELISTFUNCTIONS_H
