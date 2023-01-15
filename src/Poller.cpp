//
// Created by Dawen on 1/15/23.
//

#include <sys/event.h>
#include "Poller.h"
#include "util.h"


#ifdef OS_LINUX

Poller::Poller() {
    fd_ = kqueue();
    ErrorIf()
}