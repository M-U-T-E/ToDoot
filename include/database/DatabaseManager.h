#pragma once
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "sqlite_orm/sqlite_orm.h"
#include "models/Todo.h"

namespace db {
    inline auto make_todo_storage(const std::string& db_path) {
        using namespace sqlite_orm;
        return make_storage(db_path,
                            make_table("todos",
                                       make_column("id", &Todo::id, primary_key().autoincrement()),
                                       make_column("title", &Todo::title),
                                       make_column("description", &Todo::description),
                                       make_column("completed", &Todo::completed)));
    }

    using Storage = decltype(make_todo_storage("")); // Type alias for the storage

    class DatabaseManager {
    public:
        explicit DatabaseManager(const std::string& db_path);

        void syncSchema(); // To create/update tables

        std::optional<Todo> getTodoById(int id); // optional return value in case of the element not being found
        std::vector<Todo> getAllTodos();
        int addTodo(const std::string& title, const std::string& description, bool completed = false);
        bool updateTodo(const Todo& todo);
        bool deleteTodo(int id);
        bool toggleTodoCompleted(int id);

    private:
        Storage storage_;
    };
} // namespace db

#endif //DATABASEMANAGER_H