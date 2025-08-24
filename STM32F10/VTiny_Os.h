#ifndef VTINYOS
#define VTINYOS

#include <coroutine>
#include <array>
#include <optional>
#include "CoroType.h"


template<size_t MAX_TASKS>
class CoroutineScheduler {
public:
    void add_task(CoroType task) {
        if (task_count < MAX_TASKS) {
            tasks[task_count].emplace(std::move(task));
            task_count++;
        }
    }

    void run() {
    while (true) {
        for (size_t i = 0; i < task_count;) {
            if (!(tasks[i]->m_handle.done())) {
                tasks[i]->m_handle.resume();
                i++;
            } else {
                tasks[i]->m_handle.destroy();
                // Move the last task to the current position to fill the gap
                if (i != task_count - 1) {
                    tasks[i].emplace(std::move(*tasks[task_count - 1]));
                }
                tasks[task_count - 1].reset();
                task_count--;
            }
        }
    }
  }

private:
    std::array<std::optional<CoroType>, MAX_TASKS> tasks;
    size_t task_count = 0;
};

#endif