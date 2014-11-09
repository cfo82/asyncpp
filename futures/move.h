
#ifndef FUTURES_MOVE_H
#define FUTURES_MOVE_H

#include "../templatelibrary/removereference.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

namespace futures {

template<class TypeT> inline
const typename RemoveReference<TypeT>::type_t& move(const TypeT& arg)
{
	// forward _Arg as movable
	return arg;
}

} // namespace futures

#else

namespace futures {

template<class TypeT> inline
typename RemoveReference<TypeT>::type_t&& move(TypeT&& arg)
{
	// forward _Arg as movable
	return ((typename RemoveReference<TypeT>::type_t&&)arg);
}

} // namespace futures

#endif

#endif // FUTURES_MOVE_H
