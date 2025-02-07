#ifndef __NEGATE_H__
#define __NEGATE_H__


#include "nodes/utilities/math/UnaryOperation.h"
#include "nodes/metadata.h"
#include <typeinfo>


namespace btrack::nodes::utilities::math {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Negate : public UnaryOperation<T, T, I, I>
{
public:
	inline static const std::string defaultFriendlyName = "Negate";
	inline static const std::string description = "Negates the input value";

	Negate(std::shared_ptr<NodeRunner> runner, const std::string_view& _name = defaultFriendlyName, const std::string_view& _friendlyName = defaultFriendlyName)
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

	static const std::string_view Name() { return defaultFriendlyName; }

	MetaNegate(std::shared_ptr<NodeRunner> runner, const std::string_view& _name = defaultFriendlyName, const std::string_view& _friendlyName = defaultFriendlyName)
		: MetaNegate::MetaUnaryOperation(_name, _friendlyName, description)
		{ }
	
	void generate(int count) override
	{
		for (int i = 0; i < count; ++i)
		{
			auto node = std::make_shared<Negate<T, I>>(this, this->name());
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

BEGIN_METADATA(
	Negate,
	"Negate",
	"",
	btrack::nodes::utilities::math::Negate<int>,
	btrack::nodes::utilities::math::MetaNegate<int>
)
INPUT_VALUE_META(Parameter, "Parameter", "", int)
END_METADATA

#endif // __NEGATE_H__