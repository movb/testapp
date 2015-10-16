#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "syncserver.h"

using namespace boost::asio;
using namespace boost::posix_time;

void SyncServer::Connection::answer_to_client()
{
    try {
        read_request();
        process_request();
    } catch ( boost::system::system_error&) {
        stop();
    }
}

void SyncServer::Connection::read_request()
{
    if ( sock_.available())
        already_read_ += sock_.read_some(
                    buffer(buff_ + already_read_, max_msg - already_read_));
}

void SyncServer::Connection::process_request()
{
    bool command_end = std::find(buff_, buff_ + already_read_, '\n')
                      < buff_ + already_read_;
    if ( !command_end)
        return;

    size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
    std::string msg(buff_, pos);
    std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
    already_read_ -= pos + 1;

    if ( msg.find("hello") == 0) write("World!\n");
    else write("unknown command\n");
}

void SyncServer::Connection::write(const std::__cxx11::string &msg)
{
    sock_.write_some(buffer(msg));
}


SyncServer::SyncServer(std::string ip, unsigned int port)
    :
      m_ep(ip::address::from_string(ip),port)
{
    boost::thread_group threads;
    threads.create_thread(boost::bind(&SyncServer::accept_thread, this));
    threads.create_thread(boost::bind(&SyncServer::handle_clients_thread, this));
    threads.join_all();
}

void SyncServer::handle_clients_thread()
{
    while ( true) {
        boost::this_thread::sleep( millisec(1));
        boost::recursive_mutex::scoped_lock lk(m_cs);
        for ( clients_vector::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
            (*it)->answer_to_client();
        // erase clients that timed out
        /*
        m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                   boost::bind(&talk_to_client::timed_out,_1)), m_clients.end());
                   */
    }
}

void SyncServer::accept_thread()
{
    ip::tcp::acceptor acceptor(m_service, m_ep);
    while ( true) {
        connection_ptr new_( new Connection(m_service));
        acceptor.accept(new_->sock());

        boost::recursive_mutex::scoped_lock lk(m_cs);
        m_clients.push_back(new_);
    }
}
