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

} // namespace btrack::nodes::utilities::math
#endif // __NEGATE_H__