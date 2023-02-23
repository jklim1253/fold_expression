#ifndef __SV_SIGNATURE_HPP__
#define __SV_SIGNATURE_HPP__
#pragma once

#include <tuple>

namespace sv
{

template<class Signature>
struct signature;

template<class R, class...Args>
struct signature<R(Args...)>
{
  using return_type = R;
  using arguments_type = std::tuple<Args...>;
};

template<class R, class...Args>
struct signature<R(*)(Args...)>
{
  using return_type = R;
  using arguments_type = std::tuple<Args...>;
};


template<class Signature>
struct has_return_type : std::true_type {};

template<class...Args>
struct has_return_type<void(Args...)> : std::false_type {};

template<class...Args>
struct has_return_type<void(*)(Args...)> : std::false_type {};


template<class Signature, class Other>
using is_same_signature =
#ifdef DUMB
  std::conjunction<
    std::is_same<typename signature<std::decay_t<Signature>>::return_type,
    typename signature<std::decay_t<Other>>::return_type>,
    std::is_same<typename signature<std::decay_t<Signature>>::arguments_type,
    typename signature<std::decay_t<Other>>::arguments_type>
  >;
#else // DUMB
  std::is_same<std::decay_t<Signature>, std::decay_t<Other>>;
#endif // DUMB

template<class Signature, class Other>
constexpr bool is_same_signature_v = is_same_signature<Signature, Other>::value;

} // namespace sv

#endif // __SV_SIGNATURE_HPP__