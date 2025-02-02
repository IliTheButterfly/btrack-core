#ifndef __UNARYOPERATION_H__
#define __UNARYOPERATION_H__



#include "nodes/system/nodes.h"

namespace btrack::nodes::utilities::math {

using namespace system;

template <typename In, typename Out, 
	system::ChannelTypeConcept<In> InI = system::DefaultChannelTypeInfo<In>, 
	system::ChannelTypeConcept<Out> OutI = system::DefaultChannelTypeInfo<Out>>
class UnaryOperation : public Node
{
public:
	std::shared_ptr<InputValue<In, InI>> Parameter;
	std::shared_ptr<OutputValue<Out, OutI>> Result;
	
	UnaryOperation(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = "",
		const std::string_view& _inputFriendlyName = "Input",
		const std::string_view& _inputDescription = "The input value",
		const std::string_view& _outputFriendlyName = "Output",
		const std::string_view& _outputDescription = "The result") :
		UnaryOperation::Node(_name, _friendlyName, _description)
	{
		Parameter = this->addInputValue<In, InI>("input", _inputFriendlyName, _inputDescription);
		Result = this->addOutputValue<Out, OutI>("output", _outputFriendlyName, _outputDescription);
	}

	size_t inputCount() const { return 1; }
	size_t outputCount() const { return 1; }
};


} // namespace btrack::nodes::utilities::math

#endif // __UNARYOPERATION_H__