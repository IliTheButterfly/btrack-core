#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/functional.hpp>


// class FooBase
// {
// public:
// 	virtual int id() const = 0;
// };

// class Foo : public FooBase
// {
// protected:
// 	virtual int idImpl() const = 0;
// public:
// 	int id() const override { return idImpl() + 1; }
// };

// class FooConcrete1 : public Foo
// {
// protected:
// 	int idImpl() const override { return 0; }
// };

// class FooConcrete2 : public Foo
// {
// protected:
// 	int idImpl() const override { return 2; }
// };

// template <typename RefT, typename PtrT>
// class _IteratorBase
// {
// public:
// 	virtual void inc() = 0;
// 	virtual bool eq(const _IteratorBase<RefT, PtrT>& other) const { return false; }
// 	virtual bool ne(const _IteratorBase<RefT, PtrT>& other) const { return true; }

// 	virtual const void* innerIterator() const = 0;
// 	virtual RefT operator*() const = 0;
//     virtual PtrT operator->() const = 0;

//     const std::type_info elemType() const { return typeid(RefT); }
//     // virtual const std::type_info iterType() const = 0;

// 	bool operator==(const _IteratorBase<RefT, PtrT>& other) const { std::cout << __PRETTY_FUNCTION__ << std::endl; return eq(other); }
// 	bool operator!=(const _IteratorBase<RefT, PtrT>& other) const { std::cout << __PRETTY_FUNCTION__ << std::endl; return ne(other); }

// 	virtual ~_IteratorBase() = default;
// };


// template <typename From, typename To>
// class _CastIterator : public _IteratorBase<To&, To*>
// {
// 	static_assert(std::convertible_to<From&, To&>, "From needs to be convertible to To");
// public:
//     using UnderlyingIterator = typename std::vector<From>::iterator;
// private:
//     UnderlyingIterator iter;
// public:
// 	const void* innerIterator() const override { return &iter; }
//     using iterator_category = std::forward_iterator_tag;
//     using value_type = To;
//     using difference_type = std::ptrdiff_t;
//     using pointer = To*;
//     using reference = To&;

//     _CastIterator(UnderlyingIterator it) : iter(it) {std::cout << __PRETTY_FUNCTION__ << std::endl;}

//     reference operator*() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return (reference)*iter; }
//     pointer operator->() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return (pointer)(iter.operator->()); }

// 	void inc() override { ++iter; }

//     _CastIterator& operator++()
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         ++iter;
//         return *this;
//     }

//     _CastIterator operator++(int)
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         _CastIterator temp = *this;
//         ++(*this);
//         return temp;
//     }

// 	bool operator==(const _CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iter == other.iter;
// 	}
//     bool operator!=(const _CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iter != other.iter;
// 	}

// 	bool eq(const _IteratorBase<To>& other) const override
// 	{
// 		if (typeid(other) == typeid(_CastIterator)) return *this == reinterpret_cast<const _CastIterator&>(other);
// 		return false;
// 	}

// 	bool ne(const _IteratorBase<To>& other) const override
// 	{
// 		if (typeid(other) == typeid(_CastIterator)) return *this != reinterpret_cast<const _CastIterator&>(other);
// 		return true;
// 	}

//     // const std::type_info iterType() const override { return typeid(UnderlyingIterator); }

// 	~_CastIterator() = default;
// };

// template <typename From, typename To>
// class _CastIterator<std::shared_ptr<From>, std::shared_ptr<To>> : public _IteratorBase<To&, std::shared_ptr<To>>
// {
// 	static_assert(std::convertible_to<From&, To&>, "From needs to be convertible to To");
// public:
//     using UnderlyingIterator = typename std::vector<std::shared_ptr<From>>::iterator;
// private:
//     UnderlyingIterator iter;
// public:
// 	const void* innerIterator() const override { return &iter; }
//     using iterator_category = std::forward_iterator_tag;
//     using value_type = std::shared_ptr<To>;
//     using difference_type = std::ptrdiff_t;
//     using pointer = std::shared_ptr<To>;
//     using reference = To&;

//     _CastIterator(UnderlyingIterator it) : iter(it) { std::cout << __PRETTY_FUNCTION__ << std::endl; }


//     reference operator*() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return std::reinterpret_pointer_cast<To>(*iter); }
//     pointer operator->() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return std::reinterpret_pointer_cast<To>(*iter); }

// 	void inc() override { std::cout << __PRETTY_FUNCTION__ << std::endl; ++iter; }

//     _CastIterator& operator++()
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         ++iter;
//         return *this;
//     }

//     _CastIterator operator++(int)
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         _CastIterator temp = *this;
//         ++(*this);
//         return temp;
//     }

// 	bool operator==(const _CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iter == other.iter;
// 	}
//     bool operator!=(const _CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iter != other.iter;
// 	}


// 	bool eq(const _IteratorBase<std::shared_ptr<To>>& other) const override
// 	{
// 		if (typeid(other) == typeid(_CastIterator)) return *this == reinterpret_cast<const _CastIterator&>(other);
// 		return false;
// 	}

// 	bool ne(const _IteratorBase<std::shared_ptr<To>>& other) const override
// 	{
// 		if (typeid(other) == typeid(_CastIterator)) return *this != reinterpret_cast<const _CastIterator&>(other);
// 		return true;
// 	}
//     // const std::type_info iterType() const override { return typeid(std::declval<typename std::vector<std::shared_ptr<From>>::iterator>()); }

// 	~_CastIterator() = default;
// };

// template <typename T>
// class CastIterator : public _IteratorBase<T&, T*>
// {
// public:
//     using AbstractIterator = _IteratorBase<T&, T*>;
// private:
//     AbstractIterator* iterPtr;
//     // General constructor
//     template <typename IterT>
//     explicit CastIterator(IterT it) : iterPtr(new _CastIterator<typename IterT::value_type, T>(it)) {}
// public:
// 	const void* innerIterator() const override { return iterPtr->innerIterator(); }
//     using iterator_category = std::forward_iterator_tag;
//     using value_type = T;
//     using difference_type = std::ptrdiff_t;
//     using pointer = T*;
//     using reference = T&;


//     // Factory method for explicit creation
//     template <typename IterT>
//     static CastIterator<T> create(IterT it)
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return CastIterator(it);
// 	}

//     // Overloaded operators
//     reference operator*() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return **iterPtr; }
//     pointer operator->() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return iterPtr->operator->(); }

// 	void inc() override { iterPtr->inc(); }

//     CastIterator& operator++()
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         iterPtr->inc();
//         return *this;
//     }

//     CastIterator operator++(int)
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         CastIterator temp = *this;
//         ++(*this);
//         return temp;
//     }

//     bool operator==(const CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iterPtr->operator==(*other.iterPtr);
// 	}
//     bool operator!=(const CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iterPtr->operator!=(*other.iterPtr);
// 	}

// 	bool eq(const _IteratorBase<T&, T*>& other) const override
// 	{
// 		if (typeid(other) == typeid(CastIterator)) return *this == reinterpret_cast<const CastIterator&>(other);
// 		return false;
// 	}

// 	bool ne(const _IteratorBase<T&, T*>& other) const override
// 	{
// 		if (typeid(other) == typeid(CastIterator)) return *this != reinterpret_cast<const CastIterator&>(other);
// 		return true;
// 	}

//     // const std::type_info iterType() const override { return iterPtr->iterType(); }

//     ~CastIterator()
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		delete iterPtr;
// 	}
// };

// template <typename T>
// class CastIterator<std::shared_ptr<T>> : public _IteratorBase<T&, std::shared_ptr<T>>
// {
// public:
//     using AbstractIterator = _IteratorBase<T&, T*>;
// private:
//     AbstractIterator* iterPtr;
//     // General constructor
//     template <typename IterT>
//     explicit CastIterator(IterT it) : iterPtr(new _CastIterator<typename IterT::value_type, T>(it)) {}
// public:
// 	const void* innerIterator() const override { return iterPtr->innerIterator(); }
//     using iterator_category = std::forward_iterator_tag;
//     using value_type = T;
//     using difference_type = std::ptrdiff_t;
//     using pointer = std::shared_ptr<T>;
//     using reference = T&;


//     // Factory method for explicit creation
//     template <typename IterT>
//     static CastIterator<T> create(IterT it)
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return CastIterator(it);
// 	}

//     // Overloaded operators
//     reference operator*() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return **iterPtr; }
//     pointer operator->() const override { std::cout << __PRETTY_FUNCTION__ << std::endl; return iterPtr->operator->(); }

// 	void inc() override { iterPtr->inc(); }

//     CastIterator& operator++()
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         iterPtr->inc();
//         return *this;
//     }

//     CastIterator operator++(int)
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         CastIterator temp = *this;
//         ++(*this);
//         return temp;
//     }

//     bool operator==(const CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iterPtr->operator==(*other.iterPtr);
// 	}
//     bool operator!=(const CastIterator& other) const
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		return iterPtr->operator!=(*other.iterPtr);
// 	}

// 	bool eq(const _IteratorBase<T&, std::shared_ptr<T>>& other) const override
// 	{
// 		if (typeid(other) == typeid(CastIterator)) return *this == reinterpret_cast<const CastIterator&>(other);
// 		return false;
// 	}

// 	bool ne(const _IteratorBase<T&, std::shared_ptr<T>>& other) const override
// 	{
// 		if (typeid(other) == typeid(CastIterator)) return *this != reinterpret_cast<const CastIterator&>(other);
// 		return true;
// 	}

//     // const std::type_info iterType() const override { return iterPtr->iterType(); }

//     ~CastIterator()
// 	{
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
// 		delete iterPtr;
// 	}
// };

// template <typename T>
// using CastIterator = boost::transform_iterator<std::reinterpret_pointer_cast<T>>;


// class BarBase
// {
// public:
// 	virtual CastIterator<std::shared_ptr<FooBase>> FooBaseBegin() = 0;
// 	virtual CastIterator<std::shared_ptr<FooBase>> FooBaseEnd() = 0;
// };

// class Bar : public BarBase
// {
// public:
// 	virtual CastIterator<std::shared_ptr<Foo>> FooBegin() = 0;
// 	virtual CastIterator<std::shared_ptr<Foo>> FooEnd() = 0;
// };

// class BarConcrete1 : public Bar
// {
// public:
//     std::vector<std::shared_ptr<FooConcrete1>> vec = {};
//     BarConcrete1()
//     {
// 		std::cout << __PRETTY_FUNCTION__ << std::endl;
//         // Populate vec with some data for demonstration.
//         vec.push_back(std::make_shared<FooConcrete1>());
//     }

//     CastIterator<std::shared_ptr<FooConcrete1>> FooConcrete1Begin() { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<FooConcrete1>>(vec.begin()); }
//     CastIterator<std::shared_ptr<FooConcrete1>> FooConcrete1End() { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<FooConcrete1>>::create(vec.end()); }

//     CastIterator<std::shared_ptr<Foo>> FooBegin() override { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<Foo>>::create(vec.begin()); }
//     CastIterator<std::shared_ptr<Foo>> FooEnd() override { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<Foo>>::create(vec.end()); }

//     CastIterator<std::shared_ptr<FooBase>> FooBaseBegin() override { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<FooBase>>::create(vec.begin()); }
//     CastIterator<std::shared_ptr<FooBase>> FooBaseEnd() override { std::cout << __PRETTY_FUNCTION__ << std::endl; return CastIterator<std::shared_ptr<FooBase>>::create(vec.end()); }
// };


// TEST(CustomIteratorTest, IteratesCorrectly)
// {
// 	std::cout << "Start" << std::endl;
//     BarConcrete1 bar;
// 	std::cout << "Instaciated" << std::endl;
//     auto begin = bar.FooBaseBegin();
//     auto end = bar.FooBaseEnd();
// 	std::cout << "Grabbed iters" << std::endl;

//     ASSERT_NE(begin, end);
// 	ASSERT_EQ(bar.vec.size(), 1);
	

//     int count = 0;
//     for (auto it = begin; it != end; ++it)
//     {
// 		std::cout << "Reading" << std::endl;
// 		ASSERT_LT(count, 1);
// 		ASSERT_TRUE(*it);
//         ASSERT_EQ((*it)->id(), count + 1);
//         ++count;
//     }

//     EXPECT_EQ(count, 1); // Example check for one element in vec
// }


#include <boost/iterator/transform_iterator.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <typeinfo>
#include <iostream>

template<typename _Iter>
concept any_iterator = std::forward_iterator<_Iter> || 
						std::bidirectional_iterator<_Iter> || 
						std::random_access_iterator<_Iter> ||
						std::input_or_output_iterator<_Iter> ||
						std::contiguous_iterator<_Iter>;


// Base and derived class hierarchy for Foo
class FooBase {
public:
    virtual int id() const = 0;
    virtual ~FooBase() = default;
};

class Foo : public FooBase {
protected:
    virtual int idImpl() const = 0;

public:
    int id() const override { return idImpl() + 1; }
};

class FooConcrete1 : public Foo {
protected:
    int idImpl() const override { return 0; }
};

class FooConcrete2 : public Foo {
protected:
    int idImpl() const override { return 2; }
};



// Base and derived class hierarchy for Bar
class BarBase {
public:
    virtual ~BarBase() = default;
    virtual auto FooBaseBegin() ->  = 0;
    virtual auto FooBaseEnd() -> boost::transform_iterator<std::function<std::shared_ptr<FooBase>(std::shared_ptr<Foo>)>, std::vector<std::shared_ptr<Foo>>::iterator> = 0;
};

class Bar : public BarBase {
public:
    virtual auto FooBegin() -> std::vector<std::shared_ptr<Foo>>::iterator = 0;
    virtual auto FooEnd() -> std::vector<std::shared_ptr<Foo>>::iterator = 0;
};

// Concrete implementation of Bar
class BarConcrete1 : public Bar {
private:
    std::vector<std::shared_ptr<FooConcrete1>> vec;

public:
    BarConcrete1() {
        vec.push_back(std::make_shared<FooConcrete1>());
    }

    auto FooBegin() -> std::vector<std::shared_ptr<Foo>>::iterator override {
        auto transformFunc = [](std::shared_ptr<FooConcrete1> foo) -> std::shared_ptr<FooBase> {
            return std::reinterpret_pointer_cast<FooBase>(foo);
        };
        return boost::make_transform_iterator(vec.begin(), transformFunc);
    }

    auto FooEnd() -> std::vector<std::shared_ptr<Foo>>::iterator override {
        return reinterpret_cast<std::vector<std::shared_ptr<Foo>>::iterator>(vec.end());
    }

    auto FooBaseBegin() override -> boost::transform_iterator<std::function<std::shared_ptr<FooBase>(std::shared_ptr<Foo>)>, std::vector<std::shared_ptr<Foo>>::iterator> {
        auto transformFunc = [](std::shared_ptr<Foo> foo) -> std::shared_ptr<FooBase> {
            return std::reinterpret_pointer_cast<FooBase>(foo);
        };
        return boost::make_transform_iterator(vec.begin(), transformFunc);
    }

    auto FooBaseEnd() override -> boost::transform_iterator<std::function<std::shared_ptr<FooBase>(std::shared_ptr<Foo>)>, std::vector<std::shared_ptr<Foo>>::iterator> {
        auto transformFunc = [](std::shared_ptr<Foo> foo) -> std::shared_ptr<FooBase> {
            return std::reinterpret_pointer_cast<FooBase>(foo);
        };
        return boost::make_transform_iterator(vec.end(), transformFunc);
    }
};

// Unit test
TEST(CustomIteratorTest, IteratesCorrectly) {
    BarConcrete1 bar;

    auto begin = bar.FooBaseBegin();
    auto end = bar.FooBaseEnd();

    ASSERT_NE(begin, end);

    int count = 0;
    for (auto it = begin; it != end; ++it) {
        EXPECT_EQ((*it)->id(), count + 1);
        ++count;
    }

    EXPECT_EQ(count, 1); // Verify the expected number of elements
}