#ifndef SerTanaWEAKCALLBACK_H
#define SerTanaWEAKCALLBACK_H



#include <functional>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace SerTana
{

// A barely usable WeakCallback

#ifdef __GXX_EXPERIMENTAL_CXX0X__

// FIXME: support std::shared_ptr as well, maybe using template template parameters

template<typename CLASS, typename... ARGS>
class WeakCallback
{
 public:

  WeakCallback(const boost::weak_ptr<CLASS>& object,
               const std::function<void (CLASS*, ARGS...)>& function)
    : object_(object), function_(function)
  {
  }

  // Default dtor, copy ctor and assignment are okay

  void operator()(ARGS&&... args) const
  {
    boost::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr)
    {
      function_(ptr.get(), std::forward<ARGS>(args)...);
    }
    // else
    // {
    //   LOG_TRACE << "expired";
    // }
  }

 private:

  boost::weak_ptr<CLASS> object_;
  std::function<void (CLASS*, ARGS...)> function_;
};

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                              void (CLASS::*function)(ARGS...))
{
  return WeakCallback<CLASS, ARGS...>(object, function);
}

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                              void (CLASS::*function)(ARGS...) const)
{
  return WeakCallback<CLASS, ARGS...>(object, function);
}

#else  // __GXX_EXPERIMENTAL_CXX0X__

// the C++98/03 version doesn't support arguments.

template<typename CLASS>
class WeakCallback
{
 public:

  WeakCallback(const boost::weak_ptr<CLASS>& object,
               const boost::function<void (CLASS*)>& function)
    : object_(object), function_(function)
  {
  }

  // Default dtor, copy ctor and assignment are okay

  void operator()() const
  {
    boost::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr)
    {
      function_(ptr.get());
    }
    // else
    // {
    //   LOG_TRACE << "expired";
    // }
  }

 private:

  boost::weak_ptr<CLASS> object_;
  boost::function<void (CLASS*)> function_;
};

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)())
{
  return WeakCallback<CLASS>(object, function);
}

template<typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const boost::shared_ptr<CLASS>& object,
                                     void (CLASS::*function)() const)
{
  return WeakCallback<CLASS>(object, function);
}

#endif  // __GXX_EXPERIMENTAL_CXX0X__
}



#endif // SerTanaWEAKCALLBACK_H
