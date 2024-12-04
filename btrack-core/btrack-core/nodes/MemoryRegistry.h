#pragma once
#include <memory>
#include "nodes/type_traits.h"
#include <boost/any.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/concurrent_queues/sync_deque.hpp>
#include <opencv2/core.hpp>

namespace btrack { namespace nodes {


// template <typename T>
// class _Container;

// template <typename T>
// class _Container_fundamental
// {
// public:
// 	typedef T inner;
// private:
// 	inner mValue;

// public:
// 	_Container_fundamental<T>& operator=(const T& );

// };

// template <> class _Container<bool> : public _Container_fundamental<bool> {};
// template <> class _Container<int8_t> : public _Container_fundamental<int8_t> {};
// template <> class _Container<uint8_t> : public _Container_fundamental<uint8_t> {};
// template <> class _Container<int16_t> : public _Container_fundamental<int16_t> {};
// template <> class _Container<uint16_t> : public _Container_fundamental<uint16_t> {};
// template <> class _Container<int32_t> : public _Container_fundamental<int32_t> {};
// template <> class _Container<uint32_t> : public _Container_fundamental<uint32_t> {};
// template <> class _Container<int64_t> : public _Container_fundamental<int64_t> {};
// template <> class _Container<uint64_t> : public _Container_fundamental<uint64_t> {};
// template <> class _Container<float> : public _Container_fundamental<float> {};
// template <> class _Container<double> : public _Container_fundamental<double> {};

// template <> template <typename T> class _Container<T> : public _Container<type_traits::remove_all_ext_and_container_t<T>>{};

template <typename T, typename R>
class _Register
{
public:

public:

	virtual void push(T v) = 0;
	virtual R pull() = 0;

	virtual _Register<T, R>& operator>>(_Register<T, R>& other) = 0;
};


template <typename T, typename R>
class _SimpleRegister : public _Register<T, R>
{
public:
};


template <>
class _SimpleRegister<cv::InputArray, cv::OutputArray> : public _Register<cv::InputArray, cv::OutputArray>
{
public:


};

template <>
class _SimpleRegister<int, int> : public _Register<int, int>
{
private:
	int mValue;
public:
	void push(int v) override { mValue = v; }
	int pull() override { return mValue; }

	_Register<int, int>& operator>>(_Register<int, int>& other) override { other.push(mValue); return *this; }
};


template <typename T>
using FundamentalSimpleRegister = boost::enable_if_t<boost::is_fundamental<T>::value, _SimpleRegister<T, T>>;




}} // btrack::nodes