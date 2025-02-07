#ifndef __METAOUTPUT_H__
#define __METAOUTPUT_H__


#include <vector>
#include <iterator>
#include "nodes/system/MetaNodeIO.h"
#include "nodes/system/MetaInput.h"
#include "nodes/system/Output.h"
#include <boost/type_traits.hpp>

namespace btrack::nodes::system {



class _MetaOutput : public MetaNodeIO
{
public:
	using _OutputType = _Output;
    using _OutputPtr = borrowed_ptr_p<_Output>;

	using _MetaInputType = _MetaInput;
    using _MetaInputPtr = borrowed_ptr_p<_MetaInput>;
protected:
	_MetaOutput(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			_MetaOutput::MetaNodeIO(_name, _nodeType | NodeItemType::OUTPUT, _friendlyName, _description) {}
	

public:
	
	
	virtual _OutputPtr _OutputAt(int index) = 0;
	virtual _OutputPtr _OutputAt(int index) const = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) = 0;
	virtual _MetaInputPtr _MetaInputAt(int index) const = 0;

	bool canConnectTo(const std::shared_ptr<_MetaInput> other) const {
		return convertibleTo(other->dataType()) && 
			(
				(isArray() && other->isArray()) ||
				isValue()
			);
	}
	virtual bool connectTo(std::shared_ptr<_MetaInput> other) = 0;
	virtual bool disconnectFrom(std::shared_ptr<_MetaInput> other) = 0;

	virtual void attach(std::shared_ptr<_Output> input) = 0;
	virtual void detach(std::shared_ptr<_Output> input) = 0;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class MetaOutput : public _MetaOutput
{
protected:
	MetaOutput(
		const std::string_view& _name, 
		const NodeItemType& _nodeType,
		const std::string_view& _friendlyName = "",
		const std::string_view& _description = ""
		) : 
			MetaOutput::_MetaOutput(_name, _nodeType, _friendlyName, _description) {}
	
public:
	using _OutputPtr = typename _MetaOutput::_OutputPtr;
	using _MetaInputPtr = typename _MetaOutput::_MetaInputPtr;
	using OutputType = Output<T, I>;
    using OutputPtr = borrowed_ptr_p<Output<T, I>>;

	using MetaInputType = MetaInput<T, I>;
    using MetaInputPtr = borrowed_ptr_p<MetaInput<T, I>>;

	constexpr const std::type_info& dataType() const override { return typeid(T); }
	virtual void addSender(std::shared_ptr<Sender<T, I>> sender) = 0;
	virtual void removeSender(std::shared_ptr<Sender<T, I>> sender) = 0;
	virtual void broadcast(typename I::readonlyRef value) = 0;

	NodeAtVirtual(Output);
	NodeAtVirtual(MetaInput);
};
} // namespace btrack::nodes::system
#endif // __METAOUTPUT_H__