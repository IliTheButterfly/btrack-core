#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__


#include <exception>

namespace btrack::nodes {

class ImproperGraphManagementError : public std::exception
{
public:
	const char * what() const throw()
	{
		return "Node was released withought an observation.";
	}
};

}

#endif // __EXCEPTIONS_H__