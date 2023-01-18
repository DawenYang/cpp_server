//
// Created by Dawen on 1/17/23.
//

#include "Acceptor.h"

#include <utility>

#include "Channel.h"

#include "Socket.h"


Acceptor::Acceptor(EventLoop *loop) : loop_(loop), sock_(nullptr), channel_(nullptr) {
    sock_ = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 1234);
    sock_->Bind(addr);
    sock_->Listen();
    channel_ = new Channel(loop_, sock_);
    std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
    channel_->SetReadyCallback(cb);
    channel_->EnableRead();
    delete addr;
}

Acceptor::~Acceptor() {
    delete channel_;
    delete sock_;
}

void Acceptor::AcceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
    clnt_sock->SetNonBlocking();
    if(new_connection_callback_) {
        new_connection_callback_(clnt_sock);
    }
    delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(const std::function<void(Socket *)> &callback) {
    new_connection_callback_ = callback;
}