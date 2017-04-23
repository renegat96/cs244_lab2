#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>
#include <set>

/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */

  /* Add member variables here */

  const double init_cwnd_size = 20;
  const double decrease_mult = 0.5;
  const double increase_add = 1;

  double cwnd;

  /* List of packets that have not been acknowledged within a timeout interval.
        - first - time sent by sender clock (ms).
        - second - sequence number of the packet. */
  std::set<std::pair<uint64_t, uint64_t>> unacked;

  /* Set the current window size, in datagrams, approximately. */
  void set_window_size( double );

public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug );

  /* Get current window size, in datagrams */
  unsigned int window_size( void );

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received );

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms( void );

};

#endif
