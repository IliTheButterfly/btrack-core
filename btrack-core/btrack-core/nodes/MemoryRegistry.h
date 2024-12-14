#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <deque>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <opencv2/core.hpp>
#include <iostream>

namespace btrack { namespace nodes {

template <typename T>
class ItemValue;

template <typename T>
class ItemView;

template <typename T>
struct _ItemValueAccessor
{
    static std::unique_ptr<T>&& getPtr(ItemValue<T>&& v) noexcept { return std::move(v.mPtr); }
    static std::unique_ptr<T>& getPtr(ItemValue<T>& v) noexcept { return v.mPtr; }
    static const std::unique_ptr<T>& getPtr(const ItemValue<T>& v) noexcept { return v.mPtr; }
};

template <typename T>
struct _ItemViewAccessor
{
    static std::shared_ptr<T>&& getPtr(ItemView<T>&& v) noexcept { return v.mPtr; }
    static std::shared_ptr<T>& getPtr(ItemView<T>& v) noexcept { return v.mPtr; }
    static const std::shared_ptr<T>& getPtr(const ItemView<T>& v) noexcept { return v.mPtr; }
};


// Memory debugging tool
// #define _MEM_DEBUG(...)
#define _MEM_DEBUG(...) __VA_ARGS__

template <typename T>
struct Tracker
{
    Tracker() noexcept { std::cout << "Created " << typeid(T).name() << std::endl; }
    ~Tracker() noexcept { std::cout << "Destroyed " << typeid(T).name() << std::endl; }
};

template <typename T>
class ItemView
{
    static_assert(boost::is_copy_constructible<T>::value, "T needs to be copy constructible.");
private:
    std::shared_ptr<T> mPtr;
    _MEM_DEBUG(Tracker<ItemView<T>> _;)

    explicit ItemView(std::shared_ptr<T>&& ptr) noexcept
        : mPtr(ptr) 
        { _MEM_DEBUG(std::cout << "Allocated " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }
public:
    ItemView() noexcept = default;

    template <typename U>
    static void create(ItemView<U>&& arg) noexcept {}

    template <typename U>
    static void create(const ItemView<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemView<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemValue<U>&& arg) noexcept {}

    template <typename U>
    static void create(const ItemValue<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemValue<U>& arg) noexcept {}

    template <typename... TArgs>
    static ItemView create(TArgs&&... args) noexcept
    {
        return ItemView(std::make_shared<T>(std::forward<TArgs>(args)...));
    }

    ItemView(const ItemValue<T>& v) noexcept
        : mPtr{_ItemValueAccessor<T>::getPtr(v) ? std::make_shared<T>(*(_ItemValueAccessor<T>::getPtr(v))) : std::shared_ptr<T>()} {}

    template <typename U>
    ItemView(const ItemValue<U>& v) noexcept
        : mPtr{_ItemValueAccessor<U>::getPtr(v) ? std::make_shared<T>(*(_ItemValueAccessor<U>::getPtr(v))) : std::shared_ptr<T>()} {}

    ItemView(ItemValue<T>&& v) noexcept
        : mPtr{_ItemValueAccessor<T>::getPtr(v) ? std::move(_ItemValueAccessor<T>::getPtr(v)) : std::shared_ptr<T>()} {}

    template <typename U>
    ItemView(ItemValue<U>&& v) noexcept
        : mPtr{_ItemValueAccessor<U>::getPtr(v) ? std::move(_ItemValueAccessor<U>::getPtr(v)) : std::shared_ptr<T>()} {}

    ItemView(const ItemView& v) noexcept
        : mPtr{v.mPtr} { _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

    template <typename U>
    ItemView(const ItemView<U>& v) noexcept
        : mPtr{_ItemViewAccessor<U>::getPtr(v)} { _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

    ItemView(ItemView&& v) noexcept
        : mPtr{std::move(v.mPtr)} { _MEM_DEBUG(std::cout << "Assigned " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

    template <typename U>
    ItemView(ItemView<U>&& v) noexcept
        : mPtr{std::move(_ItemViewAccessor<U>::getPtr(v))} { _MEM_DEBUG(std::cout << "Assigned " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

    // Copy reference of the ItemView
    ItemView& operator=(const ItemView<T>& c) noexcept
    {
        mPtr = c.mPtr;
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        return *this;
    }

    // Copy value of the ItemValue
    template <typename U>
    ItemView& operator=(const ItemValue<U>& c) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        return copyFrom(c);
    }

    // Move reference of the ItemValue
    template <typename U>
    ItemView& operator=(ItemValue<U>&& c) noexcept
    {
        _MEM_DEBUG(std::cout << "Assigned " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        return takeOwnership(std::forward(c));
    }

    // Take ownership of an ItemValue
    template <typename U>
    ItemView& takeOwnership(ItemValue<U>& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Moved " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        mPtr = std::move(_ItemValueAccessor<U>::getPtr(val));
        return *this;
    }

    // Copy from an ItemValue
    template <typename U>
    ItemView& copyFrom(const ItemValue<U>& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        if (_ItemValueAccessor<U>::getPtr(val)) mPtr = std::make_shared<T>(*(_ItemValueAccessor<U>::getPtr(val)));
        else mPtr.reset();
        return *this;
    }

    // Copy from an ItemView
    template <typename U>
    ItemView& copyFrom(const ItemView<U>& view) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        if (_ItemViewAccessor<U>::getPtr(view)) mPtr = std::make_shared<T>(*(_ItemViewAccessor<U>::getPtr(view)));
        else mPtr.reset();
        return *this;
    }

    // Get readonly reference
    const T* get() const noexcept { return mPtr.get(); }

    const T& operator*() const noexcept { return *mPtr; }
    const T* operator->() const noexcept { return mPtr.operator->(); }
    operator bool() const noexcept { return (bool)mPtr; }

    ~ItemView()
    {
        _MEM_DEBUG(if (mPtr.unique()) std::cout << "Deleted " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
    }

    friend _ItemViewAccessor<T>;
};

template <typename T>
class ItemValue
{
    static_assert(boost::is_copy_constructible<T>::value, "T needs to be copy constructible.");
private:
    std::unique_ptr<T> mPtr;
    _MEM_DEBUG(Tracker<ItemValue<T>> _;)

    explicit ItemValue(std::unique_ptr<T>&& ptr) noexcept 
        : mPtr(std::move(ptr))
        { _MEM_DEBUG(std::cout << "Allocated " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

public:
    ItemValue() noexcept = default;
    ItemValue(ItemValue&& val) noexcept
        : mPtr{std::move(val.mPtr)} { _MEM_DEBUG(std::cout << "Moved " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }
    template <typename U>
    ItemValue(ItemValue<U>&& val) noexcept
        : mPtr{std::move(_ItemValueAccessor<U>::getPtr(val))} { _MEM_DEBUG(std::cout << "Moved " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;) }

    template <typename U>
    static void create(ItemView<U>&& arg) noexcept {}

    template <typename U>
    static void create(const ItemView<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemView<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemValue<U>&& arg) noexcept {}

    template <typename U>
    static void create(const ItemValue<U>& arg) noexcept {}

    template <typename U>
    static void create(ItemValue<U>& arg) noexcept {}

    template <typename... TArgs>
    static ItemValue create(TArgs&&... args) noexcept
    {
        return ItemValue(std::make_unique<T>(std::forward<TArgs>(args)...));
    }

    template <typename U>
    ItemValue<T>& operator=(const ItemView<U>& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        return copyFrom(val);
    }

    ItemValue(const ItemValue&) = delete;
    template <typename U>
    ItemValue(const ItemValue<U>&) = delete;
    template <typename U>
    ItemValue<T>& operator=(const ItemValue<U>& val) = delete;
    // {
    //     return copyFrom(val);
    // }

    template <typename U>
    ItemValue<T>& operator=(ItemValue<U>&& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Moved " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        return takeOwnership(std::forward(val));
    }

    // Take ownership of an ItemValue
    template <typename U>
    ItemValue<T>& takeOwnership(ItemValue<U>& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Moved " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        mPtr = std::move(_ItemValueAccessor<U>::getPtr(val));
        return *this;
    }

    // Copy from an ItemValue
    template <typename U>
    ItemValue<T>& copyFrom(const ItemValue<U>& val) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        if (_ItemValueAccessor<U>::getPtr(val)) mPtr = std::make_unique<T>(*(_ItemValueAccessor<U>::getPtr(val)));
        else mPtr.reset();
        return *this;
    }

    // Copy from an ItemView
    template <typename U>
    ItemValue<T>& copyFrom(const ItemView<U>& view) noexcept
    {
        _MEM_DEBUG(std::cout << "Copied " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
        if (_ItemViewAccessor<U>::getPtr(view)) mPtr = std::make_unique<T>(*(_ItemViewAccessor<U>::getPtr(view)));
        else mPtr.reset();
        return *this;
    }

    // Get readonly reference
    const T* get() const noexcept { return mPtr.get(); }

    // Get read/write reference
    T* get() noexcept { return mPtr.get(); }

    const T& operator*() const noexcept { return *mPtr; }
    T& operator*() noexcept { return *mPtr; }
    const T* operator->() const noexcept { return mPtr.operator->(); }
    T* operator->() noexcept { return mPtr.operator->(); }
    operator bool() const noexcept { return (bool)mPtr; }

    ~ItemValue()
    {
        _MEM_DEBUG(if (mPtr) std::cout << "Deleted " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
    }

    friend _ItemValueAccessor<T>;
};

template <typename T>
struct _NonConst
{
    using sendParam = const ItemView<typename boost::remove_const<T>::type>&;
    using elemType = ItemView<typename boost::remove_const<T>::type>;
    using receiveParam = ItemValue<typename boost::remove_const<T>::type>&;

    static void toReceive(elemType& s, receiveParam r)
    {
        r.copyFrom(s);
    }
};

template <typename T>
struct _Const
{
    using sendParam = const ItemView<typename boost::remove_const<T>::type>&;
    using elemType = ItemView<typename boost::remove_const<T>::type>;
    using receiveParam = ItemView<typename boost::remove_const<T>::type>&;

    static void toReceive(elemType& s, receiveParam r)
    {
        r = s;
    }
};

template <typename T>
struct ChannelTypeInfo
{
    using inner = boost::conditional_t<boost::is_const<T>::value, _Const<T>, _NonConst<T>>;
    using sendParam = typename inner::sendParam;
    using elemType = typename inner::elemType;
    using receiveParam = typename inner::receiveParam;

    
    static void toReceive(elemType s, receiveParam r)
    {
        inner::toReceive(s, r);
    };
};


// template <typename T>
// struct ChannelTypeInfo : public boost::conditional<boost::is_const<T>::value_type, _ConstChannelTypeInfo<T>, _ChannelTypeInfo<T>>::type {};
#define FundamentalChannelTypeInfo(T)\
template <> struct ChannelTypeInfo<T>\
{\
    using sendParam = const boost::remove_const_t<T>&;\
    using elemType = boost::remove_const_t<T>;\
    using receiveParam = boost::remove_const_t<T>&;\
    static void toReceive(elemType s, receiveParam r)\
    {\
        r = s;\
    }\
}

FundamentalChannelTypeInfo(bool);
FundamentalChannelTypeInfo(int8_t);
FundamentalChannelTypeInfo(uint8_t);
FundamentalChannelTypeInfo(int16_t);
FundamentalChannelTypeInfo(uint16_t);
FundamentalChannelTypeInfo(int32_t);
FundamentalChannelTypeInfo(uint32_t);
FundamentalChannelTypeInfo(int64_t);
FundamentalChannelTypeInfo(uint64_t);
FundamentalChannelTypeInfo(float);
FundamentalChannelTypeInfo(double);

FundamentalChannelTypeInfo(const bool);
FundamentalChannelTypeInfo(const int8_t);
FundamentalChannelTypeInfo(const uint8_t);
FundamentalChannelTypeInfo(const int16_t);
FundamentalChannelTypeInfo(const uint16_t);
FundamentalChannelTypeInfo(const int32_t);
FundamentalChannelTypeInfo(const uint32_t);
FundamentalChannelTypeInfo(const int64_t);
FundamentalChannelTypeInfo(const uint64_t);
FundamentalChannelTypeInfo(const float);
FundamentalChannelTypeInfo(const double);

template <> 
struct ChannelTypeInfo<cv::Mat>
{
    using sendParam = cv::InputArray;
    using elemType = cv::InputArray;
    using receiveParam = cv::OutputArray;

    static void toReceive(elemType s, receiveParam r)
    {
        s.copyTo(r);
    }
};

template <> 
struct ChannelTypeInfo<const cv::Mat>
{
    using sendParam = cv::InputArray;
    using elemType = cv::InputArray;
    using receiveParam = cv::_InputArray&;

    static void toReceive(elemType s, receiveParam r)
    {
        r = s;
    }
};


template <typename T>
using SendParam_t = typename ChannelTypeInfo<T>::sendParam;
template <typename T>
using ElemType_t = typename ChannelTypeInfo<T>::elemType;
template <typename T>
using ReceiveParam_t = typename ChannelTypeInfo<T>::receiveParam;

// template <typename T>
// using ChannelTypeInfoSnd = typename ChannelTypeInfo<T>::senderParam;
// template <typename T>
// using ChannelTypeInfoElem = typename ChannelTypeInfo<T>::elemType;
// template <typename T>
// using ChannelTypeInfoRcv = typename ChannelTypeInfo<T>::receiverParam;

template <typename T>
class Sender
{
public:
    using sendParam = typename ChannelTypeInfo<T>::sendParam;
    virtual void send(sendParam data) = 0;
    // template <typename U>
    // void send(ChannelTypeInfo<U>::sendParam data)
    // {
    //     send(converter<U, T>(data));
    // }
};

template <typename T>
class Receiver
{
public:
    using receiverParam = typename ChannelTypeInfo<T>::receiveParam;
    // Receive data from the sender
    virtual void receive(receiverParam data) = 0;

    // Non-blocking receive
    virtual bool try_receive(receiverParam data) = 0;
};

template <typename T>
class Channel : public Sender<T>, public Receiver<T>
{
public:
    using senderParam = typename ChannelTypeInfo<T>::sendParam;
    using elemType = typename ChannelTypeInfo<T>::elemType;
    using receiverParam = typename ChannelTypeInfo<T>::receiveParam;
private:
    using queue = std::deque<elemType>;
    using mutex = boost::mutex;

    queue mQueue{};
    mutex mMTX{};
    boost::condition_variable mCV{};
    int mCapacity;

    void ensureCapacity() noexcept
    {
        if (mCapacity == -1) return;
        while (mQueue.size() > mCapacity)
        {
            mQueue.pop_front();
        }
    }

public:
    Channel(int capacity = 1)
        : mCapacity{capacity} {}

    // Send data into the sender
    void send(senderParam data) override
    {
        {
            boost::lock_guard<mutex> lock(mMTX);
            mQueue.push_back(data);
            ensureCapacity();
        }
        mCV.notify_one();
    }

    // Receive data from the sender
    void receive(receiverParam data) override
    {
        boost::unique_lock<mutex> lock(mMTX);
        while (mQueue.size() == 0)
        {
            mCV.wait(lock);
        }

        ChannelTypeInfo<T>::toReceive(mQueue.at(0), data);
        mQueue.pop_front();
    }

    // Non-blocking receive
    bool try_receive(receiverParam data) override
    {
        boost::lock_guard<mutex> lock(mMTX);
        if (mQueue.size() == 0) return false;
        ChannelTypeInfo<T>::toReceive(mQueue.at(0), data);
        mQueue.pop_front();
        return true;
    }
};

template <typename T>
class BroadcastChannel : public Sender<T>
{
public:
	using Snd = Sender<T>;
	using ConstSnd = Sender<const T>;
    using SharedSndPtr = std::shared_ptr<Snd>;
	using SharedConstSndPtr = std::shared_ptr<ConstSnd>;
    using SndPtr = std::weak_ptr<Snd>;
	using ConstSndPtr = std::weak_ptr<ConstSnd>;
    using senderParam = typename Sender<T>::sendParam;
private:
    using mutex = boost::mutex;

    std::vector<SndPtr> mSenders;
    std::vector<ConstSndPtr> mConstSenders;
    mutex mMTX;

    int find(SndPtr ptr)
    {
        if (ptr.expired()) return -1;
        int i = 0;
        for (SndPtr p : mSenders)
        {
            if (!p.expired() && p.lock() == ptr.lock()) return i;
            ++i;
        }
        return -1;
    }

    int find(std::vector<ConstSndPtr>::iterator b, std::vector<ConstSndPtr>::iterator e, ConstSndPtr ptr)
    {
        if (ptr.expired()) return -1;
        int i = 0;
        for (SndPtr p : mConstSenders)
        {
            if (!p.expired() && p.lock() == ptr.lock()) return i;
            ++i;
        }
        return -1;
    }
public:
    // Add a sender
    void addChannel(SndPtr sender)
    {
        boost::lock_guard<mutex> lock(mMTX);
        mSenders.push_back(sender);
    }

    void addChannel(ConstSndPtr sender)
    {
        boost::lock_guard<mutex> lock(mMTX);
        mConstSenders.push_back(sender);
    }

	// Remove a sender
    void removeChannel(SndPtr sender)
    {
        boost::lock_guard<mutex> lock(mMTX);
        auto i = find(sender);
		if (i != -1)
		{
        	mSenders.erase(mSenders.begin() + i);
		}
    }

    void removeChannel(ConstSndPtr sender)
    {
        boost::lock_guard<mutex> lock(mMTX);
        auto i = find(sender);
		if (i != -1)
		{
        	mConstSenders.erase(mConstSenders.begin() + i);
		}
    }

    // Broadcast data to all receivers
    void send(senderParam data) override
    {
        boost::lock_guard<mutex> lock(mMTX);
        int i = 0;
        int remove = -1;
        for (SndPtr sender : mSenders) 
        {
            if (!sender.expired()) sender.lock()->send(data);
            else remove = i;
            ++i;
        }
        if (remove != -1) mSenders.erase(mSenders.begin() + remove);

        i = 0;
        remove = -1;
        for (ConstSndPtr sender : mConstSenders) 
        {
            if (!sender.expired()) sender.lock()->send(data);
            else remove = i;
            ++i;
        }
        if (remove != -1) mConstSenders.erase(mConstSenders.begin() + remove);
    }
};

}} // btrack::nodes