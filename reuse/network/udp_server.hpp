//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_network_udp_server_hpp__
#define __reuse_network_udp_server_hpp__


namespace reuse { namespace network {

    template <size_t PACKET_LENGTH>
    struct udp_task {
        udp::endpoint endpoint;
        unsigned char buffer[PACKET_LENGTH];
        size_t        valid_size = 0;
    };

    // PACKET_LENGTH limit 64K
    template <size_t PACKET_LENGTH, size_t MAXIMUM_TASKLIST = (std::numeric_limits<size_t>::max)()>
    class udp_server
    {
    public:
        typedef std::shared_ptr<udp_task<PACKET_LENGTH>> task_ptr;
        typedef std::function<void(task_ptr)> on_read_t;

        udp_server(io_service& io_serv, unsigned short listen_port, size_t num_threads = 1)
            : task_pool_(num_threads), socket_(io_serv, udp::endpoint(udp::v4(), listen_port)) {}

        udp_server(io_service& io_serv, const char *host, unsigned short listen_port, size_t num_threads = 1)
            : task_pool_(num_threads), socket_(io_serv, udp::endpoint(address::from_string(host), listen_port)) {}

        ~udp_server()
        {
            task_pool_.stop_forceful();
        }

        void start(on_read_t on_read)
        {
            read_handler_ = std::move(on_read);
            task_count_ = 0;
            start_read();
        }

        void stop()
        {
            boost::system::error_code ignore_ec;
            socket_.shutdown(tcp::socket::shutdown_send, ignore_ec);
            socket_.close(ignore_ec);
            task_pool_.stop_graceful();
        }

        /// Notify server to handle an new send task.
        /**
        * This function is used to add datagram to send queue. it will be automatically
        * send datagram to the specified endpoint.
        *
        * @param task data and remote endpoint to be sent.
        * Copies will be made of the endpoint as required.
        */
        bool notify_send(task_ptr new_task)
        {
            if (++task_pool_ > MAXIMUM_TASKLIST) {
                REUSE_DEBUG("[warn] The current udp send queue is full!\n");
                return false;
            }

            socket_.get_io_service().post([this, new_task] { this->write(new_task); });
            return true;
        }

    private:

        void start_read()
        {
            auto new_task = std::make_shared<udp_task<PACKET_LENGTH>>();

            socket_.async_receive_from(
                boost::asio::buffer(new_task->buffer, PACKET_LENGTH),
                new_task->endpoint, make_custom_alloc_handler(allocator_,
                    [this, new_task](const boost::system::error_code& ec, size_t bytes_recvd)
            {
                if (ec)
                    REUSE_DEBUG("[warn] async_receive_from error: %s!\n", ec.message().c_str());
                else {
                    new_task->valid_size = bytes_recvd;
                    task_pool_.get_io_service().post([this, new_task] { read_handler_(new_task); });
                }

                start_read();
            }));
        }

        void write(task_ptr current_task)
        {
            socket_.async_send_to(
                boost::asio::buffer(current_task->buffer, current_task->valid_size),
                current_task->endpoint, make_custom_alloc_handler(allocator_,
                    [this](const boost::system::error_code& ec, size_t)
            {
                if (ec)
                    REUSE_DEBUG("[warn] async_send_to error: %s!\n", ec.message().c_str());
                else
                    --task_count_;
            }));
        }

    private:
        io_context_pool task_pool_;
        udp::socket socket_;
        handler_allocator allocator_;
        on_read_t read_handler_;
        std::atomic_size_t task_count_;
    };

} // namespace network
} // namespace reuse

#endif // __reuse_network_udp_server_hpp__
