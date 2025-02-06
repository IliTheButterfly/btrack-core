#ifndef __NODEIO_H__
#define __NODEIO_H__


#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iterator>
#include <vector>
#include "nodes/system/MemoryRegistry.h"
#include "nodes/system/type_traits.h"
#include "nodes/exceptions.h"
#include "nodes/runners/NodeRunner.h"


namespace btrack::nodes::system {

using NodeRunner = runners::NodeRunner;

enum class NodeItemType : uint8_t
{
	UNKNOWN		= 0b000000,
	INPUT		= 0b000001,
	OUTPUT 		= 0b000010,
	NODE		= 0b000100,
	ARRAY		= 0b001000,
	VALUE		= 0b010000,
	META		= 0b100000,
	ANY			= 0b111111,
	INPUT_ARRAY = INPUT | ARRAY,
	INPUT_VALUE = INPUT | VALUE,
	OUTPUT_ARRAY = OUTPUT | ARRAY,
	OUTPUT_VALUE = OUTPUT | VALUE,
	META_INPUT_ARRAY = META | INPUT | ARRAY,
	META_INPUT_VALUE = META | INPUT | VALUE,
	META_OUTPUT_ARRAY = META | OUTPUT | ARRAY,
	META_OUTPUT_VALUE = META | OUTPUT | VALUE,
	ANY_INPUT = META | INPUT | ARRAY | VALUE,
	ANY_OUTPUT = META | OUTPUT | ARRAY | VALUE,
	ANY_ARRAY = META | INPUT | OUTPUT | ARRAY,
	ANY_VALUE = META | INPUT | OUTPUT | VALUE,
};

inline NodeItemType operator&(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
inline NodeItemType operator&(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
inline NodeItemType operator&(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
// NodeItemType operator&(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }
// NodeItemType operator&(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }
// NodeItemType operator&(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }

inline NodeItemType operator|(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
inline NodeItemType operator|(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
inline NodeItemType operator|(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
// NodeItemType operator|(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }
// NodeItemType operator|(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }
// NodeItemType operator|(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }

inline NodeItemType operator^(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
inline NodeItemType operator^(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
inline NodeItemType operator^(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
// NodeItemType operator^(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }
// NodeItemType operator^(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }
// NodeItemType operator^(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }

inline NodeItemType operator~(NodeItemType x) { return (NodeItemType)(x ^ NodeItemType::ANY); }
// NodeItemType operator~(NodeItemType x) { return (NodeItemType)(x ^ NodeItemType::ANY); }
// NodeItemType operator~(const NodeItemType x) { return (const NodeItemType)((const uint8_t)x ^ NodeItemType::ANY); }
// NodeItemType operator~(const NodeItemType x) { return (const NodeItemType)((const uint8_t)x ^ NodeItemType::ANY); }


// template <typename T>
// class NodeIterator
// {
// public:
//     // STL-like iterator traits
//     using iterator_category = std::forward_iterator_tag;
//     using difference_type = std::ptrdiff_t;
//     using pointer = T*;
//     using reference = T&;

//     // Abstract base class for iterator reference
//     struct _IteratorRef
//     {
//         virtual ~_IteratorRef() = default;
//         virtual reference operator*() const = 0;
//         virtual pointer operator->() const = 0;
//         virtual _IteratorRef& operator++() = 0;
//         virtual bool operator==(const _IteratorRef& other) const = 0;
//         virtual bool operator!=(const _IteratorRef& other) const = 0;
//         // virtual std::unique_ptr<_IteratorRef> clone() const = 0; // Clone for copy construction
//     };

//     // IteratorRef to adapt IterT (the actual iterator type)
//     template <typename IterT>
//     class IteratorRef : public _IteratorRef {
//     public:
// 		using inner_pointer = typename IterT::pointer;
// 		using inner_reference = typename IterT::reference;
//         explicit IteratorRef(IterT iter) : mCurrent(iter) {}

//         reference operator*() const override {
// 			return static_cast<reference>(*mCurrent); // Ensure this matches NodeIterator<T>::reference
// 		}

//         pointer operator->() const override {
//             return &(this->operator*()); // Cast and return pointer
//         }

//         _IteratorRef& operator++() override {
//             ++mCurrent;
//             return *this;
//         }

//         bool operator==(const _IteratorRef& other) const override {
//             // return &(*mCurrent) == &(*other);
//         }

//         bool operator!=(const _IteratorRef& other) const override {
//             // return &(*mCurrent) != &(*other);
//         }

//     private:
//         IterT mCurrent; // The underlying iterator
//     };




// private:
//     std::unique_ptr<_IteratorRef> mCurrent;

//     explicit NodeIterator(std::unique_ptr<_IteratorRef> ref) : mCurrent(std::move(ref)) {}

// public:
//     // Constructor for single iterator
//     template <typename IterT>
//     explicit NodeIterator(IterT iter)
//         : mCurrent(std::make_unique<IteratorRef<IterT>>(iter)) {}

//     // Copy constructor
//     // NodeIterator(const NodeIterator& other) : mCurrent(other.mCurrent->clone()) {}

//     // Move constructor
//     // NodeIterator(NodeIterator&& other) noexcept = default;

//     // Copy assignment
//     NodeIterator& operator=(const NodeIterator& other)
//     {
//         if (this != &other)
//         {
//             // mCurrent = other.mCurrent->clone();
//         }
//         return *this;
//     }

//     // Move assignment
//     // NodeIterator& operator=(NodeIterator&& other) noexcept = default;

//     // Static create function for implicit casts
//     template <typename IterT>
// 	static NodeIterator create(IterT iter)
// 	{
// 		static_assert(boost::is_base_of<type_traits::remove_all_t<T>, type_traits::remove_all_t<typename IterT::value_type>>::value, "Inner type needs to be derived from T");
// 		static_assert(type_traits::is_same_container<T, typename IterT::value_type>::value, "Inner type needs to have same container as T");
// 		return NodeIterator(iter);
// 	}

//     // Dereference operators
//     reference operator*() const { return **mCurrent; }
//     pointer operator->() const { return (*mCurrent).operator->(); }

//     // Increment operators
//     NodeIterator& operator++()
//     {
//         ++(*mCurrent);
//         return *this;
//     }

//     NodeIterator operator++(int)
//     {
//         NodeIterator temp = *this;
//         ++(*this);
//         return temp;
//     }

//     // Comparison operators
//     bool operator==(const NodeIterator& other) const { return *mCurrent == *other.mCurrent; }
//     bool operator!=(const NodeIterator& other) const { return !(*this == other); }

//     ~NodeIterator() = default;
// };



class _NodeItem
{
private:
	const std::string mName;
	const boost::uuids::uuid mUUID;
	const NodeItemType mNodeType;
	std::string mFriendlyName;
	std::string mDescription;
	std::weak_ptr<_NodeItem> mPtr;
private:
	_NodeItem(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const boost::uuids::uuid& _uuid, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			mRunner(runner), mName(_name), mUUID(_uuid), mNodeType(_nodeType), mFriendlyName(_friendlyName), mDescription(_description) 
			{
				mPtr = std::make_shared<_NodeItem>(this);
			}
protected:
	std::shared_ptr<NodeRunner> mRunner;
	_NodeItem(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_NodeItem::_NodeItem(runner, _name, _nodeType, boost::uuids::random_generator()(), _friendlyName, _description) {}
public:
	_NodeItem() = delete;

	const std::string_view name() const { return mName; }
	const boost::uuids::uuid& uuid() const { return mUUID; }
	const NodeItemType& nodeType() const { return mNodeType; }
	const std::string_view friendlyName() const { return mFriendlyName; }
	std::string& friendlyName() { return mFriendlyName; }
	const std::string_view description() const { return mDescription; }
	std::string& description() { return mDescription; }

	bool isInput() const { return (mNodeType & NodeItemType::INPUT) == NodeItemType::INPUT; }
	bool isOutput() const { return (mNodeType & NodeItemType::OUTPUT) == NodeItemType::OUTPUT; }
	bool isNode() const { return (mNodeType & NodeItemType::NODE) == NodeItemType::NODE; }
	bool isMeta() const { return (mNodeType & NodeItemType::META) == NodeItemType::META; }
	bool isArray() const { return (mNodeType & NodeItemType::ARRAY) == NodeItemType::ARRAY; }
	bool isValue() const { return (mNodeType & NodeItemType::VALUE) == NodeItemType::VALUE; }
	bool matches(NodeItemType pattern) const { return (mNodeType | pattern) == pattern; }

	bool operator==(const _NodeItem& other) const { return uuid() == other.uuid(); }
	bool operator!=(const _NodeItem& other) const { return uuid() != other.uuid(); }

	// std::shared_ptr<_NodeItem> asShared() { return mPtr.lock(); }
	template <typename T>
	std::shared_ptr<T> asShared() { return std::reinterpret_pointer_cast<T>(mPtr.lock()); }

	virtual ~_NodeItem() = default;
};


class NodeIO : public _NodeItem
{
private:
protected:
	NodeIO(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			NodeIO::_NodeItem(runner, _name, _nodeType, _friendlyName, _description) {}
public:
	virtual constexpr const std::type_info& dataType() const = 0;
	constexpr bool convertibleFrom(const std::type_info& from) const { return type_traits::convertible(from, dataType()); }
	constexpr bool convertibleTo(const std::type_info& to) const { return type_traits::convertible(dataType(), to); }

	virtual ~NodeIO() = default;
};

#define EXPAND(x) x

#define NodeIteratorAccessor(iteratorType, name, parentType) \
struct iteratorType##_ref \
{ \
	parentType& parent; \
	iteratorType##_ref(parentType& p) : parent{p} {} \
	iteratorType begin() { return parent.name##Begin(); } \
	iteratorType end() { return parent.name##End(); } \
};\
virtual iteratorType name##Begin() = 0; \
virtual iteratorType name##End() = 0; \
iteratorType##_ref name##Iter() { return iteratorType##_ref(*this); }


#define NodeIteratorAccessorConcrete(iteratorType, name, parentType) \
struct iteratorType##_ref \
{ \
	parentType& parent; \
	iteratorType##_ref(parentType& p) : parent{p} {} \
	iteratorType begin() { return parent.name##Begin(); } \
	iteratorType end() { return parent.name##End(); } \
};\
iteratorType##_ref name##Iter() { return iteratorType##_ref(*this); }
/*iteratorType name##Begin(); \
iteratorType name##End(); \*/

#define NodeAtVirtual(returnType) \
virtual returnType##Ptr returnType##At(int index) const = 0; \
virtual returnType##Ptr returnType##At(int index) = 0;

#define NodeAtImpl(returnType, vec) \
returnType##Ptr returnType##At(int index) const override { return vec.at(index); } \
returnType##Ptr returnType##At(int index) override { return vec.at(index); }

#define NodeAtConcrete(returnType, vec) \
returnType##Ptr returnType##At(int index) const { return vec.at(index); } \
returnType##Ptr returnType##At(int index) { return vec.at(index); }

#define NodeAtCastImpl(returnType, vec) \
returnType##Ptr returnType##At(int index) const override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); } \
returnType##Ptr returnType##At(int index) override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); }

#define NodeAtWeakCastImpl(returnType, vec) \
returnType##Ptr returnType##At(int index) const override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); } \
returnType##Ptr returnType##At(int index) override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); }

// #define NodeAtWeakCastImpl(returnType, vec) \
// returnType##Ptr returnType##At(int index) const override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index).lock()); } \
// returnType##Ptr returnType##At(int index) override { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index).lock()); }

#define NodeAtCastConcrete(returnType, vec) \
returnType##Ptr returnType##At(int index) const { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); } \
returnType##Ptr returnType##At(int index) { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); }

#define NodeAtWeakCastConcrete(returnType, vec) \
returnType##Ptr returnType##At(int index) const { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); } \
returnType##Ptr returnType##At(int index) { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index)); }
// #define NodeAtWeakCastConcrete(returnType, vec) \
// returnType##Ptr returnType##At(int index) const { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index).lock()); } \
// returnType##Ptr returnType##At(int index) { return std::reinterpret_pointer_cast<typename returnType##Ptr::element_type>(vec.at(index).lock()); }



enum class ConnectionResult
{
	UNKNOWN,
	SUCCESS,
	INCOMPATIBLE_TYPES,
	FROM_NOT_OUTPUT,
	TO_NOT_INPUT,
};


class _Node : public _NodeItem, public NodeRunner
{
private:
protected:
	_Node(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Node::_NodeItem(runner, _name, _nodeType | NodeItemType::NODE, _friendlyName, _description) {}

public:
	virtual size_t inputCount() const = 0;
	virtual size_t outputCount() const = 0;

	void update() override { mRunner->update(); }

	virtual ~_Node() = default;
};

} // namespace btrack::nodes::system
#endif // __NODEIO_H__