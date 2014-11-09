
#ifndef TOKENHEADER_H
#define TOKENHEADER_H

namespace tasks {

struct TokenHeader
{
	enum Type
	{
		Start,
		QueueTask,
		InitCounter,
		Branch,
		Merge,
		Jump,
		WaitDependency,
		End
	};

	TokenHeader(const Type type_in, const int size_in)
	:	type(type_in),
		size(size_in)
	{
	}

	Type type;
	int size;
};

} // namespace tasks

#endif TOKENHEADER_H
