
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

