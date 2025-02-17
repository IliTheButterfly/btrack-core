#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "nodes/system/Port.h"
#include "memory_backend.h"
#include <boost/container/vector.hpp>

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class Output : public Port<VariantType>
{
private:
    boost::container::vector<PortBase<VariantType>*> mDestinations;
    VariantType mValue;
public:
    Output(NodeBase<VariantType>* _parent, const ID_t& _id, const std::string& _name, const std::string& _description = "", VariantType _default = VariantType())
        : Output::Port(_parent, _id, _name, _description), mValue(_default) {}
    const VariantType& get() const override;
    VariantType& get() override;
    size_t connectionCount() const override { return mDestinations.size(); }
    PortType type() const override { return PortType::OUTPUT; }
    ConnectionResult connect(PortBase<VariantType>* other) override;
    ConnectionResult disconnect(PortBase<VariantType>* other) override;
    const boost::container::vector<PortBase<VariantType>*>& connections() const { return this->mDestinations; }
    virtual ~Output();
};

template <VariantTemplate VariantType>
inline const VariantType &Output<VariantType>::get() const
{
    return mValue;
}

template <VariantTemplate VariantType>
inline VariantType &Output<VariantType>::get()
{
    return mValue;
}

template <VariantTemplate VariantType>
inline ConnectionResult Output<VariantType>::connect(PortBase<VariantType> *other)
{
    if (!other) return ConnectionResult::NULL_POINTER;
    if (std::find(mDestinations.begin(), mDestinations.end(), other) != mDestinations.end()) return ConnectionResult::ALREADY_CONNECTED;
    mDestinations.emplace_back(other);
    if (auto r = other->connect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::ALREADY_CONNECTED)
    {
        return ConnectionResult::SUCCESS;
    }
    else
    {
        mDestinations.pop_back();
        return r;
    }
    return ConnectionResult::UNHANDLED;
}

template <VariantTemplate VariantType>
inline ConnectionResult Output<VariantType>::disconnect(PortBase<VariantType> *other)
{
    if (!other) return ConnectionResult::NULL_POINTER;
    if (auto it = std::find(mDestinations.begin(), mDestinations.end(), other); it == mDestinations.end()) return ConnectionResult::NOT_CONNECTED;
    else
    {
        mDestinations.erase(it);
        if (auto r = other->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED) return ConnectionResult::SUCCESS;
        else return r;
    }
    return ConnectionResult::UNHANDLED;
}

template <VariantTemplate VariantType>
inline Output<VariantType>::~Output()
{
    while (mDestinations.size())
    {
        auto v = mDestinations.at(0);
        if (v) v->disconnect(this);
        

        if (mDestinations.size() && v == mDestinations.at(0))
        {
            // Ensure the value was removed
            mDestinations.erase(mDestinations.begin());
        }
    }
}

} // namespace btrack::nodes::system
#endif // __OUTPUT_H__