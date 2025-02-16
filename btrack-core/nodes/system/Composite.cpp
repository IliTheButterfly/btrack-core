#include "Composite.h"

namespace btrack::nodes::system {
void Composite::update()
{
    CompositeIterator<Item> it = this->recursiveIterator(0);
    while(it.hasNext())
    {
        auto item = it.next();
        if (!item) continue;
        item->update();
    }
}

Item *Composite::relativeAt(const ID_t &_id)
{
    if (_id.id.size() == 0) return nullptr;
    if (_id.id.size() == 1) return at(_id.index(), _id.isPort());
    if (Composite* comp = dynamic_cast<Composite*>(at(_id.id.at(0))))
    {
        return comp->relativeAt(ID_t(_id.type, ID_v(_id.id.begin() + 1, _id.id.end())));
    }
    return nullptr;
}

const Item *Composite::relativeAt(const ID_t &_id) const
{
    if (_id.id.size() == 0) return nullptr;
    if (_id.id.size() == 1) return at(_id.index(), _id.isPort());
    if (const Composite* comp = dynamic_cast<const Composite*>(at(_id.id.at(0))))
    {
        return comp->relativeAt(ID_t(_id.type, ID_v(_id.id.begin() + 1, _id.id.end())));
    }
    return nullptr;
}

CompositeIterator<Item> Composite::recursiveIterator(int depth)
{
    return CompositeIterator<Item>(this, depth);
}

CompositeIterator<const Item> Composite::recursiveIterator(int depth) const
{
    return CompositeIterator<const Item>(this, depth);
}
}