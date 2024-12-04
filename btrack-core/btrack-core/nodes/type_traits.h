#pragma once
#include <boost/type_traits.hpp>

namespace btrack { namespace nodes { namespace type_traits {


using namespace boost;

template <typename T>
class NodeInput;
template <typename T>
class NodeInputValue;
template <typename T>
class NodeInputArray;
template <typename T>
class NodeOutput;
template <typename T>
class NodeOutputValue;
template <typename T>
class NodeOutputArray;

template <typename T>
class MetaNodeInput;
template <typename T>
class MetaNodeInputValue;
template <typename T>
class MetaNodeInputArray;
template <typename T>
class MetaNodeOutput;
template <typename T>
class MetaNodeOutputValue;
template <typename T>
class MetaNodeOutputArray;

template <typename T> struct remove_container { typedef T type; };

template <typename T> struct remove_container<NodeInput<T>> { typedef T type; };
template <typename T> struct remove_container<NodeInputValue<T>> { typedef T type; };
template <typename T> struct remove_container<NodeInputArray<T>> { typedef T type; };
template <typename T> struct remove_container<NodeOutput<T>> { typedef T type; };
template <typename T> struct remove_container<NodeOutputValue<T>> { typedef T type; };
template <typename T> struct remove_container<NodeOutputArray<T>> { typedef T type; };

template <typename T> struct remove_container<MetaNodeInput<T>> { typedef T type; };
template <typename T> struct remove_container<MetaNodeInputValue<T>> { typedef T type; };
template <typename T> struct remove_container<MetaNodeInputArray<T>> { typedef T type; };
template <typename T> struct remove_container<MetaNodeOutput<T>> { typedef T type; };
template <typename T> struct remove_container<MetaNodeOutputValue<T>> { typedef T type; };
template <typename T> struct remove_container<MetaNodeOutputArray<T>> { typedef T type; };

template <class T> using remove_container_t = typename remove_container<T>::type;

template <typename T> struct remove_all_ext_and_container { typedef remove_container_t<remove_all_extents_t<T>> type; };

template <class T> using remove_all_ext_and_container_t = typename remove_all_ext_and_container<T>::type;

template <typename FROM, typename TO> struct convertable_to : public is_convertible<FROM, TO> {};

template <typename FROM, typename TO> using convertable_to_t = typename convertable_to<FROM, TO>::type;



}}} // btrack::nodes::type_traits

