#ifndef _LYNN_INCLUDE_RPC_H_
#define _LYNN_INCLUDE_RPC_H_

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <server/TThreadPoolServer.h>
#include <server/TThreadedServer.h>
#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>
#include <transport/TSocket.h>

#include <string>

#include <sys/types.h>
#include <dirent.h>

using std::string;

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

using boost::shared_ptr;

template <typename T>
class Proxy {
public:
    Proxy(const T& client, const shared_ptr<TTransport>& connector)
    : m_client(client), m_connector(connector) {
        m_connector->open();
    }

    ~Proxy() {
        m_connector->close();  
    }
    
    /// @brief: overwrite ()
    T& operator()() {
        return m_client;
    }
private:
    T m_client;
    shared_ptr<TTransport> m_connector;
};

template <typename T, typename P>
class Rpc {
public:
    //TODO move the configuration out of here into a single file
    static const int DEFAULT_THREAD_NUM = 4;
    static const int TIME_OUT = 2000;

    static void Listen(int port) {

        shared_ptr<T> handler(new T);
        shared_ptr<TProcessor> processor(new P(handler));
        shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
        shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
        shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
        
        //单线程的服务器 
        /*  
        TSimpleServer server(processor,  
                            serverTransport,  
                            transportFactory,  
                            protocolFactory);  
        */
        
        //每一个请求一个线程处理
        TThreadedServer server(processor, 
                               serverTransport, 
                               transportFactory, 
                               protocolFactory); 
        
        /* 
        //启用了线程池
        //启动了13个物理机器的时候容易挂死
        shared_ptr<ThreadManager> threadManager =
            ThreadManager::newSimpleThreadManager(DEFAULT_THREAD_NUM);
        shared_ptr<PosixThreadFactory> thread_factory(new PosixThreadFactory());
        threadManager->threadFactory(thread_factory);
        threadManager->start();
        TThreadPoolServer server(processor,
                                 serverTransport,
                                 transportFactory,
                                 protocolFactory,
                                 threadManager);
        */

        server.serve();
    }

    static Proxy<T> GetProxy(const string& endpoint, int timeout = TIME_OUT) {
        // split endpoint into ip and port
        string ip = endpoint.substr(0, endpoint.find(":"));
        string str_port = endpoint.substr(endpoint.find(":") + 1);
        int port = atoi(str_port.c_str());
        TSocket* sc = new TSocket(ip, port);
        // 传大文件的时候不能设置timeout
        // sc->setRecvTimeout(timeout);
        shared_ptr<TTransport> socket(sc);
        shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        T client(protocol);
        Proxy<T> proxy(client, transport);
        return proxy;
    }
};

#endif
