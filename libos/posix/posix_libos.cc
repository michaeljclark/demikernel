// -*- mode: c++; c-file-style: "k&r"; c-basic-offset: 4 -*-
/***********************************************************************
 *
 * libos/libposix/posix.cc
 *   POSIX implementation of libos interface
 *
 * Copyright 2018 Irene Zhang  <irene.zhang@microsoft.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************/

#include "posix_queue.hh"
#include <dmtr/annot.h>
#include <dmtr/libos.h>
#include <libos/common/memory_queue.hh>
#include <libos/common/io_queue_api.hh>

#include <memory>

static std::unique_ptr<dmtr::io_queue_api> ioq_api;

int dmtr_init(int argc, char *argv[])
{
    DMTR_NULL(ioq_api.get());

    dmtr::io_queue_api *p = NULL;
    DMTR_OK(dmtr::io_queue_api::init(p, argc, argv));
    ioq_api = std::unique_ptr<dmtr::io_queue_api>(p);
    ioq_api->register_queue_ctor(dmtr::io_queue::MEMORY_Q, dmtr::memory_queue::new_object);
    ioq_api->register_queue_ctor(dmtr::io_queue::NETWORK_Q, dmtr::posix_queue::new_object);
    return 0;
}

int dmtr_queue(int *qd_out)
{
    DMTR_NOTNULL(qd_out);
    DMTR_NOTNULL(ioq_api.get());

    DMTR_OK(ioq_api->queue(*qd_out));
    return 0;
}

int dmtr_socket(int *qd_out, int domain, int type, int protocol)
{
    DMTR_NOTNULL(qd_out);
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->socket(*qd_out, domain, type, protocol);
}

int dmtr_listen(int qd, int backlog)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->listen(qd, backlog);
}

int dmtr_bind(int qd, const struct sockaddr * const saddr, socklen_t size)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->bind(qd, saddr, size);
}

int dmtr_accept(int *qd_out, struct sockaddr *saddr_out, socklen_t *size_out, int sockqd)
{
    DMTR_NOTNULL(qd_out);
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->accept(*qd_out, saddr_out, size_out, sockqd);
}

int dmtr_connect(int qd, const struct sockaddr *saddr, socklen_t size)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->connect(qd, saddr, size);
}

int dmtr_close(int qd)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->close(qd);
}

int dmtr_push(dmtr_qtoken_t *qtok_out, int qd, const dmtr_sgarray_t *sga)
{
    DMTR_NOTNULL(qtok_out);
    DMTR_NOTNULL(sga);
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->push(*qtok_out, qd, *sga);
}

int dmtr_pop(dmtr_qtoken_t *qtok_out, int qd)
{
    DMTR_NOTNULL(qtok_out);
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->pop(*qtok_out, qd);
}

int dmtr_peek(dmtr_sgarray_t *sga_out, dmtr_qtoken_t qt)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->peek(sga_out, qt);
}

int dmtr_wait(dmtr_sgarray_t *sga_out, dmtr_qtoken_t qt)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->wait(sga_out, qt);
}

int dmtr_poll(dmtr_sgarray_t *sga_out, dmtr_qtoken_t qt)
{
    DMTR_NOTNULL(ioq_api.get());

    return ioq_api->poll(sga_out, qt);
}
