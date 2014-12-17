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
