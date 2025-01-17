#include "adas_tcp_client.h"
#include "v2x_adas_event_macro.h"
#include "event_dispatcher.h"
#include "v2x_data_struct.h"
#include "cdd_fusion.h"
#include "event_queue.h"

using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;
using namespace V2X;

AdasTcpClient::AdasTcpClient(EventLoop* loop, const InetAddress& serverAddr)
    : client_(loop, serverAddr, "ChatClient")
  {
    client_.setConnectionCallback(
        std::bind(&AdasTcpClient::onConnection, this, _1));
    client_.setMessageCallback(
        std::bind(&AdasTcpClient::onMessage, this, _1, _2, _3));
    client_.enableRetry();
  }

void AdasTcpClient::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
                << conn->peerAddress().toIpPort() << " is "
                << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(mutex_);
    if (conn->connected())
    {
        connection_ = conn;
    }
    else
    {
        connection_.reset();
    }
}

void AdasTcpClient::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    // // muduo send big endian data, actually develper promise send little endian data
    // V2xAdasMsgHeader head;
    // head.msgId = static_cast<uint16_t>(ntohs(buf->readInt16()));
    // LOG_INFO << "recieve buff: msgid = " << head.msgId; 

    // if (msgDispatcher.find(head.msgId) != msgDispatcher.end())
    // {
    //     // demo版本，未取出header所有数据
    //     msgDispatcher[head.msgId](reinterpret_cast<char*>(&head), static_cast<u_int16_t>(sizeof(head)));
    // }

    // printf("recieve v2x data\n");
    // client剥离V2xAdasMsgHeader头， 读取msgId.   muduo库需要更换，因此本处先略过
    CDD_FUSION_EVENT_QUEUE.push({0x101, buf->peek(), static_cast<uint16_t>(buf->readableBytes())});
    printf ("push!\n");

}

void AdasTcpClient::connect()
{
    client_.connect();
}

void AdasTcpClient::disconnect()
{
    client_.disconnect();
}
