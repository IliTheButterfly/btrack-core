#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__


#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>
#include <type_traits>
#include <memory>
#include <vector>
#include <boost/iterator/transform_iterator.hpp>
#include <iterator>

namespace btrack::nodes::system::type_traits {



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


template <typename T> struct remove_smart_ptr { typedef T type; };
template <typename T> struct remove_smart_ptr<std::unique_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<std::shared_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<std::weak_ptr<T>> { typedef T type; };

template <class T> using remove_smart_ptr_t = typename remove_smart_ptr<T>::type;

template <class T> using remove_container_t = typename remove_container<T>::type;


template <typename T> struct remove_all_ext_and_container { typedef remove_container_t<boost::remove_all_extents_t<T>> type; };

template <class T> using remove_all_ext_and_container_t = typename remove_all_ext_and_container<T>::type;

template <typename FROM, typename TO> struct convertible_to : public boost::is_convertible<FROM, TO> {};

template <typename FROM, typename TO> using convertible_to_t = typename convertible_to<FROM, TO>::type;

constexpr bool convertible(const std::type_info& from, const std::type_info& to)
{
	return btrack::nodes::system::type_traits::convertible_to_t<decltype(from), decltype(to)>::value;
}

template <typename T> struct remove_all { typedef boost::remove_all_extents_t<remove_smart_ptr_t<boost::remove_all_extents_t<remove_smart_ptr_t<T>>>> type; };

template <typename T> using remove_all_t = typename remove_all<T>::type;

template <typename T, typename C> struct is_shared : public boost::false_type {};
template <typename T> struct is_shared<T, std::shared_ptr<T>> : public boost::true_type {};

template <typename T, typename C> struct is_unique : public boost::false_type {};
template <typename T> struct is_unique<T, std::unique_ptr<T>> : public boost::true_type {};

template <typename T, typename C> struct is_weak : public boost::false_type {};
template <typename T> struct is_weak<T, std::weak_ptr<T>> : public boost::true_type {};

template <typename T> struct container_traits { typedef T type; typedef void container; };
template <typename T> struct container_traits<std::shared_ptr<T>> { typedef T type; typedef std::shared_ptr<T> container; };
template <typename T> struct container_traits<std::unique_ptr<T>> { typedef T type; typedef std::unique_ptr<T> container; };
template <typename T> struct container_traits<std::weak_ptr<T>> { typedef T type; typedef std::weak_ptr<T> container; };

template <typename C> inline constexpr bool is_shared_v = is_shared<typename container_traits<C>::type, typename container_traits<C>::container>::value;
template <typename C> inline constexpr bool is_unique_v = is_unique<typename container_traits<C>::type, typename container_traits<C>::container>::value;
template <typename C> inline constexpr bool is_weak_v = is_weak<typename container_traits<C>::type, typename container_traits<C>::container>::value;

template <typename T1, typename C1, typename T2, typename C2> struct _is_same_container : 
	public boost::integral_constant<bool, 
		is_shared<T1, C1>::value && is_shared<T2, C2>::value ||
		is_unique<T1, C1>::value && is_unique<T2, C2>::value ||
		is_weak<T1, C1>::value && is_weak<T2, C2>::value ||
		std::is_void<C1>::value && std::is_void<T2>::value>  {};

template <typename T1, typename T2> using is_same_container = 
	_is_same_container<typename container_traits<T1>::type, typename container_traits<T1>::container,
						typename container_traits<T2>::type, typename container_traits<T2>::container>;

template<typename _Iter>
concept any_iterator = std::forward_iterator<_Iter> || 
						std::bidirectional_iterator<_Iter> || 
						std::random_access_iterator<_Iter> ||
						std::input_or_output_iterator<_Iter> ||
						std::contiguous_iterator<_Iter>;


namespace ownership {

template <typename T>
struct owned_ptr {
	typedef typename container_traits<T>::type element_type;
	typedef std::shared_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using owned_ptr_p = typename owned_ptr<T>::ptr_type;



template <typename T>
struct borrowed_ptr {
	typedef typename container_traits<T>::type element_type;
	typedef std::weak_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using borrowed_ptr_p = typename borrowed_ptr<T>::ptr_type;

template <typename T>
struct ref_ptr {
	typedef typename container_traits<T>::type element_type;
	typedef std::shared_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using ref_ptr_p = typename ref_ptr<T>::ptr_type;


} // namespace smart_ptr


template <typename T> struct is_meta : public boost::false_type {};
template <typename T> struct is_meta<MetaInput<T>> : public boost::true_type {};
template <typename T> struct is_meta<MetaInputValue<T>> : public boost::true_type {};
template <typename T> struct is_meta<MetaInputArray<T>> : public boost::true_type {};
template <typename T> struct is_meta<MetaOutput<T>> : public boost::true_type {};
template <typename T> struct is_meta<MetaOutputValue<T>> : public boost::true_type {};
template <typename T> struct is_meta<MetaOutputArray<T>> : public boost::true_type {};

template <typename T> struct is_input : public boost::false_type {};
template <typename T> struct is_input<Input<T>> : public boost::true_type {};
template <typename T> struct is_input<InputValue<T>> : public boost::true_type {};
template <typename T> struct is_input<MetaInput<T>> : public boost::true_type {};
template <typename T> struct is_input<MetaInputValue<T>> : public boost::true_type {};
template <typename T> struct is_input<MetaInputArray<T>> : public boost::true_type {};

template <typename T> struct is_output : public boost::false_type {};
template <typename T> struct is_output<Output<T>> : public boost::true_type {};
template <typename T> struct is_output<OutputValue<T>> : public boost::true_type {};
template <typename T> struct is_output<MetaOutput<T>> : public boost::true_type {};
template <typename T> struct is_output<MetaOutputValue<T>> : public boost::true_type {};
template <typename T> struct is_output<MetaOutputArray<T>> : public boost::true_type {};



template <typename FROM, typename TO> struct connectable_to : public boost::false_type {};



} // namespace btrack::nodes::system::type_traits


#endif // __TYPE_TRAITS_H__