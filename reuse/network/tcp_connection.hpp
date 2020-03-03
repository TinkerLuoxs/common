//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_network_connection_hpp__
#define __reuse_network_connection_hpp__

namespace reuse { namespace network {

class connection : public std::enable_shared_from_this<connection>
{
public:
	connection(const connection&) = delete;
	connection& operator=(const connection&) = delete;

    connection(io_service& ios) : io_service_(ios), socket_(ios), timer_(ios) {}
    
    connection(tcp::socket socket) : io_service_(socket.get_io_service()), socket_(std::move(socket)), timer_(io_service_) {}

    typedef std::function<void(void)> handler_type;

    template <typename WBuf, typename F>
    void async_write(WBuf&& wbuf, F&& f)
    {
        boost::asio::async_write(socket_, std::forward<WBuf>(wbuf),
            make_custom_alloc_handler(allocator_, 
                [self = shared_from_this(), func = std::forward<F>(f)](const boost::system::error_code& ec, size_t)
        {
            if (self->expired_) {
                CONSOLE_INFO("[info] async_write cancel, connection is closed.\n");
                return;
            }

            if (ec) {
                CONSOLE_DEBUG("[warn] async_write failure: %s!\n", ec.message().c_str());
                self->close();
                return;
            }

            func();
        }));
    }

    template <typename RBuf, typename F, typename Cond = boost::asio::transfer_all()>
	void async_read(RBuf&& rbuf, F&& f, Cond&& c)
	{
		boost::asio::async_read(socket_, std::forward<RBuf>(rbuf), std::forward<Cond>(c),
            make_custom_alloc_handler(allocator_, 
                [self = shared_from_this(), func = std::forward<F>(f)](const boost::system::error_code& ec, size_t bytes)
        {
            if (self->expired_) {
                CONSOLE_INFO("[info] async_read cancel, connection is closed.\n");
                return;
            }

            if (ec) {
                CONSOLE_DEBUG("[warn] async_read failure: %s!\n", ec.message().c_str());
                self->close();
                return;
            }

            func(bytes);      
        }));
	}

    template <typename RBuf, typename F>
    void async_read_some(RBuf&& rbuf, F&& f)
    {
        socket_.async_read_some(std::forward<RBuf>(rbuf),
            make_custom_alloc_handler(allocator_, 
                [self = shared_from_this(), func = std::forward<F>(f)](const boost::system::error_code& ec, size_t bytes)
        {
            if (self->expired_) {
                CONSOLE_INFO("[info] async_read_some cancel, connection is closed.\n");
                return;
            }

            if (ec) {
                CONSOLE_DEBUG("[warn] async_read_some failure: %s!\n", ec.message().c_str());
                self->close();
                return;
            }

            func(bytes);
        }));
    }

    template <typename Duration, typename F>
    void async_wait(Duration&& duration, F&& f)
    {
        timer_.expires_from_now(duration);
        timer_.async_wait(make_custom_alloc_handler(allocator_,
            [self = shared_from_this(), func = std::forward<F>(f)](const boost::system::error_code& ec) { func(ec); }));
    }

    void close()
    {
        if (!expired_)
        {
            expired_ = true;
            boost::system::error_code ignore_ec;
            socket_.shutdown(tcp::socket::shutdown_send, ignore_ec);
            socket_.close(ignore_ec);
            if (close_handler_) 
                close_handler_();
        }
    }

    io_service& get_io_service()
    {
        return io_service_;
    }

    tcp::socket& get_socket()
    {
        return socket_;
    }

protected:
    // IO service reference
    io_service& io_service_;

    // Socket for the connection.
	tcp::socket socket_;

    // Timer for connection timeout or other
    steady_timer timer_;

    // The allocator to use for handler-based custom memory allocation.
	handler_allocator allocator_;

    // Derived class deal with own resources when the connection is closing
    handler_type close_handler_;

    // Represents a connection status
	bool expired_ = false;
};

class active_connection : public connection
{
public:
    active_connection(io_service& io_serv, size_t max_reconnect = (std::numeric_limits<std::size_t>::max)())
        : connection(io_serv), resolver_(io_serv), maximum_reconnect_(max_reconnect), count_reconnect_(0) {}

    template <typename OnConnected>
    void register_events(OnConnected&& on_connected)
    {
        connect_handler_ = std::forward<OnConnected>(on_connected);
        close_handler_ = [this] {this->on_close(); };
    }

    template <typename OnConnected, typename OnClosed>
    void register_events(OnConnected&& on_connected, OnClosed&& on_closed)
    {
        connect_handler_ = std::forward<OnConnected>(on_connected);
        close_handler_ = std::forward<OnClosed>(on_closed);
    }

    void async_connect(const std::string& host, const std::string& serv)
    {
        async_resolve(tcp::resolver::query(host, serv));
    }

    void async_connect(const std::string& address, unsigned short port)
    {
        async_resolve(tcp::resolver::query(address, std::to_string(port)));
    }

    void close_connect()
    {
        count_reconnect_ = maximum_reconnect_;
        close();
    }

private:
    void on_close()
    {
        if (++count_reconnect_ < maximum_reconnect_) {
            expired_ = false;
            async_wait(std::chrono::seconds(interval_reconnect), [this](const boost::system::error_code& ec)
            {
                if (expired_) {
                    CONSOLE_INFO("[info] The user has cancelled the reconnect.\n");
                    return;
                }
                start_connect(iterator_);
            });
        }
    }

    void async_resolve(tcp::resolver::query&& query)
    {
        resolver_.async_resolve(query, make_custom_alloc_handler(allocator_,
            [this, self = shared_from_this()](const boost::system::error_code& ec, tcp::resolver::iterator i)
        {
            if (expired_) {
                CONSOLE_INFO("[info] async_resolve cancel, connection is closed!\n");
                return;
            }

            if (ec) {
                CONSOLE_DEBUG("[warn] async_resolve failure: %s!\n", ec.message().c_str());
                close_connect();
                return;
            }

            start_connect(i);
        }));
    }

    void start_connect(tcp::resolver::iterator i)
    {
        boost::asio::async_connect(socket_, i, make_custom_alloc_handler(allocator_,
            [this, self = shared_from_this()](const boost::system::error_code& ec, tcp::resolver::iterator i)
        {
            if (expired_) {
                CONSOLE_INFO("[info] async_connect cancel, connection is closed!\n");
                return;
            }

            if (ec) {
                CONSOLE_DEBUG("[warn] async_connect failure: %s!\n", ec.message().c_str());
                close();
                return;
            }

            iterator_ = i;
            connect_handler_();
        }));
    }

    tcp::resolver resolver_;
    tcp::resolver::iterator iterator_;
    const size_t maximum_reconnect_;
    size_t count_reconnect_;
    handler_type connect_handler_;

    static const size_t interval_reconnect = 5; // reconnect interval time, unit seconds.
};

} // namespace network
} // namespace reuse

#endif // __reuse_network_connection_hpp__
