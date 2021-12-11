#include <taskflow/taskflow.hpp>
#include <iostream>

int main() {
    tf::Taskflow taskflow;
    tf::Executor executor;

    tf::Task A = taskflow.emplace([]() { std::cout << "Task A\n"; });
    tf::Task B = taskflow.emplace([]() { std::cout << "Task B\n"; });
    tf::Task C = taskflow.emplace([]() { std::cout << "Task C\n"; });
    tf::Task D = taskflow.emplace([]() { std::cout << "Task D\n"; });

    A.precede(B);
    A.precede(C);
    B.precede(D);
    C.precede(D);

    executor.run(taskflow).wait();

    return 0;
}