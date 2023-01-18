//
// Created by Dawen on 1/17/23.
//

#include "Connection.h"
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>

#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"

Connection::Connection(EventLoop *loop, Socket *sock) : loop_(loop), sock_(sock) {
    if (loop_ != nullptr) {
        channel_ = new Channel(loop_, sock_);
        channel_->EnableRead();
        channel_->UseET();
    }
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection() {
    if (loop_ != nullptr) {
        delete channel_;
    }
    delete sock_;
    delete read_buffer_;
    delete send_buffer_;
}

void Connection::Read() {
    ASSERT(state_ = State::Connected, "connection state is disconnected!");
    read_buffer_->Clear();
    if (sock_->IsNonBlocking()) {
        ReadNonBlocking();
    }
}

void Connection::ReadNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[1024];
    while(true) {
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading \n");
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            break;
        }
        else if (bytes_read == 0) {
            printf("read EOF, client fd %d disconnected \n", sockfd);
            state_ = State::Closed;
            Close();
            break;
        }
        else {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            Close();
            break;
        }
    }
}

void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf, send_buffer_->ToStr(), send_buffer_->Size());
    int data_size = send_buffer_->Size();
    int data_left = data_size;
    while (data_left >0) {
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN) {
            printf("continue writing \n");
            continue;
        }
        if (bytes_write == -1 && errno == EINTR) {
            break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::ReadBlocking() {
    int sockfd = sock_->GetFd();
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0 ) {
        read_buffer_->Append(buf, bytes_read);
    } else if (bytes_read == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", sockfd);
        state_ = State::Closed;
    } else if (bytes_read == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
}

void Connection::WriteBlocking() {
    int sockfd = sock_->GetFd();
    ssize_t bytes_write = write(sockfd, send_buffer_->ToStr(), send_buffer_->Size());
    if(bytes_write == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
}

void Connection::Send(std::string msg) {
    SetSendBuffer(msg.c_str());
    Write();
}

void Connection::Business() {
    Read();
    on_message_callback_(this);
}

void Connection::Close() {
    delete_connection_callback_(sock_);
}

void Connection::SetDeleteConnectionCallback(std::function<void(Socket *)> const &callback) {
    delete_connection_callback_ = callback;
}
void Connection::SetOnConnectCallback(std::function<void(Connection *)> const &callback) {
    on_connect_callback_ = callback;
//     channel_->SetReadCallback([this]() { on_connect_callback_(this); });
}

void Connection::SetOnMessageCallback(std::function<void(Connection *)> const &callback) {
    on_message_callback_ = callback;
    std::function<void()> bus = std::bind(&Connection::Business, this);
    channel_->SetReadCallback(bus);
}

void Connection::GetlineSendBuffer() { send_buffer_->Getline(); }

Socket *Connection::GetSocket() { return sock_; }






