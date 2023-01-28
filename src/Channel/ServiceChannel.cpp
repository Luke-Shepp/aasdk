/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#include <f1x/aasdk/IO/PromiseLink.hpp>
#include <f1x/aasdk/Channel/ServiceChannel.hpp>

// Ensure the correct io_service() is called based on boost version
#ifndef GET_IO_SERVICE
#if BOOST_VERSION >= 107000
#define GET_IO_SERVICE(s) ((boost::asio::io_context&)(s).context())
#else
#define GET_IO_SERVICE(s) ((s).get_io_service())
#endif
#endif

namespace f1x
{
namespace aasdk
{
namespace channel
{

ServiceChannel::ServiceChannel(boost::asio::io_service::strand& strand,
                               messenger::IMessenger::Pointer messenger,
                               messenger::ChannelId channelId)
    : strand_(strand)
    , messenger_(std::move(messenger))
    , channelId_(channelId)
{

}

void ServiceChannel::send(messenger::Message::Pointer message, SendPromise::Pointer promise)
{
    auto sendPromise = messenger::SendPromise::defer(GET_IO_SERVICE(strand_));
    io::PromiseLink<>::forward(*sendPromise, std::move(promise));
    messenger_->enqueueSend(std::move(message), std::move(sendPromise));
}

}
}
}
