#ifndef __INPUT_H__
#define __INPUT_H__

#include "nodes/system/Port.h"
#include "memory_backend.h"

namespace btrack::nodes::system {

template <typename ..._Types>
class Input : public Port<..._Types>, public enable_shared_from_this<Input>
{
private:
    weak_ptr<Port<..._Types>> mSource;
    VariantBase<..._Types> mDefault;
public:
    const VariantBase<..._Types>& get() const override;
    VariantBase<..._Types>& get() override;
    Type type() const override { return Port<..._Types>::INPUT; }
    bool connect(std::shared_ptr<Port<..._Types>> other) override;
    bool disconnect(std::shared_ptr<Port<..._Types>> other) override;

    shared_ptr<Input> getPtr() { return this->shared_from_this(); }
};

template <typename... _Types>
inline const VariantBase<... _Types> &Input<_Types...>::get() const
{
    if (mSource.expired() || !mSource.lock()) return mDefault;
    return mSource.lock()->get();
}

template <typename... _Types>
inline VariantBase<... _Types> &Input<_Types...>::get()
{
    if (mSource.expired() || !mSource.lock()) return mDefault;
    return mSource.lock()->get();
}
template <typename... _Types>
inline bool Input<_Types...>::connect(std::shared_ptr<Port<... _Types>> other)
{
    if (!other) return false;
    if (!mSource.expired() && mSource.lock())
    {
        mSource->disconnect(getPtr());
    }
    return other->connect(getPtr());
}
template <typename... _Types>
inline bool Input<_Types...>::disconnect(std::shared_ptr<Port<... _Types>> other)
{
    if (!other) return false;
    if (!mSource.expired() && mSource.lock())
    {
        if (mSource.lock() == other) return mSource.lock()->disconnect(getPtr());
    }
    return false;
}
}

#endif // __INPUT_H__