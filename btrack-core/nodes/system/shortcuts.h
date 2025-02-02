#ifndef __SHORTCUTS_H__
#define __SHORTCUTS_H__


#include "nodes/system/nodes.h"
#include <boost/type_traits.hpp>
#include <concepts>

namespace btrack::nodes::system {

template <std::derived_from<Node> T>
class AutoMetaNode : public MetaNode
{
	BOOST_STATIC_ASSERT((boost::is_base_of<Node, T>::value));
protected:
public:
	AutoMetaNode(
		const std::string_view& _name, 
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) :
			AutoMetaNode::MetaNode(_name, _friendlyName, _description)
	{
		T dummy = T(std::string{_name} + "_dummy", std::string{_friendlyName} + "_dummy");
	}

};

} // namespace btrack::nodes::system

#endif // __SHORTCUTS_H__