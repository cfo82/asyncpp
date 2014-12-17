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

#ifndef POOLALLOCATOR_H
#define POOLALLOCATOR_H

#include <vector>

template <int ElementSize>
class PoolAllocator
{
private:
	union Entry
	{
		Entry* next;
		unsigned char data[ElementSize];
	};

private:
	static const size_t ChunkSize = 256;

public:
	PoolAllocator()
	:	first_free_(nullptr),
		allocated_chunks_()
	{
	}

	~PoolAllocator()
	{
		for (size_t i = 0; i < allocated_chunks_.size(); ++i)
			{ delete[] allocated_chunks_[i]; }
		allocated_chunks_.clear();
	}

	void* Allocate()
	{
		if (!first_free_)
		{
			Grow();
		}

		Entry* entry = first_free_;
		first_free_ = entry->next;
		return entry;
	}

	void Free(void* mem)
	{
		Entry* entry = reinterpret_cast<Entry*>(mem);
		entry->next = first_free_;
		first_free_ = entry;
	}

	void Grow()
	{
		Entry* chunk = new Entry[ChunkSize];
		allocated_chunks_.push_back(chunk);

		for (size_t i = 0; i < ChunkSize; ++i)
		{
			if (i < ChunkSize-1)
				{ chunk->next = (chunk+1); }
			else
				{ chunk->next = first_free_; }
		}

		first_free_ = chunk;
	}

private:
	std::vector<Entry*> allocated_chunks_;
	Entry* first_free_;
};

#endif // POOLALLOCATOR_H
