#include <iostream>
#include "TaskManager.h"
#include <functional>

TaskManager::TaskManager() : taskid(0) {
}

TaskManager::~TaskManager() = default;

int TaskManager::getId() const {
    return taskid;
}

void TaskManager::setId(int id) {
    taskid = id;
}

int TaskManager::length_() const {
    int length = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        if (arr[i].getName() != "")length += 1;
    }
    return length;
}

void TaskManager::assignTask(const std::string &personName, const Task &task) {
    int length = this->length_();
    int f = 0;
    if (length < MAX_PERSONS) {
        for (int i = 0; i < length; i++) {
            if (this->arr[i].getName() == personName) {
                Task newtask(task);
                newtask.setId(taskid);
                arr[i].assignTask(newtask);
                taskid++;
                return;
            }
            f = i;
        }
    }
   if (f < MAX_PERSONS){
        arr[f] = Person(personName);
        Task newtask(task);
        newtask.setId(taskid);
        arr[f].assignTask(newtask);
        taskid++;
        return;
    }
    throw std::runtime_error("no space");
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
    if (priority <= 0) {
        return;
    }
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
    for (int i = 0; i < this->length_(); i++) {
        for (auto x: arr[i].getTasks()) {
            newlist.insert(x);
        }
    }
    // print the new node
    for (auto x: newlist) {
        std::cout << x << std::endl;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> newlist;

    for (int i = 0; i < this->length_(); i++) {
        for (auto x: this->arr[i].getTasks()) {
            if (x.getType() == type)
                newlist.insert(x);
        }
    }
    //print the new node
    for (auto x: newlist) {
        std::cout << x << std::endl;
    }
}
