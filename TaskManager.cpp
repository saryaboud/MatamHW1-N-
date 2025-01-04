#include <iostream>
#include "TaskManager.h"
#include <functional>

TaskManager::TaskManager() = default;

int TaskManager::length_() const {
    int length = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        if (arr[i].getName() != "")length += 1;
    }
    return length;
}

void TaskManager::assignTask(const std::string &personName, const Task &task) {
    int length = this->length_();
    int taskid = 0;
    for (int i = 0; i < length; i++) {
        taskid += arr[i].getTasks().length();
    }
    for (int i = 0; i < length; i++) {
        if (this->arr[i].getName() == personName) {
            Task newtask(task);
            newtask.setId(taskid);
            arr[i].assignTask(newtask);
            taskid++;
            return;
        }
    }
    if (length >= MAX_PERSONS) throw std::runtime_error("no space");
    arr[length] = Person(personName);
    Task newtask(task);
    newtask.setId(taskid);
    arr[length].assignTask(newtask);
    taskid++;
    length++;
}

void TaskManager::completeTask(const string &personName) {
    int length = this->length_();
    for (int i = 0; i < length; i++) {
        if (this->arr[i].getName() == personName) {
            arr[i].completeTask();
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    int length = this->length_();
    for (int i = 0; i < length; i++) {
        SortedList<Task> list = arr[i].getTasks();
        auto func = [type,priority](Task task) -> Task {
            if (task.getType() == type) {
                Task *new_task = new Task(task.getPriority() + priority, task.getType()
                                          , task.getDescription());
                new_task->setId(task.getId());
                task = *new_task;
                delete new_task; // caution
            }
            return task;
         };
        SortedList<Task> l = list.apply(func);
        arr[i].setTasks(l);
        delete &l; // caution
    }
}

void TaskManager::printAllEmployees() const {
    int length = this->length_();
    for (int i = 0; i < length; i++) {
        std::cout << arr[i] << std::endl;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> newlist;
    int length = this->length_();
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
    int length = this->length_();
    auto *newtask = new TaskManager;
    for (int i = 0; i < length; i++) {
        SortedList<Task> list = arr[i].getTasks();
        auto func = [type](Task task) -> bool {
            if (task.getType() == type) {
                return true;
            }
            return false;
        };
        SortedList<Task> l = list.filter(func);
        newtask->arr[i].setTasks(l);
        delete &l; // caution
    }
    newtask->printAllTasks();
    delete newtask;
}
