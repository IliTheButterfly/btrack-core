#ifndef __NODE_H__
#define __NODE_H__



#include "nodes/system/NodeIO.h"
#include "nodes/system/InputValue.h"
#include "nodes/system/OutputValue.h"

namespace btrack::nodes::system {

class Node : public _Node
{
public:
	using _OutputType = _Output;
	using _OutputPtr = type_traits::ownership::owned_ptr_p<_Output>;
	// using OutputIterator = NodeIterator<OutputPtr>;
	// NodeIteratorAccessorConcrete(OutputIterator, Outputs, Node);

	using _InputType = _Input;
	using _InputPtr = type_traits::ownership::owned_ptr_p<_Input>;
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

	// OutputIterator OutputsBegin() { return OutputIterator::create(mOutputs.begin()); }
	// OutputIterator OutputsEnd() { return OutputIterator::create(mOutputs.end()); }
	// InputIterator InputsBegin() { return InputIterator::create(mInputs.begin()); }
	// InputIterator InputsEnd() { return InputIterator::create(mInputs.end()); }

	_InputPtr getInput(const std::string name)
	{
		for (_InputPtr input : mInputs)
		{
			if (input && input->name() == name) return input;
		}
		return _InputPtr();
	}

	_OutputPtr getOutput(const std::string name)
	{
		for (_OutputPtr output : mOutputs)
		{
			if (output && output->name() == name) return output;
		}
		return _OutputPtr();
	}

	friend class NodeGraph;
};



} // namespace btrack::nodes::system
#endif // __NODE_H__