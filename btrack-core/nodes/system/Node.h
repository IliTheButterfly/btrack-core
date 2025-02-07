#ifndef __NODE_H__
#define __NODE_H__



#include "nodes/system/NodeIO.h"
#include "nodes/system/InputValue.h"
#include "nodes/system/OutputValue.h"

namespace btrack::nodes::system {

using namespace type_traits::ownership;

class Node : public _Node
{
public:
	using _OutputType = _Output;
	using _OutputPtr = owned_ptr_p<_Output>;
	// using OutputIterator = NodeIterator<OutputPtr>;
	// NodeIteratorAccessorConcrete(OutputIterator, Outputs, Node);

	using _InputType = _Input;
	using _InputPtr = owned_ptr_p<_Input>;
	// using InputIterator = NodeIterator<InputPtr>;
	// NodeIteratorAccessorConcrete(InputIterator, Inputs, Node);
protected:
	std::vector<_OutputPtr> mOutputs;
	std::vector<_InputPtr> mInputs;

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<InputValue<T, I>> addInputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = std::make_shared<InputValue<T, I>>(_name, _friendlyName, _description);
		mInputs.push_back(input);
		IF_WEAK_VALID(mObserver)->addItem(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<OutputValue<T, I>> addOutputValue(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = std::make_shared<OutputValue<T, I>>(_name, _friendlyName, _description);
		mOutputs.push_back(output);
		IF_WEAK_VALID(mObserver)->addItem(output);
		return output;
	}

public:
	virtual void process() = 0;

	Node(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Node::_Node(_name, NodeItemType::NODE, _friendlyName, _description) {}


	NodeAtConcrete(_Input, mInputs)
	NodeAtConcrete(_Output, mOutputs)

	_InputPtr getInput(const std::string name)
	{
		auto it = std::find_if(mInputs.begin(), mInputs.end(), [&](_InputPtr i){ return i->name() == name; });
		if (it != mInputs.end()) return *it;
		return _InputPtr();
	}

	_OutputPtr getOutput(const std::string name)
	{
		auto it = std::find_if(mOutputs.begin(), mOutputs.end(), [&](_OutputPtr i){ return i->name() == name; });
		if (it != mOutputs.end()) return *it;
		return _OutputPtr();
	}

	virtual ~Node()
	{
		for (auto i : mInputs)
		{
			IF_WEAK_VALID(mObserver)->removeItem(i);
		}
		for (auto i : mOutputs)
		{
			IF_WEAK_VALID(mObserver)->removeItem(i);
		}
	}
};

} // namespace btrack::nodes::system
#endif // __NODE_H__