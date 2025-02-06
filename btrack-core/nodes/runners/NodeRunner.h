#ifndef __NODERUNNER_H__
#define __NODERUNNER_H__

#include <memory>

namespace btrack::nodes::runners {

class _NodeItem;

class NodeRunner
{
public:
	/**
	 * @brief Call when the node layout has changed
	 */
	virtual void update() = 0;

	/**
	 * @brief Executes the node layout
	 */
	virtual void run() = 0;

	virtual void addItem(std::shared_ptr<_NodeItem> node) = 0;
	virtual void removeItem(std::shared_ptr<_NodeItem> node) = 0;

	virtual void addConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) = 0;
	virtual void removeConnection(std::shared_ptr<_NodeItem> from, std::shared_ptr<_NodeItem> to) = 0;
};

}

#endif // __NODERUNNER_H__