//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DAPP_SERVER2_CONNECTION_HPP
#define DAPP_SERVER2_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "TcpSplitPkt.h"
#include "BizDeal.h"

//#include "connection_manager.hpp"

namespace dapp {
namespace server2 {

class connection_manager;
/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_context.
  explicit connection(boost::asio::io_context& io_context, connection_manager& manager );
  explicit connection(boost::asio::ip::tcp::socket socket, connection_manager& manager);
  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();
  void write( const boost::system::error_code& e);
  void stop( );
private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  CTcpSplitPkt  splitPkt_;
  CBizDeal      bizDeal_;
    
  /// The handler used to process the incoming request.
  ///request_handler& request_handler_;

  /// Buffer for incoming data.
  boost::array<char, 8192> buffer_; // need modify

  /// The incoming request.
  ///request request_;

  /// The parser for the incoming request.
  ///request_parser request_parser_;

  ///if error, The reply to be sent back to the client.
  reply reply_;
  connection_manager& connection_manager_;

};

typedef boost::shared_ptr<connection> connection_ptr;

} // namespace server2
} // namespace http

#endif // DAPP_SERVER2_CONNECTION_HPP


