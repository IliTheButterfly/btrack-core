#pragma once
#include <exception>

class UndefinedIO : public std::exception
{
public:
	const char * what() const throw()
	{
		return "IO was not added to the list!\n";  
	}
};
