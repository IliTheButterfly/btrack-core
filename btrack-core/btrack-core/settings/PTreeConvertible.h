#pragma once
#include <boost/property_tree/ptree.hpp>

class PTreeConvertible
{
public:
	virtual void to_ptree(boost::property_tree::ptree& pt) const = 0;
	virtual void from_ptree(const boost::property_tree::ptree& pt) = 0;
};

