#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "type_traits.h"

namespace btrack::memory {

template <typename T>
struct owned_ptr {
    typedef typename container_traits<T>::type element_type;
    typedef shared_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using owned_ptr_p = typename owned_ptr<T>::ptr_type;

template <typename T>
struct borrowed_ptr {
    typedef typename container_traits<T>::type element_type;
    typedef weak_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using borrowed_ptr_p = typename borrowed_ptr<T>::ptr_type;

template <typename T>
struct ref_ptr {
    typedef typename container_traits<T>::type element_type;
    typedef shared_ptr<typename container_traits<T>::type> ptr_type;
};

template <typename T>
using ref_ptr_p = typename ref_ptr<T>::ptr_type;


}

#endif // __MEMORY_H__