
#ifndef REMOVEREFERENCE_H
#define REMOVEREFERENCE_H

template<typename TypeT>
struct RemoveReference
{
	typedef TypeT type_t;
};
template <typename TypeT>
struct RemoveReference<TypeT&>
{
	typedef TypeT type_t;
};

#endif // REMOVEREFERENCE_H

