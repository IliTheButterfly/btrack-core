#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__


#include <exception>

class UndefinedIO : public std::exception
{
public:
	const char * what() const throw()
	{
		return "IO was not added to the list!\n";  
	}
};

#endif // __EXCEPTIONS_H__