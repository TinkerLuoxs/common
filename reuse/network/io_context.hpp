//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_network_io_context_hpp__
#define __reuse_network_io_context_hpp__

#include <vector>
#include <string>
#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>


namespace reuse { namespace network {

    using namespace boost::asio;
    using namespace boost::asio::ip;

    // Class to manage the memory to be used for handler-based custom allocation.
    // It contains a single block of memory which may be returned for allocation
    // requests. If the memory is in use when an allocation request is made, the
    // allocator delegates allocation to the global heap.
    class handler_allocator
    {
    public:
        handler_allocator() : used_(false) {}
        handler_allocator(const handler_allocator&) = delete;
        handler_allocator& operator=(const handler_allocator&) = delete;

        void* allocate(std::size_t size)
        {
            if (!used_ && size < sizeof(storage_)) {
                used_ = true;
                return &storage_;
            }

            return ::operator new(size);
        }

        void deallocate(void* pointer)
        {
            if (pointer == &storage_)
                used_ = false;
            else
                ::operator delete(pointer);
        }
    private:
        // Storage space used for handler-based custom memory allocation.
        std::aligned_storage<1024>::type storage_;

        // Whether the handler-based custom allocation storage has been used.
        bool used_;
    };

    // Wrapper class template for handler objects to allow handler memory
    // allocation to be customised. Calls to operator() are forwarded to the
    // encapsulated handler.
    template <typename Handler>
    class custom_alloc_handler
    {
    public:
        custom_alloc_handler(handler_allocator& a, Handler& h) : allocator_(a), handler_(h) {}
        custom_alloc_handler(handler_allocator& a, Handler&& h) : allocator_(a), handler_(std::move(h)) {}

        template <typename ...Args>
        void operator()(Args&&... args)
        {
            handler_(std::forward<Args>(args)...);
        }

        friend void* asio_handler_allocate(std::size_t size,
            custom_alloc_handler<Handler>* this_handler)
        {
            return this_handler->allocator_.allocate(size);
        }

        friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/,
            custom_alloc_handler<Handler>* this_handler)
        {
            this_handler->allocator_.deallocate(pointer);
        }

    private:
        handler_allocator& allocator_;
        Handler handler_;
    };

    // Helper function to wrap a handler object to add custom allocation.
    template <typename Handler>
    inline custom_alloc_handler<Handler> make_custom_alloc_handler(handler_allocator& a, Handler&& h)
    {
        return custom_alloc_handler<Handler>(a, std::forward<Handler>(h));
    }

    class io_context_pool : private boost::noncopyable
    {
    public:
        explicit io_context_pool(size_t num_threads): pools_(num_threads) {}

        ~io_context_pool()
        {
            stop_forceful();
        }

        void start() 
        {
            index_ = pools_.begin();
            for (auto& elem : pools_)
                elem.start();
        }

        void stop_forceful()
        {
            for (auto& elem : pools_)
                elem.stop_forceful();
        }

        void stop_graceful()
        {
            for (auto& elem : pools_)
                elem.stop_graceful();
        }

        io_service& get_io_service()
        {
            auto current_index = index_;
            if (++index_ == pools_.end())
                index_ = pools_.begin();
            return current_index->get_io_service();
        }

    private:
        class element
        {
        public:
            void start()
            {
                if (!worker_.joinable()) {
                    task_ = std::make_unique<io_service::work>(io_serv_);
                    worker_ = std::thread([this] { io_serv_.run(); });
                }
            }

            void stop_forceful()
            {
                if (worker_.joinable()) {
                    task_.reset();
                    io_serv_.stop();
                    worker_.join();
                }
            }

            void stop_graceful()
            {
                if (worker_.joinable()) {
                    task_.reset();
                    worker_.join();
                }
            }

            io_service& get_io_service()
            {
                return io_serv_;
            }

        private:
            std::thread worker_;
            io_service io_serv_;
            std::unique_ptr<io_service::work> task_;
        };

    private:
        std::vector<element> pools_;
        std::vector<element>::iterator index_;
    };

} // namespace network
} // namespace reuse

#include "tcp_connection.hpp"
#include "tcp_server.hpp"
#include "udp_server.hpp"

#endif // __reuse_network_io_context_hpp__
