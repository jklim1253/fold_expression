#include <iostream>
#include <functional>
#include <tuple>
#include <utility>
#include <type_traits>

#include "signature.hpp"

void hello(int a, int b, char c, char d)
{
  std::cout << __func__ << std::endl;
  std::cout << (a > b? c : d) << std::endl;
}

char world(int a, int b, char c, char d)
{
  std::cout << __func__ << std::endl;
  return (a > b ? c : d);
}

template<bool HasReturn, class Signature, class...Args>
struct delegator;

template<class Signature, class...Args>
struct delegator<true, Signature, Args...>
{
  auto operator()(Signature&& callable, Args&&...args)
    ->typename std::enable_if<
        sv::has_return_type<Signature>::value,
        typename sv::signature<std::decay_t<Signature>>::return_type
      >::type
  {
    std::cout << "have return" << std::endl;
    return callable(std::forward<Args>(args)...);
  }
};

template<class Signature, class...Args>
struct delegator<false, Signature, Args...>
{
  void operator()(Signature&& callable, Args&&...args)
  {
    std::cout << "no return" << std::endl;
    callable(std::forward<Args>(args)...);
  }
};

template<class Signature, class...Args>
auto Delegate(Signature&& callable, Args&&...args)
  ->typename std::enable_if<
      !sv::has_return_type<std::decay_t<Signature>>::value,
      void
    >::type
{
  delegator<sv::has_return_type<std::decay_t<Signature>>::value, Signature, Args...>()(
    std::forward<Signature>(callable),
    std::forward<Args>(args)...);
}

template<class Signature, class...Args>
auto Delegate(Signature&& callable, Args&&...args)
  ->typename std::enable_if<
      sv::has_return_type<std::decay_t<Signature>>::value,
      typename sv::signature<std::decay_t<Signature>>::return_type
    >::type
{
  std::cout << std::boolalpha;
  std::cout << typeid(Signature).name() << std::endl;
  std::cout << typeid(char(Args...)).name() << std::endl;
  std::cout << typeid(std::decay_t<Signature>).name() << std::endl;
  std::cout << typeid(std::decay_t<char(Args...)>).name() << std::endl;

  std::cout << "Signature[" << typeid(Signature).name() << "], ";
  std::cout << "Other[" << typeid(char(Args...)).name() << "] is same? ";
  std::cout << std::is_same<Signature, char(Args...)>::value << ", ";
  std::cout << std::is_same<std::decay_t<Signature>, std::decay_t<char(Args...)>>::value << ", ";
  std::cout << std::is_same<std::decay_t<Signature>, std::decay_t<void(Args...)>>::value << std::endl;
  std::cout << sv::is_same_signature<Signature, char(Args...)>::value << std::endl;
  std::cout << sv::is_same_signature<Signature, void(Args...)>::value << std::endl;
  std::cout << sv::is_same_signature<Signature, char(int,Args...)>::value << std::endl;

  std::cout << sv::has_return_type<char(Args...)>::value << std::endl;
  std::cout << sv::has_return_type<void(Args...)>::value << std::endl;
  std::cout << sv::has_return_type<std::decay_t<Signature>>::value << std::endl;

  return delegator<sv::has_return_type<std::decay_t<Signature>>::value, Signature, Args...>()(
    std::forward<Signature>(callable),
    std::forward<Args>(args)...);
  //return callable(std::forward<Args>(args)...);
}

int main()
{
  // call free function
  //hello(1, 2, 'a', 'A');
  //hello(3, 2, 'a', 'A');

  //// call free function returning value.
  //std::cout << world(10, 20, 'b', 'B') << std::endl;
  //std::cout << world(100, 20, 'b', 'B') << std::endl;

  // call Delegate function with a callable and those arguments
  Delegate(hello, 1, 2, 'a', 'A');
  Delegate(hello, 3, 2, 'a', 'A');

  std::cout << Delegate(world, 10, 20, 'b', 'B') << std::endl;

  return 0;
}