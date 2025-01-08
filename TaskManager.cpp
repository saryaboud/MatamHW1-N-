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
    int length = this->length_(); // Get the current number of occupied slots.

    // Explicit check: Ensure length is within bounds.
    if (length > MAX_PERSONS) {
        throw std::logic_error("Length exceeds maximum allowed persons. Internal state corruption?");
    }

    // Search for the person in the array.
    for (int i = 0; i < length; i++) {
        if (this->arr[i].getName() == personName) {
            // Person found, assign the task.
            Task newTask(task);
            newTask.setId(taskid);
            arr[i].assignTask(newTask);
            taskid++; // Increment task ID for the next task.
            return;
        }
    }

    // If the person is not found and thezzn.
    if (length < MAX_PERSONS) {
        arr[length] = Person(personName); // Add new person at the next free slot.
        Task newTask(task);
        newTask.setId(taskid);
        arr[length].assignTask(newTask); // Assign the task to the new person.
        taskid++; // Increment task ID for the next task.
    } else {
        // If the array is full and the person was not found, throw an exception.
        throw std::runtime_error("No space to add a new person, and the person was not found.");
    }
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
