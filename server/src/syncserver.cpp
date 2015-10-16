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

void SyncServer::Connection::stop() {
    boost::system::error_code err;
    m_socket.close(err);
}

bool SyncServer::Connection::isClosed()
{
    return !m_socket.is_open();
}

void SyncServer::Connection::read_request()
{
    if ( m_socket.available())
        m_already_read += m_socket.read_some(
                    buffer(m_buffer + m_already_read, bufferLength - m_already_read));
}

void SyncServer::Connection::process_request()
{
    bool command_end = std::find(m_buffer, m_buffer + m_already_read, '\n')
                      < m_buffer + m_already_read;
    if ( !command_end)
        return;

    size_t pos = std::find(m_buffer, m_buffer + m_already_read, '\n') - m_buffer;
    std::string msg(m_buffer, pos);
    std::copy(m_buffer + m_already_read, m_buffer + bufferLength, m_buffer);
    m_already_read -= pos + 1;

    if ( msg.find("hello") == 0) write("World!\n");
    else write("unknown command\n");
}

void SyncServer::Connection::write(const std::__cxx11::string &msg)
{
    m_socket.write_some(buffer(msg));
}


SyncServer::SyncServer(std::string ip, unsigned int port)
    :
      m_ep(ip::address::from_string(ip),port)
{
}

void SyncServer::run()
{
    m_threads.create_thread(boost::bind(&SyncServer::accept_thread, this));
    m_threads.create_thread(boost::bind(&SyncServer::handle_clients_thread, this));

    while( !m_terminated )
    {
        boost::this_thread::sleep( millisec(100));
    }
}

void SyncServer::handle_clients_thread()
{
    while ( !m_terminated ) {
        boost::this_thread::sleep( millisec(1));
        boost::recursive_mutex::scoped_lock lk(m_cs);
        for ( clients_vector::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
            (*it)->answer_to_client();

        m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(),
                   boost::bind(&Connection::isClosed,_1)), m_clients.end());
    }
}

void SyncServer::accept_thread()
{
    ip::tcp::acceptor acceptor(m_service, m_ep);
    while ( m_terminated ) {
        connection_ptr new_( new Connection(m_service));
        acceptor.accept(new_->socket());

        boost::recursive_mutex::scoped_lock lk(m_cs);
        m_clients.push_back(new_);
    }
}

void SyncServer::terminate()
{
    m_terminated = true;

    boost::recursive_mutex::scoped_lock lk(m_cs);
    for ( clients_vector::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
        (*it)->stop();

    m_threads.join_all();
}
