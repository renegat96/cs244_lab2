#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
  , cwnd( init_cwnd_size )
  , rt_est( init_rt_est )
  , rt_min( init_rt_est )
  , unacked()
{}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << cwnd << endl;
  }

  return cwnd; 
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  unacked.insert(pair<uint64_t, uint64_t>(send_timestamp, sequence_number));
  if (send_timestamp > timeout_ms()) {
    auto it = unacked.upper_bound(pair<uint64_t, uint64_t>(send_timestamp - timeout_ms(), 0));
    if (it != unacked.begin()) {
      if ( debug_ ) {
        cerr << "Window reduced in half" << endl;
      }
      unacked.erase(unacked.begin(), it);
      set_window_size(cwnd * decrease_mult);
    }
  }
  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  double rt_measure = timestamp_ack_received - send_timestamp_acked;
  rt_est = filter_gain * rt_est + ( 1 - filter_gain ) * rt_measure;
  rt_min = ( rt_min < rt_measure ? rt_min : rt_measure );

  if ( rt_est < rt_min * 1.5 )
    set_window_size(cwnd + increase_add / cwnd);

  if ( unacked.erase(pair<uint64_t, uint64_t>(send_timestamp_acked, sequence_number_acked)) ) {
    /* Additive increase */
    set_window_size(cwnd + increase_add / cwnd); 
  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return rt_min * rto_gain;
}

void Controller::set_window_size( double window_size )
{
  if (window_size < 1)
    window_size = 1;
  cwnd = window_size;
}
