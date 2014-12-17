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
 
#ifndef TOKENS_H
#define TOKENS_H

#include "tokenheader.h"
#include "starttoken.h"
#include "endtoken.h"
#include "queuetasktoken.h"

namespace tasks {

struct InitCounterToken : public TokenHeader
{
	InitCounterToken(int index_in, int value_in)
	:	TokenHeader(TokenHeader::InitCounter, sizeof(InitCounterToken)),
		index(index_in),
		value(value_in)
	{
	}

	int index;
	int value;
};

template <int CounterIndex, int CounterValue>
struct InitCounterTokenTemplate : public InitCounterToken
{
	InitCounterTokenTemplate()
	:	InitCounterToken(CounterIndex, CounterValue)
	{
	}
};

struct BranchTokenHeader : public TokenHeader
{
	BranchTokenHeader(const int dim, const int size)
	:	TokenHeader(TokenHeader::Branch, size),
		dimension(dim)
	{
	}
	int dimension;
};

template <int dim>
struct BranchToken : public BranchTokenHeader
{
	BranchToken(const int* v)
	:	BranchTokenHeader(dim, sizeof(BranchToken<dim>))
	{
		for (int i = 0; i < dim; ++i)
			{ values[i] = v[i]; }

	}
	int values[dim];
};

template <typename BranchToListT>
struct BranchTokenTemplate : public BranchTokenHeader
{
private:

#pragma pack(push)
#pragma pack(1)

	template <int n, typename BranchToListT>
	struct ValuesGenerator
	{
		typedef typename BranchToListT::rest_t rest_t;

		typedef struct Result {
			Result()
			:	value(BranchToListT::Value),
				rest()
			{
			}
			int value;
			typename ValuesGenerator<rest_t::Size, rest_t>::result_t rest;
		} result_t;
	};
	template <typename BranchToListT>
	struct ValuesGenerator<1, BranchToListT>
	{
		typedef struct Result {
			Result()
			:	value(BranchToListT::Value)
			{
			}
			int value;
		} result_t;
	};

#pragma pack(pop)

public:
	BranchTokenTemplate()
	:	BranchTokenHeader(BranchToListT::Size, sizeof(BranchTokenTemplate<BranchToListT>))
	{
	}

	typename ValuesGenerator<BranchToListT::Size, BranchToListT>::result_t values;
};

struct MergeToken : public TokenHeader
{
	MergeToken(const int index_in)
	:	TokenHeader(TokenHeader::Merge, sizeof(MergeToken)),
		index(index_in)
	{
	}

	int index;
};

template <int TemplateIndex>
struct MergeTokenTemplate : public MergeToken
{
	MergeTokenTemplate()
	:	MergeToken(TemplateIndex)
	{
	}
};

struct JumpToken : public TokenHeader
{
	JumpToken(const int jump_target_in)
	:	TokenHeader(TokenHeader::Jump, sizeof(JumpToken)),
		jump_target(jump_target_in)
	{
	}

	int jump_target;
};

template <int JumpTarget>
struct JumpTokenTemplate : public JumpToken
{
	JumpTokenTemplate()
	:	JumpToken(JumpTarget)
	{
	}
};

struct WaitDependencyToken : public TokenHeader
{
	WaitDependencyToken(const int dependency_index_in, const int wait_context_index_in)
	:	TokenHeader(TokenHeader::WaitDependency, sizeof(WaitDependencyToken)),
		dependency_index(dependency_index_in),
		wait_context_index(wait_context_index_in)
	{
	}

	int dependency_index;
	int wait_context_index;
};

template <int DependencyIndex, int WaitContextIndex>
struct WaitDependencyTokenTemplate : public WaitDependencyToken
{
	WaitDependencyTokenTemplate()
	:	WaitDependencyToken(DependencyIndex, WaitContextIndex)
	{
	}
};

} // namespace tasks

#endif // TOKENS_H
