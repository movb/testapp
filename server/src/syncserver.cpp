#include <sstream>
#include <algorithm>
#include <iterator>

#include <boost/bind.hpp>

#include "syncserver.h"

using namespace boost::asio;
using namespace boost::posix_time;
using namespace std;

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
    bool command_end = find(m_buffer, m_buffer + m_already_read, '\n')
                      < m_buffer + m_already_read;
    if ( !command_end)
        return;

    size_t pos = find(m_buffer, m_buffer + m_already_read, '\n') - m_buffer;
    string message(m_buffer, pos);
    copy(m_buffer + m_already_read, m_buffer + bufferLength, m_buffer);
    m_already_read -= pos + 1;

    string command;
    list<string> params;
    pos = message.find(' ');
    if(pos != string::npos) {
        command = message.substr(0,pos);
        istringstream iss(message.substr(pos+1));
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(params));
    }
    else {
        //command = message.substr(0,message.size()-1);
        command = message;
    }

    CommandProcessors::iterator it = m_commandProcessors.find(command);
    if(it != m_commandProcessors.end())
    {
        // выполняем команду
        write(it->second->processCommand(m_ledLight,params));
    }
    else if((it = m_commandProcessors.find("default")) != m_commandProcessors.end())
    {
        // если есть дефолтная команда
        write(it->second->processCommand(m_ledLight,params));
    }
    // иначе ничего не делаем
}

void SyncServer::Connection::write(const string &msg)
{
    m_socket.write_some(buffer(msg + "\n"));
}


SyncServer::SyncServer(boost::shared_ptr<LedLight> ledLight, string ip, unsigned int port)
    :
      m_ledLight(ledLight),
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

        m_clients.erase(remove_if(m_clients.begin(), m_clients.end(),
                   boost::bind(&Connection::isClosed,_1)), m_clients.end());
    }
}

void SyncServer::accept_thread()
{
    ip::tcp::acceptor acceptor(m_service, m_ep);
    while ( !m_terminated ) {
        connection_ptr new_( new Connection(m_service, m_ledLight, m_commandProcessors));
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

void SyncServer::registerCommandProcessor(string name, boost::shared_ptr<AbstractCommand> processor)
{
    m_commandProcessors[name] = processor;
}
