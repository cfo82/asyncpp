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
 
/*
	check if a node contains a node-id in its dependants list
*/

#pragma once

#include "../templatelist.h"
#include "../templatelistfunctions.h"

namespace template_graph {

template <typename GraphNodeT, typename DependentNodeIdT>
struct ContainsDependentNodeId
{
	static const bool yes = GetIndexOfElement<GraphNodeT::dependent_list_t, DependentNodeIdT>::Index > -1;
};

namespace internal {


template <typename CurrentGraphNodeT, typename RestListT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId
{
	static const bool yes = 
		ContainsDependentNodeId<CurrentGraphNodeT, DependentNodeIdT>::yes ||
		NodeListContainsDependentNodeId<RestListT::current_t, RestListT::rest_t, DependentNodeIdT>::yes
		;
};

template <typename CurrentGraphNodeT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId<CurrentGraphNodeT, EmptyTemplateList, DependentNodeIdT>
{
	static const bool yes = ContainsDependentNodeId<CurrentGraphNodeT, DependentNodeIdT>::yes;
};

}

template <typename GraphNodeListT, typename DependentNodeIdT>
struct NodeListContainsDependentNodeId
{
	static const bool yes = internal::NodeListContainsDependentNodeId<typename GraphNodeListT::current_t, typename GraphNodeListT::rest_t, DependentNodeIdT>::yes;
};

}
