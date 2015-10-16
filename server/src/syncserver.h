#ifndef SYNCSERVER_H
#define SYNCSERVER_H

#include <atomic>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>

#include "abstractcommand.h"

using CommandPtr = boost::shared_ptr<AbstractCommand>;

class SyncServer
{
public:
    using CommandProcessors = std::map<std::string, CommandPtr>;

    struct Connection : boost::enable_shared_from_this<Connection> {
        Connection(boost::asio::io_service& io_service,
                   boost::shared_ptr<LedLight> ledLight,
                   CommandProcessors& commandProcessors)
            : m_socket(io_service),
              m_ledLight(ledLight),
              m_commandProcessors(commandProcessors)
        {
        }

        void answer_to_client();

        boost::asio::ip::tcp::socket & socket() { return m_socket; }

        void stop();
        bool isClosed();

    private:
        void read_request();
        void process_request();

        void write(const std::string & msg);
    private:

        boost::asio::ip::tcp::socket m_socket;
        constexpr static unsigned int bufferLength { 1024 };
        int m_already_read { 0 };
        char m_buffer[bufferLength];
        bool m_started { false };

        boost::shared_ptr<LedLight> m_ledLight;
        CommandProcessors m_commandProcessors;
    };

    using connection_ptr = boost::shared_ptr<Connection>;
    using clients_vector = std::vector<connection_ptr>;

    SyncServer(boost::shared_ptr<LedLight> ledLight, std::string ip, unsigned int port);

    void run();
    void terminate();

    void registerCommandProcessor(std::string name, boost::shared_ptr<AbstractCommand> processor);

private:
    void handle_clients_thread();
    void accept_thread();

private:
    boost::shared_ptr<LedLight> m_ledLight;

    boost::thread_group m_threads;
    boost::asio::io_service m_service;
    boost::asio::ip::tcp::endpoint m_ep;
    clients_vector m_clients;
    // thread-safe access to clients array
    boost::recursive_mutex m_cs;

    std::atomic_bool m_terminated { false };

    CommandProcessors m_commandProcessors;
};

#endif // SYNCSERVER_H
