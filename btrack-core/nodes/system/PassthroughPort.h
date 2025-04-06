#ifndef __PASSTHROUGHPORT_H__
#define __PASSTHROUGHPORT_H__

#include "nodes/system/Port.h"
#include "memory_backend.h"

namespace btrack::nodes::system {

template <VariantTemplate VariantType>
class PassthroughPort : public Port<VariantType>
{
private:
    PortBase<VariantType>* mSource = nullptr;
    boost::container::vector<PortBase<VariantType>*> mDestinations;
    VariantType mDefault{};
    volatile bool mConnecting = false;
    PortType mType;
    PassthroughType mPassthroughType{};
public:
    PassthroughPort() : PassthroughPort::Port() { }
    PassthroughPort(NodeBase<VariantType>* _parent, const ID_e& _id, PortType _type, PassthroughType _passthroughType, const std::string& _name, const std::string& _description = "", VariantType _default = VariantType())
        : PassthroughPort::Port(_parent, _id, _name, _description), mDefault(_default), mType(_type), mPassthroughType(_passthroughType) { }
    const VariantType& get() const override;
    VariantType& get() override;
    Item* createClone() const override;
    const PortBase<VariantType>* _source() const { return mSource; }
    const PortBase<VariantType>* _destination(size_t index) const 
    {
        if (index >= mDestinations.size()) return nullptr;
        return mDestinations.at(index);
    }
    size_t connectionCount() const override { return (mSource ? 1 : 0) + mDestinations.size(); }
    const PortBase<VariantType>* connectionAt(const ID_e& _id) const override {
        return _id == mDestinations.size() ?
            mSource :
            _id > mDestinations.size() ?
                nullptr :
                mDestinations.at(_id);
    }
    PortType type() const override { return mType; }
    bool isPassthrough() const override { return true; }
    ConnectionResult connect(PortBase<VariantType>* other) override;
    ConnectionResult disconnect(PortBase<VariantType>* other) override;

    virtual ~PassthroughPort();
};

template <VariantTemplate VariantType>
inline const VariantType &PassthroughPort<VariantType>::get() const
{
    if (!mSource) return mDefault;
    return mSource->get();
}

template <VariantTemplate VariantType>
inline VariantType &PassthroughPort<VariantType>::get()
{
    if (!mSource) return mDefault;
    return mSource->get();
}
template <VariantTemplate VariantType>
inline ConnectionResult PassthroughPort<VariantType>::connect(PortBase<VariantType>* other)
{
    if (mConnecting) return ConnectionResult::OTHER;
    Recursion r(mConnecting);
    if (!other) return ConnectionResult::NULL_POINTER;
    PortType otherPortType = other->type();
    if (otherPortType == PortType::UNKNOWN) return ConnectionResult::UNHANDLED;

    // We act as an input in 2 cases:
    // Either, we are acting like a normal port (output->input)
    // Either, we are acting as a passthrough in the case other->connect(this) (right hand side) so we act as an input
    if (otherPortType == this->type() ? otherPortType == PortType::OUTPUT && !other->isPassthrough() : otherPortType == PortType::OUTPUT) // Acting as an input
    {
        if (mSource == other) return ConnectionResult::ALREADY_CONNECTED;
        if (mSource)
        {
            if (auto r = mSource->disconnect(this); r != ConnectionResult::SUCCESS) return r;
            mSource = nullptr;
        }
        mSource = other;
        if (auto rr = other->connect(this); rr == ConnectionResult::SUCCESS || rr == ConnectionResult::ALREADY_CONNECTED || rr == ConnectionResult::OTHER)
        {
            return ConnectionResult::SUCCESS;
        }
        else
        {
            mSource = nullptr;
            return rr;
        }
    }
    // We act as an output in 2 cases:
    // Either, we are acting like a normal port (output->input)
    // Either, we are acting as a passthrough in the case this->connect(other) (left hand side) so we act as an output
    if (otherPortType == this->type() ? otherPortType == PortType::INPUT || other->isPassthrough() : otherPortType == PortType::INPUT) // Acting as an output
    {
        mDestinations.emplace_back(other);
        if (other->isPassthrough())
        {
            PassthroughPort<VariantType>* p = dynamic_cast<PassthroughPort<VariantType>*>(other);
            if (!p) return ConnectionResult::INCOMPATIBLE;
            if (p->mSource)
            {
                p->mSource->disconnect(p);
            }
            p->mSource = this;
            return ConnectionResult::SUCCESS;
        }
        else
        {
            if (auto r = other->connect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::OTHER)
            {
                if (otherPortType == this->type()) std::cout << "Passthrough";
                else std::cout << "Connection";
                std::cout << ":{" << this->parent()->name() << '(' << this->parent()->id() << ")." << this->name() << '(' << this->id() << ")}->{"
                << other->parent()->name() << '(' << other->parent()->id() << ")." << other->name() << '(' << other->id() << ")}" << std::endl;
                return ConnectionResult::SUCCESS;
            }
            else 
            {
                mDestinations.pop_back();
                return r;
            }
        }
    }
    return ConnectionResult::UNHANDLED;
}
template <VariantTemplate VariantType>
inline ConnectionResult PassthroughPort<VariantType>::disconnect(PortBase<VariantType>* other)
{
    if (!other) return ConnectionResult::NULL_POINTER;
    PortType otherPortType = other->type();
    if (otherPortType == PortType::UNKNOWN) return ConnectionResult::UNHANDLED;
    if (mSource == other)
    {
        mSource = nullptr;
        if (auto r = other->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED)
        {
            return ConnectionResult::SUCCESS;
        }
        else 
        {
            mSource = other;
            return r;
        }
    }
    else
    {
        if (auto it = std::find(mDestinations.begin(), mDestinations.end(), other); it == mDestinations.end()) return ConnectionResult::NOT_CONNECTED;
        else
        {
            mDestinations.erase(it);
            // std::cout << "Disconnection:{" << this->parent()->name() << '(' << this->parent()->id() << ")." << this->name() << '(' << this->id() << ")}->{"
            // << other->parent()->name() << '(' << other->parent()->id() << ")." << other->name() << '(' << other->id() << ")}" << std::endl;
            if (auto r = other->disconnect(this); r == ConnectionResult::SUCCESS || r == ConnectionResult::NOT_CONNECTED) return ConnectionResult::SUCCESS;
            else return r;
        }
    }
    return ConnectionResult::NOT_CONNECTED;
}
template <VariantTemplate VariantType>
inline Item* PassthroughPort<VariantType>::createClone() const
{
    auto res = new PassthroughPort<VariantType>();
    Port<VariantType>::clone(res);
    return res;
}
template <VariantTemplate VariantType>
inline PassthroughPort<VariantType>::~PassthroughPort()
{
    if (mSource)
    {
        mSource->disconnect(this);
        mSource = nullptr;
    }
    for (auto o : mDestinations)
    {
        o->disconnect(this);
    }
}
}

#endif // __PASSTHROUGHPORT_H__