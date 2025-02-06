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
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto input = InputValue<T, I>(asShared<NodeRunner>(), _name, _friendlyName, _description).asShared<InputValue<T, I>>();
		mRunner->addItem(input);
		mInputs.push_back(input);
		return input;
	}

	template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
	std::shared_ptr<OutputValue<T, I>> addOutputValue(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "")
	{
		auto output = OutputValue<T, I>(asShared<NodeRunner>(), _name, _friendlyName, _description).asShared<OutputValue<T, I>>(); 
		mRunner->addItem(output);
		mOutputs.push_back(output);
		return output;
	}

public:
	virtual void process() = 0;

	Node(
		std::shared_ptr<NodeRunner> runner,
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Node::_Node(runner, _name, NodeItemType::NODE, _friendlyName, _description) {}


	NodeAtConcrete(_Input, mInputs)
	NodeAtConcrete(_Output, mOutputs)

	// OutputIterator OutputsBegin() { return OutputIterator::create(mOutputs.begin()); }
	// OutputIterator OutputsEnd() { return OutputIterator::create(mOutputs.end()); }
	// InputIterator InputsBegin() { return InputIterator::create(mInputs.begin()); }
	// InputIterator InputsEnd() { return InputIterator::create(mInputs.end()); }

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
		while (mInputs.size())
		{
			mRunner->removeItem(mInputs[0]);
		}
		while (mOutputs.size())
		{
			mRunner->removeItem(mOutputs[0]);
		}
	}
};

} // namespace btrack::nodes::system
#endif // __NODE_H__