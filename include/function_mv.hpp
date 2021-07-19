//
//  Simple header only function calss for objects that must be Movalbe
//  implementation base on std::unique_ptr
//
//

#ifndef SIMPLE_FUNCTION_VM_HPP
#define SIMPLE_FUNCTION_VM_HPP

#include <memory>

namespace simple
{

template<class T>
class function_mv;

template<class Ret, class ...Args>
class function_mv<Ret(Args...)>
{
    using invoke_fn_type = Ret(*)(void*, Args&&...);
    using create_fn_type = void(*)(void*, void*);
    using destroy_fn_type = void(*)(void*);

    template<class T>
    static Ret invoke_functor(void* fn, Args&& ...args)
    {
        return (*reinterpret_cast<T*>(fn))(std::forward<Args>(args)...);
    }

    template<class T>
    static void create_functor(void* place, T&& t)
    {
        new (place) T(std::forward<T>(t));
    }

    template<class T>
    static void destroy_functor(void* place)
    {
        reinterpret_cast<T*>(place)->~T();
    }

    // helper to call destructor
    void destroy_current() noexcept
    {
        if (destroy_)
        {
            (*destroy_)(data_.get());
            destroy_ = nullptr;
            invoke_ = nullptr; // TODO ?
            data_.reset(nullptr); // TODO ?
        }
    }

    public:

    auto operator()(Args&& ...args) -> Ret
    {
        return invoke_(data_.get(), std::forward<Args>(args)...);
    }

    function_mv()
        : data_{}
        , invoke_(nullptr)
        , destroy_(nullptr)
    {}

    template<class Functor>
    function_mv(Functor&& functor)
        : invoke_{reinterpret_cast<invoke_fn_type>(invoke_functor<Functor>)}
        , destroy_{reinterpret_cast<destroy_fn_type>(destroy_functor<Functor>)}
        , data_{new char[sizeof(Functor)]}
    {
        create_functor(data_.get(), std::forward<Functor>(functor));
    }

    template<class Functor>
    auto operator = (Functor&& functor) -> function_mv& 
    {
        destroy_current();

        new (this) function_mv(std::forward<Functor>(functor));
#if 0 // TODO ?
        data_.reset(new char[sizeof(Functor)]);
        invoke_ = reinterpret_cast<invoke_fn_type>(invoke_functor<Functor>);
        destroy_ = reinterpret_cast<destroy_fn_type>(destroy_functor<Functor>);

        create_functor(data_.get(), std::forward<Functor>(functor));
#endif
        return *this;
    }

    template<class Functor>
    function_mv(function_mv<Functor>&& fn_mv):function_mv()
    {
        swap(std::forward<function_mv<Functor>>(fn_mv));
    }


    template<class Functor>
    auto operator = (function_mv<Functor>&& fn_mv) -> function_mv& 
    {
        swap(std::forward<function_mv<Functor>>(fn_mv));
        return *this;
    }

    ~function_mv() { destroy_current(); }

    template<class Functor>
    function_mv(Functor const&) = delete;

    template<class Functor>
    function_mv(function_mv const&) = delete;


    template<class Functor>
    void swap(function_mv<Functor>&& fn_mv)
    {
        data_.swap(fn_mv.data_);
        std::swap(invoke_, fn_mv.invoke_);
        std::swap(destroy_, fn_mv.destroy_);
    }

    private:

    invoke_fn_type invoke_;
    destroy_fn_type destroy_;
    std::unique_ptr<char[]> data_;
};

} // namespace simple

#endif // SIMPLE_FUNCTION_VM_HPP
