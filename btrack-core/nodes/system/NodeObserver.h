#ifndef __NODEOBSERVER_H__
#define __NODEOBSERVER_H__

#include "nodes/system/type_traits.h"

namespace btrack::nodes::system {

using namespace type_traits::ownership;

class _NodeItem;

class NodeObserver
{
public:
	virtual void update() = 0;

	virtual void addItem(std::shared_ptr<_NodeItem> node) {}
	virtual void removeItem(std::shared_ptr<_NodeItem> node) {}

	virtual void addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) {}
	virtual void removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) {}

	virtual ~NodeObserver() = default;
};

}

#endif // __NODEOBSERVER_H__