#include <iostream>
#include "TaskManager.h"
#include <functional>

TaskManager::TaskManager() = default;

int TaskManager::length() const {
    int length = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        if (!arr[i].getName().empty())length += 1;
    }
    return length;
}

void TaskManager::assignTask(const std::string &personName, const Task &task) {
    int length = this->length();
    for (int i = 0; i < length; i++) {
        if (this->arr[i].getName() == personName) {
            arr[i].assignTask(task);
            return;
        }
    }
    if (length == MAX_PERSONS) throw std::runtime_error("no_space");
    Person p(personName);
    arr[length] = p;
    arr[length].assignTask(task);
}

void TaskManager::completeTask(const string &personName) {
    int length = this->length();
    for (int i = 0; i < length; i++) {
        if (this->arr[i].getName() == personName) {
            arr[i].completeTask();
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    int length = this->length();
    for (int i = 0; i < length; i++) {
        SortedList<Task> list = arr[i].getTasks();
        auto func = [type,priority](Task task) -> Task {
            if (task.getType() == type) {
                Task *new_task = new Task(task.getPriority() + priority, task.getType()
                                          , task.getDescription());
                new_task->setId(task.getId());
                return *new_task;
            }
            return task;
        };
        list = list.apply(func);
        arr[i].setTasks(list);
    }
}

void TaskManager::printAllEmployees() const {
    int length = this->length();
    for (int i = 0; i < length; i++) {
        std::cout << arr[i] << std::endl;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> newlist;
    int length = this->length();
    for (int i = 0; i < length; i++) {
        SortedList<Task> list = arr[i].getTasks();
        for (auto it = list.begin(); it != list.end(); ++it) {
            newlist.insert(*it);
        }
    }
    for (auto it = newlist.begin(); it != newlist.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    int length = this->length();
    auto *newtask = new TaskManager;
    for (int i = 0; i < length; i++) {
        SortedList<Task> list = arr[i].getTasks();
        auto func = [type](Task task) -> bool {
            if (task.getType() == type) {
                return true;
            }
            return false;
        };
        list = list.filter(func);
        newtask->arr[i].setTasks(list);
    }
    newtask->printAllTasks();
    delete newtask;
}
