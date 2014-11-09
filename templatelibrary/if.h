
#ifndef IF_H
#define IF_H

template <bool Criteria, typename IfT, typename ElseT>
struct If
{
	typedef IfT type_t;
};
template <typename IfT, typename ElseT>
struct If<false, IfT, ElseT>
{
	typedef ElseT type_t;
};

#endif // IF_H
