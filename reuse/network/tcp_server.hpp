//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_network_tcp_server_hpp__
#define __reuse_network_tcp_server_hpp__

namespace reuse { namespace network {

    template <typename Connection>
    class tcp_server
    {
    public:
        tcp_server(io_service& io_serv, unsigned short listen_port, size_t num_threads = 1)
            : iocxt_pool_(num_threads), acceptor_(io_serv, tcp::endpoint(tcp::v4(), listen_port)) {}

        tcp_server(io_service& io_serv, const char *host, unsigned short listen_port, size_t num_threads = 1)
            : iocxt_pool_(num_threads), acceptor_(io_serv, tcp::endpoint(address::from_string(host), listen_port)) {}

        ~tcp_server()
        {
            iocxt_pool_.stop_forceful();
        }

        void start()
        {
            iocxt_pool_.start();
            async_accept();
        }

        void stop()
        {
            boost::system::error_code ignore_ec;
            acceptor_.cancel(ignore_ec);
            iocxt_pool_.stop_graceful();
        }

    private:

        void async_accept()
        {
            auto new_connection = std::make_shared<Connection>(iocxt_pool_.get_io_service());

            acceptor_.async_accept(new_connection->get_socket(),
                make_custom_alloc_handler(allocator_,
                    [this, new_connection](const boost::system::error_code& ec)
            {
                if (ec)
                    REUSE_DEBUG("[warn] async_accept error: %s!\n", ec.message().c_str());
                else {
                    tcp::no_delay option(true);
                    boost::system::error_code ignore_ec;
                    new_connection->get_socket().set_option(option, ignore_ec);
                    new_connection->start();
                }

                async_accept();
            }));
        }

        io_context_pool iocxt_pool_;
        tcp::acceptor acceptor_;
        handler_allocator allocator_;
    };

} // namespace network
} // namespace reuse

#endif // __reuse_network_tcp_server_hpp__
