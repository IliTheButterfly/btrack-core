#ifndef __INPUT_H__
#define __INPUT_H__

#include "nodes/system/Port.h"
#include "memory_backend.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class Input : public Port<VariantType>
{
private:
    PortBase<VariantType>* mSource = nullptr;
    VariantType mDefault;
    volatile bool mConnecting = false;
public:
    Input() = default;
    Input(NodeBase<VariantType>* _parent, const ID_e& _id, const std::string& _name, const std::string& _description = "", VariantType _default = VariantType())
        : Input::Port(_parent, _id, _name, _description), mDefault(_default) {}
    const VariantType& get() const override;
    VariantType& get() override;
    Item* createClone() const override;
    size_t connectionCount() const override { return mSource ? 1 : 0; }
    const PortBase<VariantType>* connectionAt(const ID_e& _id) const override { return _id != 0 ? nullptr : mSource; }
    PortType type() const override { return PortType::INPUT; }
    ConnectionResult connect(PortBase<VariantType>* other) override;
    ConnectionResult disconnect(PortBase<VariantType>* other) override;

    virtual ~Input();
};

template <VariantTemplate VariantType>
inline const VariantType &Input<VariantType>::get() const
{
    if (!mSource) return mDefault;
    return mSource->get();
}

template <VariantTemplate VariantType>
inline VariantType &Input<VariantType>::get()
{
    if (!mSource) return mDefault;
    return mSource->get();
}
template <VariantTemplate VariantType>
inline ConnectionResult Input<VariantType>::connect(PortBase<VariantType>* other)
{
    if (mConnecting) return ConnectionResult::OTHER;
    Recursion r(mConnecting);
    if (!other) return ConnectionResult::NULL_POINTER;
    if (mSource == other) return ConnectionResult::ALREADY_CONNECTED;
    if (mSource)
    {
        if (auto r = mSource->disconnect(this); r != ConnectionResult::SUCCESS) return r;
        mSource = nullptr;
    }
    if (auto rr = other->connect(this); rr == ConnectionResult::SUCCESS || rr == ConnectionResult::ALREADY_CONNECTED || rr == ConnectionResult::OTHER)
    {
        mSource = other;
        return ConnectionResult::SUCCESS;
    }
    else return rr;
    return ConnectionResult::UNHANDLED;
}
template <VariantTemplate VariantType>
inline ConnectionResult Input<VariantType>::disconnect(PortBase<VariantType>* other)
{
    if (!other) return ConnectionResult::NULL_POINTER;
    if (mSource == other)
    {
        if (auto r = mSource->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED)
        {
            mSource = nullptr;
            return ConnectionResult::SUCCESS;
        }
        else return r;
    }
    return ConnectionResult::NOT_CONNECTED;
}
template <VariantTemplate VariantType>
inline Item* Input<VariantType>::createClone() const
{
    auto res = new Input<VariantType>();
    Port<VariantType>::clone(res);
    return res;
}
template <VariantTemplate VariantType>
inline Input<VariantType>::~Input()
{
    if (mSource)
    {
        mSource->disconnect(this);
        mSource = nullptr;
    }
}
}

#endif // __INPUT_H__