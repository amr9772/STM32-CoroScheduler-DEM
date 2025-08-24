#ifndef _COROTYPE_H
#define _COROTYPE_H


#include <coroutine>
#include <exception>
#include "uart_stream.h"

struct CoroType {
    struct promise_type { 
        int m_value;
        bool done{};
        CoroType get_return_object() { return CoroType(this); }    
        std::suspend_always initial_suspend() { return {}; }        
        std::suspend_always final_suspend() noexcept{ 
          done = true;
          return {}; }
      
        void unhandled_exception() noexcept
        {
            std::rethrow_exception(std::current_exception());
        }
        std::suspend_always yield_value(int val) {
            m_value = val; 
            return {};
        }
        /*
        void return_value(int val){
            uart_cout << "Returning value " << uart_endl;
             m_value = val;
         }*/
         
      
        void return_void() {
        }
       
       
    };
    CoroType(promise_type* p)
         : m_handle(std::coroutine_handle<promise_type>::from_promise(*p)) {}
    CoroType(const CoroType&) = delete; // Coroutine handles are not copyable, so we delete this
    CoroType& operator=(const CoroType&) = delete;
    
     CoroType(CoroType&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }
    
    // Explicitly define a move assignment operator
    CoroType& operator=(CoroType&& other) noexcept
    {
        if (this != &other)
        {
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }
    /*
    ~CoroType() {
        if (m_handle) {
            m_handle.destroy();
        }
    }
    */
    ~CoroType() = default;
    std::coroutine_handle<promise_type>   m_handle;
    // Member to store the handle of the coroutine that co_awaits this object
    std::coroutine_handle<> m_awaiting_coro; 
    bool await_ready() const {
        return false; // Always suspend
    }

    void await_suspend(std::coroutine_handle<> awaiting_coro) {
        // Save the handle of the coroutine that is awaiting us.
        m_awaiting_coro = awaiting_coro;

        // At this point, you'd typically schedule the awaited coroutine (`this->m_handle`)
        // to run in your event loop. Since you have a `NonBlockingSYSTICK_Delay`,
        // this is where you'd register it with the system's scheduler or
        // event loop to be resumed later.
    }

    void await_resume() {
        // This is called when the awaiting coroutine is resumed.
        // You might return a value here if needed.
    }
};


#endif