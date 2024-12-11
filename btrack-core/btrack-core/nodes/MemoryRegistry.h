#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/optional.hpp>
#include <memory>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <opencv2/core.hpp>

namespace btrack { namespace nodes {



template <typename T>
struct _NonConst
{
    using sendParam = std::shared_ptr<const T>;
    using elemType = std::shared_ptr<const T>;
    using receiveParam = std::unique_ptr<T>&;

    static void toReceive(elemType s, receiveParam r)
    {
        if (s)
        {
            const T v = *s;
            r = std::make_unique<T>(v);
            return;
        }

        r.reset();
    }
};

template <typename T>
struct _Const
{
    using sendParam = std::shared_ptr<const T>;
    using elemType = std::shared_ptr<const T>;
    using receiveParam = std::shared_ptr<const T>&;

    static void toReceive(elemType s, receiveParam r)
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
    // using mutex = boost::mutex;
    boost::lockfree::queue<elemType, boost::lockfree::capacity<1>> mQueue;
    // mutex mMTX;
    boost::condition_variable mCV;

    struct Consumer
    {
        receiverParam p;
        void operator()(elemType data) { ChannelTypeInfo<T>::toReceive(data, p); }
    };

public:

    // Send data into the sender
    void send(senderParam data) override {
        {
            // boost::lock_guard<mutex> lock(mMTX);
            mQueue.push(data);
        }
        mCV.notify_one();
    }

    // Receive data from the sender
    void receive(receiverParam data) override {
        // boost::unique_lock<mutex> lock(mMTX);
        // mCV.wait(mQueue., [&] { return !mQueue.empty(); });

        elemType tmp;
        if (mQueue.pop(tmp)) ChannelTypeInfo<T>::toReceive(tmp, data);
        
    }

    // Non-blocking receive
    bool try_receive(receiverParam data) override {
        // std::lock_guard<mutex> lock(mMTX);
        if (mQueue.empty()) return false;
        receive(data);
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
public:
    // Add a sender
    void addChannel(SndPtr sender) {
        boost::lock_guard<mutex> lock(mMTX);
        mSenders.push_back(sender);
    }

    void addChannel(ConstSndPtr sender) {
        boost::lock_guard<mutex> lock(mMTX);
        mConstSenders.push_back(sender);
    }

	// Remove a sender
    void removeChannel(SndPtr sender) {
        boost::lock_guard<mutex> lock(mMTX);
        auto i = std::find(mSenders.begin(), mSenders.end(), sender);
		if (i != mSenders.end())
		{
        	mSenders.erase(i);
		}
    }

    void removeChannel(ConstSndPtr sender) {
        boost::lock_guard<mutex> lock(mMTX);
        auto i = std::find(mConstSenders.begin(), mConstSenders.end(), sender);
		if (i != mConstSenders.end())
		{
        	mConstSenders.erase(i);
		}
    }

    // Broadcast data to all receivers
    void send(senderParam data) override {
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