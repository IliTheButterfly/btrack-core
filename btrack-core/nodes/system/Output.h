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
    volatile bool mConnecting = false;
public:
    Output() = default;
    Output(NodeBase<VariantType>* _parent, const ID_e& _id, const std::string& _name, const std::string& _description = "", VariantType _default = VariantType())
        : Output::Port(_parent, _id, _name, _description), mValue(_default) {}
    const VariantType& get() const override;
    VariantType& get() override;
    size_t connectionCount() const override { return mDestinations.size(); }
    const PortBase<VariantType>* connectionAt(const ID_e& _id) const override { return _id >= mDestinations.size() ? nullptr : mDestinations.at(_id); }
    Item* createClone() const override;
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
inline Item* Output<VariantType>::createClone() const
{
    auto res = new Output<VariantType>();
    Port<VariantType>::clone(res);
    return res;
}

template <VariantTemplate VariantType>
inline ConnectionResult Output<VariantType>::connect(PortBase<VariantType> *other)
{
    if (mConnecting) return ConnectionResult::OTHER;
    Recursion r(mConnecting);
    if (!other) return ConnectionResult::NULL_POINTER;
    if (other->type() == PortType::INPUT)
    {
        if (auto r = other->connect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::OTHER)
        {
            mDestinations.emplace_back(other);
            return ConnectionResult::SUCCESS;
        }
        else return r;
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