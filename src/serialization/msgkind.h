//lang::CwC

#pragma once

enum class MsgKind { Ack, Nack, Put, Reply,  Get, WaitAndGet, Status,
    Kill,   Register,  Directory 
};
