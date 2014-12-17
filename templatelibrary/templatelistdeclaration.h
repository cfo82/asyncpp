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
 
#ifndef TEMPLATELISTDECLARATION_H
#define TEMPLATELISTDECLARATION_H

#include "templatelist.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

struct Empty { };

template <
	typename AT, typename BT = Empty, typename CT = Empty, typename DT = Empty,
	typename ET = Empty, typename FT = Empty, typename GT = Empty, typename HT = Empty,
	typename IT = Empty, typename JT = Empty, typename KT = Empty, typename LT = Empty
>
struct TemplateListDeclaration
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT, HT, IT, JT, KT, LT>::type_t> type_t;
};
template <
	typename AT
>
struct TemplateListDeclaration<AT, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, EmptyTemplateList> type_t;
};
template <
	typename AT, typename BT
>
struct TemplateListDeclaration<AT, BT, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT
>
struct TemplateListDeclaration<AT, BT, CT, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT
>
struct TemplateListDeclaration<AT, BT, CT, DT, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, Empty, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, Empty, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT, typename GT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, GT, Empty, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT, typename GT, typename HT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, GT, HT, Empty, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT, HT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT, typename GT, typename HT,
	typename IT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, GT, HT, IT, Empty, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT, HT, IT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT, typename GT, typename HT,
	typename IT, typename JT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, GT, HT, IT, JT, Empty, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT, HT, IT, JT>::type_t> type_t;
};
template <
	typename AT, typename BT, typename CT, typename DT,
	typename ET, typename FT, typename GT, typename HT,
	typename IT, typename JT, typename KT
>
struct TemplateListDeclaration<AT, BT, CT, DT, ET, FT, GT, HT, IT, JT, KT, Empty>
{
	typedef TemplateList<AT, typename TemplateListDeclaration<BT, CT, DT, ET, FT, GT, HT, IT, JT, KT>::type_t> type_t;
};

#else

template <typename Arg, typename... Arguments>
struct TemplateListDeclaration
{
	typedef TemplateList<Arg, typename TemplateListDeclaration<Arguments...>::type_t> type_t;
};
template <typename Arg>
struct TemplateListDeclaration<Arg>
{
	typedef TemplateList<Arg, EmptyTemplateList> type_t;
};

#endif

#endif // TEMPALTELISTDECLARATION_H
