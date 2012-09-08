#ifndef RECEIVER_HH
#define RECEIVER_HH

#include <stdint.h>
#include <queue>

#include "process.hh"
#include "processforecaster.hh"

#include "deliveryforecast.pb.h"

class Receiver
{
private:
  static constexpr double MAX_ARRIVAL_RATE = 2000;
  static constexpr double BROWNIAN_MOTION_RATE = 300;
  static constexpr double OUTAGE_ESCAPE_RATE = 5;
  static const int NUM_BINS = 64;
  static const int TICK_LENGTH = 20;
  static const int MAX_ARRIVALS_PER_TICK = 30;
  static const int NUM_TICKS = 10;

  class RecvQueue {
  private:
    std::priority_queue< uint64_t, std::deque<uint64_t>, std::greater<uint64_t> > received_sequence_numbers;
    uint64_t throwaway_before;

  public:
    RecvQueue() : received_sequence_numbers(), throwaway_before( 0 ) {}

    void recv( const uint64_t seq, const uint16_t throwaway_window );
    uint64_t packet_count( void );
  };

  Process _process;

  std::vector< ProcessForecastInterval > _forecastr;

  uint64_t _time;

  int _count_this_tick;

  Sprout::DeliveryForecast _cached_forecast;

  RecvQueue _recv_queue;

public:

  Receiver();
  void warp_to( const uint64_t time ) { _time = time; }
  void advance_to( const uint64_t time );
  void recv( const uint64_t seq, const uint16_t throwaway_window );

  Sprout::DeliveryForecast forecast( void );

  int get_tick_length( void ) const { return TICK_LENGTH; }
};

#endif