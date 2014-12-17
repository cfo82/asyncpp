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
