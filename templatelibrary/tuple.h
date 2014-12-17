/*
	Tuple from jabernet TODO: jabernet add correct license!
*/

#ifndef TUPLE_H
#define TUPLE_H

#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800 && _MSC_FULL_VER != 170051025) // 170051025 == ctp nov 2012

namespace impl
{
    struct Empty {};

    template<typename TupleT, typename Arg0, typename Arg1 = ::impl::Empty, typename Arg2 = ::impl::Empty, typename Arg3 = ::impl::Empty, typename Arg4 = ::impl::Empty, typename Arg5 = ::impl::Empty, typename Arg6 = ::impl::Empty, typename Arg7 = ::impl::Empty, typename Arg8 = ::impl::Empty, typename Arg9 = ::impl::Empty>
    class TupleBaseIf
    {
        template<int index> struct helper;
        template<> struct helper<0> 
        { 
            typedef Arg0 type; 
            static type get(const TupleT& tup) { return tup.get0(); }
            static void set(TupleT& tup, const type& value) { tup.set0(value); }
        };
        template<> struct helper<1>
        {
            typedef Arg1 type;
            static type get(const TupleT& tup) { return tup.get1(); }
            static void set(TupleT& tup, const type& value) { tup.set1(value); }
        };
        template<> struct helper<2>
        {
            typedef Arg2 type;
            static type get(const TupleT& tup) { return tup.get2(); }
            static void set(TupleT& tup, const type& value) { tup.set2(value); }
        };
        template<> struct helper<3>
        {
            typedef Arg3 type;
            static type get(const TupleT& tup) { return tup.get3(); }
            static void set(TupleT& tup, const type& value) { tup.set3(value); }
        };
        template<> struct helper<4>
        {
            typedef Arg4 type;
            static type get(const TupleT& tup) { return tup.get4(); }
            static void set(TupleT& tup, const type& value) { tup.set4(value); }
        };
        template<> struct helper<5>
        {
            typedef Arg5 type;
            static type get(const TupleT& tup) { return tup.get5(); }
            static void set(TupleT& tup, const type& value) { tup.set5(value); }
        };

    public:
        template<int index>
        typename helper<index>::type get() const
        {
            return helper<index>::get(*static_cast<const TupleT*>(this));
        }

        template<int index>
        void set(const typename helper<index>::type& value)
        {
            helper<index>::set(*static_cast<TupleT*>(this), value);
        }
    };

    template<typename Arg0, typename Arg1 = ::impl::Empty, typename Arg2 = ::impl::Empty, typename Arg3 = ::impl::Empty, typename Arg4 = ::impl::Empty, typename Arg5 = ::impl::Empty, typename Arg6 = ::impl::Empty, typename Arg7 = ::impl::Empty, typename Arg8 = ::impl::Empty, typename Arg9 = ::impl::Empty>
    class Tuple;

    template<typename Arg0>
    class Tuple<Arg0>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0)
            : m_arg0(arg0)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg0& get0() const { return m_arg0; }
        void set0(const Arg0& value) { m_arg0 = value; }

    private:
        Arg0 m_arg0;
    };

    template<typename Arg0, typename Arg1>
    class Tuple<Arg0, Arg1>: public Tuple<Arg0>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0, const Arg1& arg1)
            : Tuple<Arg0>(arg0)
            , m_arg1(arg1)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg1& get1() const { return m_arg1; }
        void set1(const Arg1& value) { m_arg1 = value; }

    private:
        Arg1 m_arg1;
    };

    template<typename Arg0, typename Arg1, typename Arg2>
    class Tuple<Arg0, Arg1, Arg2>: public Tuple<Arg0, Arg1>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2)
            : Tuple<Arg0, Arg1>(arg0, arg1)
            , m_arg2(arg2)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg2& get2() const { return m_arg2; }
        void set2(const Arg2& value) { m_arg2 = value; }

    private:
        Arg2 m_arg2;
    };

    template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
    class Tuple<Arg0, Arg1, Arg2, Arg3>: public Tuple<Arg0, Arg1, Arg2>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
            : Tuple<Arg0, Arg1, Arg2>(arg0, arg1, arg2)
            , m_arg3(arg3)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg3& get3() const { return m_arg3; }
        void set3(const Arg3& value) { m_arg3 = value; }

    private:
        Arg3 m_arg3;
    };

    template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    class Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>: public Tuple<Arg0, Arg1, Arg2, Arg3>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4)
            : Tuple<Arg0, Arg1, Arg2, Arg3>(arg0, arg1, arg2, arg3)
            , m_arg4(arg4)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg4& get4() const { return m_arg4; }
        void set4(const Arg4& value) { m_arg4 = value; }

    private:
        Arg4 m_arg4;
    };

    template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    class Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>: public Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>
    {
    public:
        Tuple() {}

        Tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4, const Arg5& arg5)
            : Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>(arg0, arg1, arg2, arg3, arg4)
            , m_arg5(arg5)
        {}

    private:
        template<typename TupleT, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
        friend class TupleBaseIf;

        const Arg5& get5() const { return m_arg5; }
        void set5(const Arg5& value) { m_arg5 = value; }

    private:
        Arg5 m_arg5;
    };
}

template<typename Arg0, typename Arg1 = ::impl::Empty, typename Arg2 = ::impl::Empty, typename Arg3 = ::impl::Empty, typename Arg4 = ::impl::Empty, typename Arg5 = ::impl::Empty, typename Arg6 = ::impl::Empty, typename Arg7 = ::impl::Empty, typename Arg8 = ::impl::Empty, typename Arg9 = ::impl::Empty>
class Tuple;

template<typename Arg0>
class Tuple<Arg0>:
public ::impl::Tuple<Arg0>, public ::impl::TupleBaseIf<Tuple<Arg0>, Arg0>
{
public:
    static const int size = 1;

    Tuple() {}

    Tuple(
        const Arg0& arg0
        )
        : ::impl::Tuple<Arg0>(arg0)
    {}
};

template<typename Arg0, typename Arg1>
class Tuple<Arg0, Arg1>:
public ::impl::Tuple<Arg0, Arg1>, public ::impl::TupleBaseIf<Tuple<Arg0, Arg1>, Arg0, Arg1>
{
public:
    static const int size = 2;

    Tuple() {}

    Tuple(
        const Arg0& arg0,
        const Arg1& arg1
        )
        : ::impl::Tuple<Arg0, Arg1>(arg0, arg1)
    {}
};

template<typename Arg0, typename Arg1, typename Arg2>
class Tuple<Arg0, Arg1, Arg2>:
public ::impl::Tuple<Arg0, Arg1, Arg2>, public ::impl::TupleBaseIf<Tuple<Arg0, Arg1, Arg2>, Arg0, Arg1, Arg2>
{
public:
    static const int size = 3;

    Tuple() {}

    Tuple(
        const Arg0& arg0,
        const Arg1& arg1,
        const Arg2& arg2
        )
        : ::impl::Tuple<Arg0, Arg1, Arg2>(arg0, arg1, arg2)
    {}
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
class Tuple<Arg0, Arg1, Arg2, Arg3>:
public ::impl::Tuple<Arg0, Arg1, Arg2, Arg3>, public ::impl::TupleBaseIf<Tuple<Arg0, Arg1, Arg2, Arg3>, Arg0, Arg1, Arg2, Arg3>
{
public:
    static const int size = 4;

    Tuple() {}

    Tuple(
        const Arg0& arg0,
        const Arg1& arg1,
        const Arg2& arg2,
        const Arg3& arg3
        )
        : ::impl::Tuple<Arg0, Arg1, Arg2, Arg3>(arg0, arg1, arg2, arg3)
    {}
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>:
public ::impl::Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>, public ::impl::TupleBaseIf<Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>, Arg0, Arg1, Arg2, Arg3, Arg4>
{
public:
    static const int size = 5;

    Tuple() {}

    Tuple(
        const Arg0& arg0,
        const Arg1& arg1,
        const Arg2& arg2,
        const Arg3& arg3,
        const Arg4& arg4
        )
        : ::impl::Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>(arg0, arg1, arg2, arg3, arg4)
    {}
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
class Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>:
public ::impl::Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>, public ::impl::TupleBaseIf<Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>, Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>
{
public:
    static const int size = 6;

    Tuple() {}

    Tuple(
        const Arg0& arg0,
        const Arg1& arg1,
        const Arg2& arg2,
        const Arg3& arg3,
        const Arg4& arg4,
        const Arg5& arg5
        )
        : ::impl::Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>(arg0, arg1, arg2, arg3, arg4, arg5)
    {}
};

template<typename Arg0>
Tuple<Arg0> make_tuple(const Arg0& arg0)
{
    return Tuple<Arg0>(arg0);
}

template<typename Arg0, typename Arg1>
Tuple<Arg0, Arg1> make_tuple(const Arg0& arg0, const Arg1& arg1)
{
    return Tuple<Arg0, Arg1>(arg0, arg1);
}

template<typename Arg0, typename Arg1, typename Arg2>
Tuple<Arg0, Arg1, Arg2> make_tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2)
{
    return Tuple<Arg0, Arg1, Arg2>(arg0, arg1, arg2);
}

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
Tuple<Arg0, Arg1, Arg2, Arg3> make_tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
{
    return Tuple<Arg0, Arg1, Arg2, Arg3>(arg0, arg1, arg2, arg3);
}

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
Tuple<Arg0, Arg1, Arg2, Arg3, Arg4> make_tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4)
{
    return Tuple<Arg0, Arg1, Arg2, Arg3, Arg4>(arg0, arg1, arg2, arg3, arg4);
}

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5> make_tuple(const Arg0& arg0, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4, const Arg5& arg5)
{
    return Tuple<Arg0, Arg1, Arg2, Arg3, Arg4, Arg5>(arg0, arg1, arg2, arg3, arg4, arg5);
}

#else

#include <tuple>
#include <utility>

template<typename... Args>
class Tuple: public std::tuple<Args...>
{
public:
    static const int size = sizeof...(Args);

    using std::tuple<Args...>::tuple;

    Tuple() {}

    Tuple(const std::tuple<Args...>& t)
        : std::tuple<Args...>(t)
    {
    }

    Tuple(std::tuple<Args...>&& t)
        : std::tuple<Args...>(t)
    {
    }   

    template<int index>
    typename std::tuple_element<index, std::tuple<Args...>>::type get() const
    {
    	return std::get<index>(*this);
    }

    template<int index>
    void set(typename std::tuple_element<index, std::tuple<Args...>>::type&& value)
    {
        std::get<index>(*this) = std::move(value);
    }

    template<int index>
    void set(const typename std::tuple_element<index, std::tuple<Args...>>::type& value)
    {
        std::get<index>(*this) = value;
    }
};

template<typename... Args>
Tuple<Args...> make_tuple(Args... args)
{
    return std::make_tuple(std::move(args)...);
}

#endif
#endif // TUPLE_H