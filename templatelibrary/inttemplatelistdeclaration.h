
#ifndef INTTEMPLATELISTDECLARATION_H
#define INTTEMPLATELISTDECLARATION_H

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

template <int AT, int BT = 2147483647, int CT = 2147483647, int DT = 2147483647, int ET = 2147483647, int FT = 2147483647>
struct IntTemplateListDeclaration
{
	typedef IntTemplateList<AT, typename IntTemplateListDeclaration<BT, CT, DT, ET, FT>::type_t> type_t;
};
template <int AT>
struct IntTemplateListDeclaration<AT, 2147483647, 2147483647, 2147483647, 2147483647, 2147483647>
{
	typedef IntTemplateList<AT, EmptyTemplateList> type_t;
};
template <int AT, int BT>
struct IntTemplateListDeclaration<AT, BT, 2147483647, 2147483647, 2147483647, 2147483647>
{
	typedef IntTemplateList<AT, typename IntTemplateListDeclaration<BT>::type_t> type_t;
};
template <int AT, int BT, int CT>
struct IntTemplateListDeclaration<AT, BT, CT, 2147483647, 2147483647, 2147483647>
{
	typedef IntTemplateList<AT, typename IntTemplateListDeclaration<BT, CT>::type_t> type_t;
};
template <int AT, int BT, int CT, int DT>
struct IntTemplateListDeclaration<AT, BT, CT, DT, 2147483647, 2147483647>
{
	typedef IntTemplateList<AT, typename IntTemplateListDeclaration<BT, CT, DT>::type_t> type_t;
};
template <int AT, int BT, int CT, int DT, int ET>
struct IntTemplateListDeclaration<AT, BT, CT, DT, ET, 2147483647>
{
	typedef IntTemplateList<AT, typename IntTemplateListDeclaration<BT, CT, DT, ET>::type_t> type_t;
};

#else

template <int Arg, int... Arguments>
struct IntTemplateListDeclaration
{
	typedef IntTemplateList<Arg, typename IntTemplateListDeclaration<Arguments...>::type_t> type_t;
};

template <int Arg>
struct IntTemplateListDeclaration<Arg>
{
	typedef IntTemplateList<Arg, EmptyTemplateList> type_t;
};

#endif

#endif // INTTEMPLATELISTDECLARATION_H
