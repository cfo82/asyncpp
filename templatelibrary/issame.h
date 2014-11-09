
#ifndef ISSAME_H
#define ISSAME_H

template <typename Type1, typename Type2>
struct IsSame
{
	static const bool yes = false;
};

template <typename Type1>
struct IsSame<Type1, Type1>
{
	static const bool yes = true;
};

#endif // ISSAME_H
