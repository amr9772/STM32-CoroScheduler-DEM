#ifndef DEM_H
#define DEM_H

#include <cstdint>
#include <optional>
#include <array>
#include "uart_stream.h"


typedef enum {
    PASSED     = 0, 
    FAILED     = 1, 
    PRE_PASSED = 2,
    PRE_FAILED = 3,
} Event_Status_TypeDef;

class DEM_event
{
    private:
      int8_t Maturation_threshold{};
      int8_t Dematuration_threshold{};
      int8_t counter{};
      bool   event_status{};
      const char* event_name = nullptr;
    public:
      DEM_event() = default;
      DEM_event(const char* name, int8_t Maturation, int8_t DeMaturation)
       : event_name(name), Maturation_threshold(Maturation),Dematuration_threshold(DeMaturation){};
      ~DEM_event()= default;
      const char* get_event_name() const{
        return event_name;
      }
      void set_event_status(Event_Status_TypeDef stat){
        switch(stat){
          case PASSED:
                counter = 0;
                event_status = 0;
                break;
          case FAILED:
                counter = 0;
                event_status = 1;
                break;
          case PRE_PASSED:
                if(event_status){
                  if(counter>0){
                    counter = 0;
                  }
                  else{
                    counter--;
                  }
                  if(counter == Dematuration_threshold){
                    event_status = 0;
                  }
                  break;
                }
                else{
                  break;
                }
                
          case PRE_FAILED:
                if(!event_status){
                  if(counter<0){
                    counter = 0;
                  }
                  else{
                    counter++;
                  }
                  if(counter == Maturation_threshold){
                    event_status = 1;
                  }
                  break;
                }
                else{
                  break;
                }
          default:
                break;

        }
      }
      bool get_event_status() const {
        return event_status;
      }
};

template<uint8_t No_of_events>
class DEM
{
    private:
      std::array<std::optional<DEM_event>, No_of_events> events;
      uint8_t events_counter{};
    public:
      DEM()  = default;
      ~DEM() = default;
      DEM_event* create_event(const char* name, int8_t Maturation, int8_t DeMaturation){
          if (events_counter < No_of_events) {
            DEM_event event(name,Maturation,DeMaturation);
            events[events_counter].emplace(std::move(event));
            events_counter++;
        }
        return &(*events[events_counter - 1]);
      }
      void Show_info(){
        uart_cout << "...........................Diagnostic Info..........................." << uart_endl;
        for(int i{};i<events_counter;i++){
        uart_cout << events[i]->get_event_name() << "   Status: " << events[i]->get_event_status() << uart_endl;
        }
        uart_cout << "......................................................................" << uart_endl;
      }

};

#endif