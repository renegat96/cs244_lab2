#include <iostream>
#include <cmath>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
  , rtt_estimate( 1000 )
  , rtt_prop ( 1000 )
  , cwnd ( 50 )
{
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
  double goal = goal_ratio * rtt_prop;
  double f = 2 * (rtt_measure - goal_ratio) / goal_ratio;
  f = filter - f * f;
  f = max(filter/2, f);
  f = min(f, filter);
  rtt_estimate = f * rtt_estimate + (1 - f) * rtt_measure;
  rtt_prop = min( rtt_prop, (double)rtt_measure );

  //set_window_size( cwnd + 3 * (1.2*rtt_prop - rtt_estimate) / rtt_prop );

  if ( rtt_estimate > rtt_prop*1.8 )
    set_window_size ( cwnd - 0.25 * (rtt_estimate - goal) / rtt_prop );
  else if ( rtt_estimate > rtt_prop*1.6 )
    set_window_size ( cwnd - 0.15 * (rtt_estimate - goal) / rtt_prop );
    //set_window_size ( cwnd - 0.2 );
  else if ( rtt_estimate < goal && rtt_measure < 1.5*rtt_prop)
    cwnd += 35*(goal - rtt_estimate)/ (goal) / cwnd;

  uint64_t seq = sequence_number_acked + recv_timestamp_acked;
  (void)seq;

  if ( debug_ ) {
//    cerr << "At time " << timestamp_ack_received
//	 << " received ack for datagram " << sequence_number_acked
//	 << " (send @ time " << send_timestamp_acked
//	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
//	 << endl;
    cerr << "Rtt estimate: "
         << rtt_estimate
         << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return 2 * rtt_estimate;
}

void Controller::set_window_size( double window_size )
{
  if (window_size < 1)
    window_size = 1;
  cwnd = window_size;
}
