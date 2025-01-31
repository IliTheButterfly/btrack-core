#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <deque>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <concepts>

namespace btrack::nodes::system {

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
#define _MEM_DEBUG(...)
// #define _MEM_DEBUG(...) __VA_ARGS__

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

    ~ItemView() noexcept
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

    ~ItemValue() noexcept
    {
        _MEM_DEBUG(if (mPtr) std::cout << "Deleted " << typeid(T).name() << " from " << __PRETTY_FUNCTION__ << std::endl;)
    }

    friend _ItemValueAccessor<T>;
};


// Define the concept for ChannelTypeInfo
template <typename T, typename DT>
concept ChannelTypeConcept = requires {
    typename T::type; // Alias to data type
    typename T::item; // Alias for containers like arrays or queues
    typename T::readonlyRef; // Alias for readonly references
    typename T::readonlyIn; // Alias for readonly input parameters to functions
    typename T::readonlyOut; // Alias for readonly output parameters to functions
    typename T::readonlyInOut; // Alias for readonly input/output parameters to functions
    typename T::writable; // Alias for writable values
    typename T::writableRef; // Alias for writable references
    typename T::writableIn; // Alias for writable input parameters to functions
    typename T::writableOut; // Alias for writable output parameters to functions
    typename T::writableInOut; // Alias for writable input/output parameters to functions

    // Ensure T::type matches the specified DT
    std::same_as<typename T::type, DT>;

    // Ensure conversions
    std::convertible_to<typename T::item, typename T::readonlyRef>;
    std::convertible_to<typename T::item, typename T::readonlyIn>;
    std::convertible_to<typename T::item, typename T::readonlyOut>;
    std::convertible_to<typename T::item, typename T::readonlyInOut>;

    
    { T::byRef(std::declval<typename T::readonlyRef>(), 
               std::declval<typename T::readonlyOut>()) } -> std::same_as<void>;

    { T::byCopy(std::declval<typename T::readonlyRef>(), 
                std::declval<typename T::writableOut>()) } -> std::same_as<void>;

    { T::moveFromRef(std::declval<typename T::readonlyIn>()) } -> std::same_as<typename T::writable>;
    
    { T::moveFromCopy(std::declval<typename T::writableIn>()) } -> std::same_as<typename T::writableIn>;
};

template <typename T>
struct DefaultChannelTypeInfo
{
    using type = T; // Alias to T
    using item = ItemView<T>; // Alias for containers like arrays or queues
    using readonlyRef = ItemView<T>&; // Alias for readonly references
    using readonlyIn = const ItemView<T>&; // Alias for readonly input parameters to functions
    using readonlyOut = ItemView<T>&; // Alias for readonly output parameters to functions
    using readonlyInOut = ItemView<T>&; // Alias for readonly input/output parameters to functions
    using writable = ItemValue<T>; // Alias for writable values
    using writableRef = ItemValue<T>&; // Alias for writable references
    using writableIn = ItemValue<T>&&; // Alias for writable input parameters to functions
    using writableOut = ItemValue<T>&; // Alias for writable output parameters to functions
    using writableInOut = ItemValue<T>&; // Alias for writable input/output parameters to functions

    static void byCopy(readonlyRef s, writableOut c)
    {
        c.copyFrom(s);
    }

    static void byRef(readonlyRef s, readonlyOut r)
    {
        r = s;
    }

    static writable moveFromRef(readonlyIn r)
    {
        writable res;
        res.copyFrom(r);
        return std::move(res);
    }

    static writableIn moveFromCopy(writableIn r)
    {
        return static_cast<writableIn>(std::move(r));
    }
};


template <>
struct DefaultChannelTypeInfo<cv::Mat>
{
    using type = cv::Mat;
    using item = cv::InputArray;
    using readonlyRef = cv::_InputArray&;
    using readonlyIn = cv::InputArray;
    using readonlyOut = cv::_InputArray&;
    using readonlyInOut = cv::_InputArray&;
    using writable = cv::Mat;
    using writableRef = cv::Mat&;
    using writableIn = cv::_InputArray&;
    using writableOut = cv::OutputArray;
    using writableInOut = cv::InputOutputArray;

    static void byCopy(readonlyRef s, writableOut c)
    {
        s.copyTo(c);
    }

    static void byRef(readonlyRef s, readonlyOut r)
    {
        r = s;
    }

    static writable moveFromRef(readonlyIn r)
    {
        return r.getMat();
    }

    static writable moveFromCopy(writableIn r)
    {
        writable w;
        r.copyTo(w);
        return w;
    }
};

#define FundamentalChannelTypeInfo(T)\
template <> struct DefaultChannelTypeInfo<T>\
{\
    using type = T;\
    using item = T;\
    using readonlyRef = const T&;\
    using readonlyIn = const T&;\
    using readonlyOut = T&;\
    using readonlyInOut = T&;\
    using writable = T;\
    using writableRef = T&;\
    using writableIn = const T&;\
    using writableOut = T&;\
    using writableInOut = T&;\
    \
    static void byRef(readonlyRef s, readonlyOut r)\
    {\
        r = s;\
    }\
    static void byCopy(readonlyRef s, writableOut r)\
    {\
        r = s;\
    }\
    static writable moveFromRef(readonlyIn r) { return r; }\
    static writableIn moveFromCopy(writableIn r) { return r; }\
}



// template <typename T>
// struct ChannelTypeInfo : public boost::conditional<boost::is_const<T>::value_type, _ConstChannelTypeInfo<T>, _ChannelTypeInfo<T>>::type {};


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




template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Sender
{
    static_assert(ChannelTypeConcept<I, T>, "I must satisfy ChannelTypeConcept");
public:
    virtual void send(typename I::readonlyIn data) = 0;
    virtual void sendAndTake(typename I::writableIn data) = 0;
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Receiver
{
    static_assert(ChannelTypeConcept<I, T>, "I must satisfy ChannelTypeConcept");
public:
    virtual void receive(typename I::readonlyOut data) = 0;
    virtual bool tryReceive(typename I::readonlyOut data) = 0;
    virtual void receiveCopy(typename I::writableOut data) = 0;
    virtual bool tryReceiveCopy(typename I::writableOut data) = 0;
};


template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class Channel : public Sender<T, I>, public Receiver<T, I>
{
private:
    using queue = std::deque<typename I::item>;
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
    void send(typename I::readonlyIn data) override
    {
        std::cout << "Sending" << std::endl;
        {
            boost::lock_guard<mutex> lock(mMTX);
            mQueue.push_back(data);
            ensureCapacity();
        }
        mCV.notify_one();
    }

    // Send data into the sender
    void sendAndTake(typename I::writableIn data) override
    {
        std::cout << "Sending and taking" << std::endl;
        {
            boost::lock_guard<mutex> lock(mMTX);
            mQueue.push_back(I::moveFromCopy(std::forward<typename I::writableIn>(data)));
            ensureCapacity();
        }
        mCV.notify_one();
    }

    // Receive data from the sender
    void receive(typename I::readonlyOut data) override
    {
        std::cout << "Receiving" << std::endl;
        boost::unique_lock<mutex> lock(mMTX);
        while (mQueue.size() == 0)
        {
            mCV.wait(lock);
        }

        I::byRef(mQueue.at(0), data);
        mQueue.pop_front();
    }

    // Non-blocking receive
    bool tryReceive(typename I::readonlyOut data) override
    {
        std::cout << "Try receiving" << std::endl;
        boost::lock_guard<mutex> lock(mMTX);
        if (mQueue.size() == 0) return false;
        I::byRef(mQueue.at(0), data);
        mQueue.pop_front();
        return true;
    }

    // Receive data from the sender
    void receiveCopy(typename I::writableOut data) override
    {
        std::cout << "Receiving a copy" << std::endl;
        boost::unique_lock<mutex> lock(mMTX);
        while (mQueue.size() == 0)
        {
            mCV.wait(lock);
        }

        I::byCopy(mQueue.at(0), data);
        mQueue.pop_front();
    }

    // Non-blocking receive
    bool tryReceiveCopy(typename I::writableOut data) override
    {
        std::cout << "Try receiving a copy" << std::endl;
        boost::lock_guard<mutex> lock(mMTX);
        if (mQueue.size() == 0) return false;
        I::byCopy(mQueue.at(0), data);
        mQueue.pop_front();
        return true;
    }
};

template <typename T, ChannelTypeConcept<T> I = DefaultChannelTypeInfo<T>>
class BroadcastChannel : public Sender<T, I>
{
public:
	using Snd = Sender<T, I>;
    using SharedSndPtr = std::shared_ptr<Snd>;
    using SndPtr = std::weak_ptr<Snd>;
private:
    using mutex = boost::mutex;

    std::vector<SndPtr> mSenders;
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

public:
    // Add a sender
    void addChannel(SndPtr sender)
    {
        boost::lock_guard<mutex> lock(mMTX);
        mSenders.push_back(sender);
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

    // Broadcast data to all receivers
    void send(typename I::readonlyIn data) override
    {
        std::cout << "Broadcasting" << std::endl;
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
    }

    // Broadcast data to all receivers
    void sendAndTake(typename I::writableIn data) override
    {
        boost::lock_guard<mutex> lock(mMTX);
        int i = 0;
        int remove = -1;
        if (mSenders.size() == 0) return;
        else if (mSenders.size() == 1)
        {
            if (!mSenders[0].expired()) mSenders[0].lock()->sendAndTake(std::move(data));
            else mSenders.clear();
        }
        else
        {
            send(I::moveFromCopy(std::move(data)));
        }
    }
};


} // namespace btrack::nodes::system