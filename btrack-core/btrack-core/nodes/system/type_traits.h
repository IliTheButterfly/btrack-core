#pragma once
#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>

namespace btrack { namespace nodes { namespace system { namespace type_traits {



template <typename T>
class Input;
template <typename T>
class InputValue;
template <typename T>
class InputArray;
template <typename T>
class Output;
template <typename T>
class OutputValue;
template <typename T>
class OutputArray;

template <typename T>
class MetaInput;
template <typename T>
class MetaInputValue;
template <typename T>
class MetaInputArray;
template <typename T>
class MetaOutput;
template <typename T>
class MetaOutputValue;
template <typename T>
class MetaOutputArray;

template <typename T> struct remove_container { typedef T type; };

template <typename T> struct remove_container<Input<T>> { typedef T type; };
template <typename T> struct remove_container<InputValue<T>> { typedef T type; };
template <typename T> struct remove_container<InputArray<T>> { typedef T type; };
template <typename T> struct remove_container<Output<T>> { typedef T type; };
template <typename T> struct remove_container<OutputValue<T>> { typedef T type; };
template <typename T> struct remove_container<OutputArray<T>> { typedef T type; };

template <typename T> struct remove_container<MetaInput<T>> { typedef T type; };
template <typename T> struct remove_container<MetaInputValue<T>> { typedef T type; };
template <typename T> struct remove_container<MetaInputArray<T>> { typedef T type; };
template <typename T> struct remove_container<MetaOutput<T>> { typedef T type; };
template <typename T> struct remove_container<MetaOutputValue<T>> { typedef T type; };
template <typename T> struct remove_container<MetaOutputArray<T>> { typedef T type; };

template <class T> using remove_container_t = typename remove_container<T>::type;

template <typename T> struct remove_all_ext_and_container { typedef remove_container_t<boost::remove_all_extents_t<T>> type; };

template <class T> using remove_all_ext_and_container_t = typename remove_all_ext_and_container<T>::type;

template <typename FROM, typename TO> struct convertible_to : public boost::is_convertible<FROM, TO> {};

template <typename FROM, typename TO> using convertible_to_t = typename convertible_to<FROM, TO>::type;

constexpr bool convertible(const std::type_info& from, const std::type_info& to)
{
	return btrack::nodes::system::type_traits::convertible_to_t<decltype(from), decltype(to)>::value;
}


}}}} // namespace btrack::nodes::system::type_traits

