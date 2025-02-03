#ifndef __NEGATE_H__
#define __NEGATE_H__


#include "nodes/utilities/math/UnaryOperation.h"

namespace btrack::nodes::utilities::math {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Negate : public UnaryOperation<T, T, I, I>
{
public:
	inline static const std::string defaultFriendlyName = "Negate";
	inline static const std::string description = "Negates the input value";

	Negate(const std::string_view& _name, const std::string_view& _friendlyName = defaultFriendlyName)
		: Negate::UnaryOperation(_name, _friendlyName, description)
		{ }
	void process() override
	{
		typename I::item val;
		*(this->Parameter) >> val;
		*(this->Result) << -val;
	}
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaNegate : public MetaUnaryOperation<T, T, I, I>
{
public:
	inline static const std::string defaultFriendlyName = "Negate";
	inline static const std::string description = "Negates the input value";

	MetaNegate(const std::string_view& _name, const std::string_view& _friendlyName = defaultFriendlyName)
		: MetaNegate::MetaUnaryOperation(_name, _friendlyName, description)
		{ }
	
	void generate(int count) override
	{
		for (int i = 0; i < count; ++i)
		{
			auto node = std::make_shared<Negate<T, I>>(this->name());
			for (int ii = 0; ii < this->inputCount(); ++ii)
			{
				this->_MetaInputAt(ii)->attach(node->_InputAt(ii));
			}
			for (int ii = 0; ii < this->outputCount(); ++ii)
			{
				this->_MetaOutputAt(ii)->attach(node->_OutputAt(ii));
			}
		}
	}
	
};



} // namespace btrack::nodes::utilities::math
#endif // __NEGATE_H__