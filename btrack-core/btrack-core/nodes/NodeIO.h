#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <iterator>
#include <vector>
#include <nodes/MemoryRegistry.h>

namespace btrack { namespace nodes
{

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

NodeItemType operator&(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
NodeItemType operator&(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
NodeItemType operator&(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x & (uint8_t)y); }
// NodeItemType operator&(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }
// NodeItemType operator&(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }
// NodeItemType operator&(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x & (const uint8_t)y); }

NodeItemType operator|(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
NodeItemType operator|(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
NodeItemType operator|(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x | (uint8_t)y); }
// NodeItemType operator|(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }
// NodeItemType operator|(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }
// NodeItemType operator|(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x | (const uint8_t)y); }

NodeItemType operator^(uint8_t x, NodeItemType y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
NodeItemType operator^(NodeItemType x, uint8_t y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
NodeItemType operator^(NodeItemType x, NodeItemType y) { return (NodeItemType)((uint8_t)x ^ (uint8_t)y); }
// NodeItemType operator^(const uint8_t x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }
// NodeItemType operator^(const NodeItemType x, const uint8_t y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }
// NodeItemType operator^(const NodeItemType x, const NodeItemType y) { return (const NodeItemType)((const uint8_t)x ^ (const uint8_t)y); }

NodeItemType operator~(NodeItemType x) { return (NodeItemType)(x ^ NodeItemType::ANY); }
// NodeItemType operator~(NodeItemType x) { return (NodeItemType)(x ^ NodeItemType::ANY); }
// NodeItemType operator~(const NodeItemType x) { return (const NodeItemType)((const uint8_t)x ^ NodeItemType::ANY); }
// NodeItemType operator~(const NodeItemType x) { return (const NodeItemType)((const uint8_t)x ^ NodeItemType::ANY); }



template <typename NodeType, typename NodePtr>
class NodeIterator
{
public:
	// STL-like iterator traits
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using pointer = NodePtr;
	using reference = NodeType&;

	struct _IteratorRef
	{
		virtual reference operator*() const = 0;
		virtual pointer operator->() const = 0;

		virtual _IteratorRef& operator++() = 0;

		// virtual _IteratorRef operator++(int) = 0;

		bool operator==(_IteratorRef& other) const { return (*this).operator->() == (*other).operator->(); }
		bool operator!=(_IteratorRef& other) const { return !(*this == other); }
	};

	template <typename IterT>
	struct IteratorRef : public _IteratorRef
	{
		IterT mCurrent;

		// IteratorRef(std::vector<IterT>::iterator iter) : mCurrent{mCurrent} {}
		IteratorRef(IterT iter) : mCurrent{mCurrent} {}
		
		reference operator*() const override { return *mCurrent; }
		pointer operator->() const override  { return &((reference)*(*this)); }

		_IteratorRef& operator++() override
		{
			++mCurrent;
			return *this;
		}

		IteratorRef operator++(int)
		{
			IteratorRef temp = *this;
			++(*this);
			return temp;
		}

		// bool operator==(_IteratorRef& other) const override { return (**this) == (**other); }
		// bool operator!=(_IteratorRef& other) const override { return !(*this == other); }
	};

private:
	_IteratorRef* mCurrent;

	NodeIterator(_IteratorRef* current) : mCurrent(current) {}
public:

	template <typename IterT>
	NodeIterator(IteratorRef<IterT> current) : mCurrent(new IteratorRef<IterT>(current)) {}

	NodeIterator operator=(const NodeIterator& it) { return NodeIterator(new _IteratorRef(*(it.mCurrent))); }

	template <typename IterT>
	static NodeIterator create(IterT iter)
	{
		return NodeIterator(new IteratorRef<IterT>(iter));
	}

	reference operator*() const { return **mCurrent; }
	pointer operator->() const { return (*mCurrent); }

	NodeIterator& operator++()
	{
		(*mCurrent)++;
		return *this;
	}

	NodeIterator operator++(int)
	{
		NodeIterator temp = *this;
		(*this)++;
		return temp;
	}

	bool operator==(const NodeIterator& other) const { return (*mCurrent) == *(other.mCurrent); }
	bool operator!=(const NodeIterator& other) const { return !(*this == other); }

	~NodeIterator()
	{
		delete mCurrent;
		mCurrent = nullptr;
	}
};


class _NodeItem
{
private:
	const std::string mName;
	const boost::uuids::uuid mUUID;
	const NodeItemType mNodeType;
	std::string mFriendlyName;
	std::string mDescription;
private:
	_NodeItem(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const boost::uuids::uuid& _uuid, 
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			mName(_name), mUUID(_uuid), mNodeType(_nodeType), mFriendlyName(_friendlyName), mDescription(_description) {}
protected:
	_NodeItem(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_NodeItem::_NodeItem(_name, _nodeType, boost::uuids::random_generator()(), _friendlyName, _description) {}
public:
	_NodeItem() = delete;

	const std::string& name() const { return mName; }
	const boost::uuids::uuid& uuid() const { return mUUID; }
	const NodeItemType& nodeType() const { return mNodeType; }
	const std::string& friendlyName() const { return mFriendlyName; }
	std::string& friendlyName() { return mFriendlyName; }
	const std::string& description() const { return mDescription; }
	std::string& description() { return mDescription; }

	bool isInput() const { return (mNodeType & NodeItemType::INPUT) == NodeItemType::INPUT; }
	bool isOutput() const { return (mNodeType & NodeItemType::OUTPUT) == NodeItemType::OUTPUT; }
	bool isNode() const { return (mNodeType & NodeItemType::NODE) == NodeItemType::NODE; }
	bool isMeta() const { return (mNodeType & NodeItemType::META) == NodeItemType::META; }
	bool isArray() const { return (mNodeType & NodeItemType::ARRAY) == NodeItemType::ARRAY; }
	bool isValue() const { return (mNodeType & NodeItemType::VALUE) == NodeItemType::VALUE; }
	bool matches(NodeItemType pattern) const { return (mNodeType | pattern) == pattern; }
};

class NodeIO : public _NodeItem
{
private:
protected:
	NodeIO(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			NodeIO::_NodeItem(_name, _nodeType, _friendlyName, _description) {}
public:
	virtual const std::type_info& dataType() const = 0;
};

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


class _Node : public _NodeItem
{
private:
protected:
	_Node(
		const std::string& _name, 
		const NodeItemType& _nodeType,
		const std::string& _friendlyName = "",
		const std::string& _description = ""
		) : 
			_Node::_NodeItem(_name, _nodeType | NodeItemType::NODE, _friendlyName, _description) {}
public:
	// using NodeIOType = NodeIO;
	// using NodeIOPtr = boost::shared_ptr<NodeIO>;
	// using NodeIOIterator = NodeIterator<_Node, NodeIOType, NodeIOPtr>;
	// NodeIteratorAccessor(NodeIOIterator, NodeIO);


	virtual size_t inputCount() const = 0;
	virtual size_t outputCount() const = 0;
};

}} // btrack::nodes