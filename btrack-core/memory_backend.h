#ifndef __MEMORY_BACKEND_H__
#define __MEMORY_BACKEND_H__

#include <boost/smart_ptr.hpp>
#include <boost/pointer_cast.hpp>
#include <memory>

namespace btrack {

#define USE_BOOST_MEMORY

#ifdef USE_BOOST_MEMORY

using boost::shared_ptr;
using boost::movelib::unique_ptr;
using boost::weak_ptr;
using boost::enable_shared_from_raw;
using boost::enable_shared_from_this;
using boost::movelib::swap;
using boost::make_shared;
using boost::movelib::make_unique;
using boost::static_pointer_cast;
using boost::dynamic_pointer_cast;
using boost::const_pointer_cast;
using boost::reinterpret_pointer_cast;

#else

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::enable_shared_from_raw;
using std::enable_shared_from_this;
using std::swap;
using std::make_shared;
using std::make_unique;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;
using std::reinterpret_pointer_cast;

#endif // USE_BOOST_MEMORY


}

#endif // __MEMORY_BACKEND_H__