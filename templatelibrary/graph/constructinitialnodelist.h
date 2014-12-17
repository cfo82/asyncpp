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