
#ifndef TEMPLATELIST_H
#define TEMPLATELIST_H

struct EmptyTemplateList
{
	static const int Size = 0;
};

template <typename CurrentT, typename RestT>
struct TemplateList
{
	static const int Size = TemplateList<RestT::current_t, RestT::rest_t>::Size + 1;

	typedef CurrentT current_t;
	typedef RestT rest_t;
};

template <typename CurrentT>
struct TemplateList<CurrentT, EmptyTemplateList>
{
	static const int Size = 1;

	typedef CurrentT current_t;
	typedef EmptyTemplateList rest_t;
};

#endif // TEMPLATELIST_H
