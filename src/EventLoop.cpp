//
// Created by Dawen on 1/15/23.
//

#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Poller.h"

EventLoop::EventLoop() {
    poller_ = new Poller();
}

EventLoop::~EventLoop() {
    Quite();
    delete poller_;
}

void EventLoop::Loop() {
    while (!quit_) {
        std::vector<Channel *> chs;
        chs = poller_->Poll();
        for (auto &ch: chs) {
            ch->HandleEvent();
        }
    }
}

void EventLoop::Quite() {
    quit_ = true;
}

void EventLoop::UpdateChannel(Channel *ch) {
    poller_->UpdateChannel(ch);
}

void EventLoop::DeleteChannel(Channel *ch) {
    poller_->DeleteChannel(ch);
}