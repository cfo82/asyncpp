
#ifndef TEMPLATELISTCONTAINER_H
#define TEMPLATELISTCONTAINER_H

template <typename TypeT, typename TemplateListT>
struct InternalTemplateListContainer
{
	static const int Size = TemplateListT::Size;
	typedef TypeT current_t;
	typedef InternalTemplateListContainer<typename TemplateListT::current_t, typename TemplateListT::rest_t> rest_t;

	inline current_t& GetCurrent() { return current; }
	inline rest_t& GetMore()  { return rest; }

	current_t current;
	rest_t rest;
};

template <typename TypeT>
struct InternalTemplateListContainer<TypeT, EmptyTemplateList>
{
	static const int Size = 1;
	typedef TypeT current_t;
	typedef EmptyTemplateList rest_t;

	inline current_t& GetCurrent() { return current; }

	current_t current;
};

template <typename TypeListContainerT, int n>
struct TypeExtractor
{
	typedef typename TypeExtractor<typename TypeListContainerT::rest_t, n-1>::type_t type_t;
};

template <typename TypeListContainerT>
struct TypeExtractor<TypeListContainerT, 0>
{
	typedef typename TypeListContainerT::current_t type_t;
};


template <typename TypeListContainerT, int n>
struct TypeAccessor
{
	static inline typename TypeExtractor<TypeListContainerT, n>::type_t& Get(TypeListContainerT& list)
	{
		return TypeAccessor<typename TypeListContainerT::rest_t, n-1>::Get(list.GetMore());
	}
};

template <typename TypeListContainerT>
struct TypeAccessor<TypeListContainerT, 0>
{
	static inline typename TypeListContainerT::current_t& Get(TypeListContainerT& list)
	{
		return list.GetCurrent();
	}
};

template <typename TypeListT>
struct TemplateListContainer
{
	typedef InternalTemplateListContainer<typename TypeListT::current_t, typename TypeListT::rest_t> container_t;

	template <int index>
	typename typename TypeExtractor<container_t, index>::type_t& GetData()
	{
		return TypeAccessor<container_t, index>::Get(type_list);
	}

	template <int index, typename TypeT>
	typename typename TypeExtractor<container_t, index>::type_t& SetData(const TypeT& val)
	{
		return GetData<index>() = val;
	}

	container_t type_list;
};

#endif // TEMPLATELISTCONTAINER_H
