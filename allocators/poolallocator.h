
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
