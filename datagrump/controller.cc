#include <iostream>
#include <cmath>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
  , rtt_estimate( 100 )
  , rtt_prop ( 100 )
  , cwnd ( 20 )
{
  cout << "Starting with low-pass filter = " << filter << endl;
  cout << "Starting with target = " << goal_ratio << endl;
}

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{
  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << cwnd << endl;
  }

  return round(cwnd);
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */

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
  /* Default: take no action */

  uint64_t rtt_measure = timestamp_ack_received - send_timestamp_acked;
  rtt_estimate = filter * rtt_estimate + (1 - filter) * rtt_measure;
  rtt_prop = min( rtt_prop, (double)rtt_measure );
  double goal = goal_ratio * rtt_prop;

  if ( rtt_estimate > rtt_prop*1.8 )
    set_window_size ( cwnd - 0.25 * (rtt_estimate - goal) / goal );
  else if ( rtt_estimate > rtt_prop*1.6 )
    set_window_size ( cwnd - 0.125 * (rtt_estimate - goal) / goal );
  else if ( rtt_estimate < goal)
    cwnd += (2 + 1/(goal_ratio-1) * (goal - rtt_estimate) / (goal) ) / cwnd;

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	       << " received ack for datagram " << sequence_number_acked
	       << " (send @ time " << send_timestamp_acked
	       << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	       << endl;
    cerr << "Rtt estimate: "
         << rtt_estimate
         << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return 2 * rtt_prop;
}

void Controller::set_window_size( double window_size )
{
  if (window_size < 1)
    window_size = 1;
  cwnd = window_size;
}
