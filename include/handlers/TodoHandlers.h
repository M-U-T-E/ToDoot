#pragma once
#ifndef TODOHANDLERS_H
#define TODOHANDLERS_H

#include "crow.h"
#include "database/DatabaseManager.h" // Needs access to the database manager

namespace handlers {
    crow::response listTodos(db::DatabaseManager& dbManager);
    crow::response addTodo(const crow::request& req, db::DatabaseManager& dbManager);
    crow::response getTodo(int id, db::DatabaseManager& dbManager);
    crow::response updateTodo(int id, const crow::request& req, db::DatabaseManager& dbManager);
    crow::response deleteTodo(int id, db::DatabaseManager& dbManager);
    crow::response toggleTodo(int id, db::DatabaseManager& dbManager);

} // namespace handlers
#endif //TODOHANDLERS_H
