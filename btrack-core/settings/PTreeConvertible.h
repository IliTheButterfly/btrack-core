#ifndef __PTREECONVERTIBLE_H__
#define __PTREECONVERTIBLE_H__


#include <boost/property_tree/ptree.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/quaternion.hpp>

namespace btrack::settings {

class PTreeConvertible
{
public:
    virtual void to_ptree(boost::property_tree::ptree& pt) = 0;
    virtual void from_ptree(boost::property_tree::ptree& pt) = 0;
};

template<class T>
void convert_to_ptree(boost::property_tree::ptree& pt, T& v);

template<class T>
void convert_from_ptree(boost::property_tree::ptree& pt, T& v);

template<>
inline void convert_to_ptree(boost::property_tree::ptree& pt, PTreeConvertible& v)
{
    v.to_ptree(pt);
}

template<>
inline void convert_from_ptree(boost::property_tree::ptree& pt, PTreeConvertible& v)
{
    v.from_ptree(pt);
}

template<class,class V>
inline void convert_to_ptree(boost::property_tree::ptree& pt, cv::Size_<V>& v)
{
    pt.add("width", v.width);
    pt.add("height", v.height);
}

template<class V>
inline void convert_from_ptree(boost::property_tree::ptree& pt, cv::Size_<V>& v)
{
    v.width = pt.get<V>("width");
    v.height = pt.get<V>("height");
}

template<class,class V>
inline void convert_to_ptree(boost::property_tree::ptree& pt, cv::Quat<V>& v)
{
    pt.add("w", v.w);
    pt.add("x", v.x);
    pt.add("y", v.y);
    pt.add("z", v.z);
}

template<class,class V>
inline void convert_from_ptree(boost::property_tree::ptree& pt, cv::Quat<V>& v)
{
    v.w = pt.get<V>("w");
    v.x = pt.get<V>("x");
    v.y = pt.get<V>("y");
    v.z = pt.get<V>("z");
}

template<class,class V>
inline void convert_to_ptree(boost::property_tree::ptree& pt, cv::Point3_<V>& v)
{
    pt.add("x", v.x);
    pt.add("y", v.y);
    pt.add("z", v.z);
}

template<class,class V>
inline void convert_from_ptree(boost::property_tree::ptree& pt, cv::Point3_<V>& v)
{
    v.x = pt.get<V>("x");
    v.y = pt.get<V>("y");
    v.z = pt.get<V>("z");
}

template<class,class V>
inline void convert_to_ptree(boost::property_tree::ptree& pt, cv::Point_<V>& v)
{
    pt.add("x", v.x);
    pt.add("y", v.y);
}

template<class,class V>
inline void convert_from_ptree(boost::property_tree::ptree& pt, cv::Point_<V>& v)
{
    v.x = pt.get<V>("x");
    v.y = pt.get<V>("y");
}

template <class T>
inline void convert_to_ptree(boost::property_tree::ptree &pt, T &v)
{
}

template <class T>
inline void convert_from_ptree(boost::property_tree::ptree &pt, T &v)
{
}

} // btrack::settings
#endif // __PTREECONVERTIBLE_H__