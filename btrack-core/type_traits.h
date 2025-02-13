#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include "memory_backend.h"
#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>
#include <type_traits>



namespace btrack::type_traits {


template <typename T> struct remove_smart_ptr { typedef T type; };
template <typename T> struct remove_smart_ptr<std::unique_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<std::shared_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<std::weak_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<std::auto_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<boost::shared_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<boost::movelib::unique_ptr<T>> { typedef T type; };
template <typename T> struct remove_smart_ptr<boost::weak_ptr<T>> { typedef T type; };

template <class T> using remove_smart_ptr_t = typename remove_smart_ptr<T>::type;

template <typename T, typename C> struct is_shared : public boost::false_type {};
template <typename T> struct is_shared<T, std::shared_ptr<T>> : public boost::true_type {};
template <typename T> struct is_shared<T, boost::shared_ptr<T>> : public boost::true_type {};

template <typename T, typename C> struct is_unique : public boost::false_type {};
template <typename T> struct is_unique<T, std::unique_ptr<T>> : public boost::true_type {};
template <typename T> struct is_unique<T, boost::movelib::unique_ptr<T>> : public boost::true_type {};

template <typename T, typename C> struct is_weak : public boost::false_type {};
template <typename T> struct is_weak<T, std::weak_ptr<T>> : public boost::true_type {};
template <typename T> struct is_weak<T, boost::weak_ptr<T>> : public boost::true_type {};

template <typename T, typename C> struct is_auto : public boost::false_type {};
template <typename T> struct is_auto<T, std::auto_ptr<T>> : public boost::true_type {};


template <typename T> struct container_traits { typedef T type; typedef void container; };
template <typename T> struct container_traits<std::shared_ptr<T>> { typedef T type; typedef shared_ptr<T> container; };
template <typename T> struct container_traits<boost::shared_ptr<T>> { typedef T type; typedef shared_ptr<T> container; };
template <typename T> struct container_traits<std::unique_ptr<T>> { typedef T type; typedef unique_ptr<T> container; };
template <typename T> struct container_traits<boost::movelib::unique_ptr<T>> { typedef T type; typedef unique_ptr<T> container; };
template <typename T> struct container_traits<std::weak_ptr<T>> { typedef T type; typedef weak_ptr<T> container; };
template <typename T> struct container_traits<boost::weak_ptr<T>> { typedef T type; typedef weak_ptr<T> container; };

template <typename C> inline constexpr bool is_shared_v = is_shared<typename container_traits<C>::type, typename container_traits<C>::container>::value;
template <typename C> inline constexpr bool is_unique_v = is_unique<typename container_traits<C>::type, typename container_traits<C>::container>::value;
template <typename C> inline constexpr bool is_weak_v = is_weak<typename container_traits<C>::type, typename container_traits<C>::container>::value;

template <typename T1, typename C1, typename T2, typename C2> struct _is_same_container : 
    public boost::integral_constant<bool, 
        is_shared<T1, C1>::value && is_shared<T2, C2>::value ||
        is_unique<T1, C1>::value && is_unique<T2, C2>::value ||
        is_weak<T1, C1>::value && is_weak<T2, C2>::value ||
        std::is_void<C1>::value && std::is_void<C2>::value>  {};

template <typename T1, typename T2> using is_same_container = 
    _is_same_container<typename container_traits<T1>::type, typename container_traits<T1>::container,
                        typename container_traits<T2>::type, typename container_traits<T2>::container>;



} // namespace btrack::nodes::system::type_traits


#endif // __TYPE_TRAITS_H__