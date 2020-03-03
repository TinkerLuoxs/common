//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_timing_wheel_hpp__
#define __reuse_timing_wheel_hpp__

namespace reuse {

template <typename Handler, int SCALES = 6>
class timing_wheel
{
public:
    typedef Handler handler_type;
    typedef std::pair<unsigned int, unsigned long long> index_type;
    typedef std::unordered_map<unsigned long long, handler_type> slot_type;
    
    timing_wheel(const timing_wheel&) = delete;
    timing_wheel& operator=(const timing_wheel&) = delete;
    
    explicit timing_wheel(unsigned int tick_interval_sec)
        : tick_interval_(tick_interval_sec), autoseries_(0), loopexit_(false)
    {
    }

    ~timing_wheel()
    {
        stop();
    }

    void start()
    {
        if (!thread_.joinable())
            thread_ = std::thread([this] {this->run(); });
    }

    void stop()
    {
        if (thread_.joinable()) {
            loopexit_ = true;
            thread_.join();
        }
    }

public:
    class element
    {
    public:
        element(timing_wheel& twheel) : timing_wheel_(twheel), index_(0, 0) {}

        void append(handler_type&& handler)
        {
            std::lock_guard<std::mutex> l(timing_wheel_.mutex_);
            index_.first = (timing_wheel_.no_slots_ == 0 ? SCALES : timing_wheel_.no_slots_) - 1;
            index_.second = ++timing_wheel_.autoseries_;
            timing_wheel_.scale_slots_[index_.first].emplace(index_.second, std::forward<handler_type>(handler));
        }

        bool update()
        {
            std::lock_guard<std::mutex> l(timing_wheel_.mutex_);
            auto& slot = timing_wheel_.scale_slots_[index_.first];
            auto it = slot.find(index_.second);
            if (it == slot.end())
                return false;

            index_.first = (timing_wheel_.no_slots_ == 0 ? SCALES : timing_wheel_.no_slots_) - 1;
            timing_wheel_.scale_slots_[index_.first].emplace(it->first, std::move(it->second));
            slot.erase(it);
            return true;
        }

        void cancel()
        {
            std::lock_guard<std::mutex> l(timing_wheel_.mutex_);
            auto& slot = timing_wheel_.scale_slots_[index_.first];
            auto it = slot.find(index_.second);
            if (it == slot.end())
                return;
            slot.erase(it);
        }

    private:
        timing_wheel &timing_wheel_;
        index_type index_;
    };

private:
    void run()
    {
        no_slots_ = 0;
        while (!loopexit_) {
            std::this_thread::sleep_for(std::chrono::seconds(tick_interval_));

            mutex_.lock();
            slot_type slot = std::move(scale_slots_[no_slots_]);
            mutex_.unlock();

            for (auto& it : slot)
                it.second();

            if (++no_slots_ >= SCALES)
                no_slots_ = 0;
        }
        loopexit_ = false;
    }
    unsigned int no_slots_;
    const unsigned int tick_interval_;
    unsigned long long autoseries_;
    std::atomic_bool loopexit_;
    slot_type scale_slots_[SCALES];
    std::thread thread_;
    std::mutex mutex_;
};

} // namespace reuse

#endif // __reuse_timing_wheel_hpp__
