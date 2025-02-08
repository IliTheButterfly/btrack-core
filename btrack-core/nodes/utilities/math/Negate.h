#ifndef __NEGATE_H__
#define __NEGATE_H__


#include "nodes/utilities/math/UnaryOperation.h"
#include "nodes/metadata.h"
#include <typeinfo>


namespace btrack::nodes::utilities::math {

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Negate : public UnaryOperation<T, T, I, I>
{
protected:
	struct Protected { explicit Protected() = default; };
public:
	Negate(
		Protected _,
		const std::string_view& _name,
		const std::string_view& _friendlyName = defaultFriendlyName
		) :
			Negate::UnaryOperation(_name, _friendlyName, description) { }
	static std::shared_ptr<Negate> create(std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name,
		const std::string_view& _friendlyName = defaultFriendlyName)
	{
		auto res = std::make_shared<Negate>(Protected(), _name, _friendlyName);
		res->init();
		res->mObserver = _observer;
		return res;
	}

	inline static const std::string defaultFriendlyName = "Negate";
	inline static const std::string description = "Negates the input value";

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
protected:
	struct Protected { explicit Protected() = default; };
public:
	inline static const std::string defaultFriendlyName = "Negate";
	inline static const std::string description = "Negates the input value";

	static const std::string_view Name() { return defaultFriendlyName; }

	MetaNegate(
		Protected _,
		const std::string_view& _name,
		const std::string_view& _friendlyName = defaultFriendlyName
		) :
			MetaNegate::MetaUnaryOperation(_name, _friendlyName, description) { }
	
	static std::shared_ptr<MetaNegate> create(
		std::shared_ptr<NodeObserver> _observer,
		const std::string_view& _name,
		const std::string_view& _friendlyName = defaultFriendlyName)
	{
		auto res = std::make_shared<MetaNegate>(Protected(), _name, _friendlyName);
		res->init();
		res->mObserver = _observer;
		return res;
	}

	void generate(int count) override
	{
		for (int i = 0; i < count; ++i)
		{
			auto node = Negate<T, I>::create(this->asObserver(), this->name());
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