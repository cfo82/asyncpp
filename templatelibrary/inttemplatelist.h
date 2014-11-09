
#ifndef INTTEMPLATELIST_H
#define INTTEMPLATELIST_H

template <int Current, typename RestT>
struct IntTemplateList
{
	static const int Size = RestT::Size + 1;
	static const int Value = Current;
	typedef RestT rest_t;
};
template <int Current>
struct IntTemplateList<Current, EmptyTemplateList>
{
	static const int Size = 1;
	static const int Value = Current;
	typedef EmptyTemplateList rest_t;
};

#endif // INTTEMPLATELIST_H
