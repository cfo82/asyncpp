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
 
#ifndef EVALUATELAMBDATYPES_H
#define EVALUATELAMBDATYPES_H

#include "templatelist.h"
#include "templatelistdeclaration.h"

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

#else

template <typename LambdaT>
struct EvaluateLambdaTypes : public EvaluateLambdaTypes<decltype(&LambdaT::operator())>
{
};
template <typename ClassT, typename ReturnT, typename... ArgsT>
struct EvaluateLambdaTypes<ReturnT(ClassT::*)(ArgsT...) const>
{
	typedef ReturnT type_t;
	typedef typename TemplateListDeclaration<ArgsT..., EmptyTemplateList>::type_t arguments_t;
};
template <typename ReturnT, typename... ArgsT>
struct EvaluateLambdaTypes<ReturnT(*)(ArgsT...)>
{
	typedef ReturnT type_t;
	typedef typename TemplateListDeclaration<ArgsT..., EmptyTemplateList>::type_t arguments_t;
};

#endif

#endif // EVALUATELAMBDATYPES_H

