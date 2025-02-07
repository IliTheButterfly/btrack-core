#ifndef __INPUT_H__
#define __INPUT_H__

#include "nodes/system/NodeIO.h"
#include <vector>
#include <memory>

namespace btrack::nodes::system {

class _Input : public NodeIO
{
protected:
	_Input(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_Input::NodeIO(_name, _nodeType | NodeItemType::INPUT, _friendlyName, _description) {}
public:
	virtual ~_Input() = default;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Input : public _Input
{
protected:
	std::shared_ptr<Channel<T, I>> mChannel = std::make_shared<Channel<T, I>>();
	Input(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			Input::_Input(_name, _nodeType, _friendlyName, _description) {}
public:
	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual ~Input() = default;
};

} // namespace btrack::nodes::system
#endif // __INPUT_H__