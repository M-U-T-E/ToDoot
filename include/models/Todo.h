#pragma once
#ifndef TODO_H
#define TODO_H
#include <string>
struct Todo {
    int id;
    std::string title;
    std::string description;
    bool completed;

    // Optional: A constructor for convenience
    Todo(int i = -1, std::string t="",std::string d="", bool c = false) :
    id(i), title(std::move(t)), description(std::move(d)), completed(c) {}
};


#endif //TODO_H
