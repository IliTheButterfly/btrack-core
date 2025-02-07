#ifndef __NODERUNNER_H__
#define __NODERUNNER_H__

#include <memory>
#include "nodes/system/NodeObserver.h"

namespace btrack::nodes::runners {

class _NodeItem;

class NodeRunner : public system::NodeObserver
{
public:
	/**
	 * @brief Executes the node layout
	 */
	virtual void run() = 0;
};

}

#endif // __NODERUNNER_H__