#ifndef SYNCSERVER_H
#define SYNCSERVER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/recursive_mutex.hpp>

class SyncServer
{
public:

    struct Connection : boost::enable_shared_from_this<Connection> {
        Connection(boost::asio::io_service& io_service)
            : sock_(io_service)
        {
        }

        void answer_to_client();

        boost::asio::ip::tcp::socket & sock() { return sock_; }

        void stop() {
            boost::system::error_code err;
            sock_.close(err);
        }

    private:
        void read_request();
        void process_request();

        void write(const std::string & msg);
    private:

        boost::asio::ip::tcp::socket sock_;
        enum { max_msg = 1024 };
        int already_read_ { 0 };
        char buff_[max_msg];
        bool started_ { false };
    };
    using connection_ptr = boost::shared_ptr<Connection>;
    using clients_vector = std::vector<connection_ptr>;

    void handle_clients_thread();
    void accept_thread();

    SyncServer(std::string ip, unsigned int port);
private:
    boost::asio::io_service m_service;
    boost::asio::ip::tcp::endpoint m_ep;
    clients_vector m_clients;
    // thread-safe access to clients array
    boost::recursive_mutex m_cs;
};

#endif // SYNCSERVER_H
