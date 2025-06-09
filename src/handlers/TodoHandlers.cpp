#include "handlers/TodoHandlers.h"
#include "models/Todo.h" // schema for Todo
#include <vector>

namespace handlers {

// Helper to convert Todo to JSON
crow::json::wvalue todoToJson(const Todo& todo) {
    crow::json::wvalue item;
    item["id"] = todo.id;
    item["title"] = todo.title;
    item["description"]= todo.description;
    item["completed"] = todo.completed;
    return item;
}

crow::response listTodos(db::DatabaseManager& dbManager) {
    auto todos = dbManager.getAllTodos();
    crow::json::wvalue::list response_todos_list; // Renamed for clarity
    // Reserve space if you expect many todos, for minor optimization
    // response_todos_list.reserve(todos.size());
    for (const auto& todo_item : todos) { // Use range-based for and const auto&
        response_todos_list.emplace_back(todoToJson(todo_item));
    }

    // Wrap the list in a wvalue
    crow::json::wvalue json_array(response_todos_list);
    return json_array;
}

crow::response addTodo(const crow::request& req, db::DatabaseManager& dbManager) {
    auto body = crow::json::load(req.body);
    if (!body || !body.has("title")) {
        return {400, "Missing 'title' in request body"};
    }
    std::string title = body["title"].s();
    // Check if description exists before trying to access it to avoid exceptions
    std::string description;
    if (body.has("description")) {
        description = body["description"].s();
    }

    if (title.empty()) {
        return {400, "'title' cannot be empty"};
    }
    bool completed = body.has("completed") ? body["completed"].b() : false;

    int new_id = dbManager.addTodo(title, description, completed);
    if (new_id != -1) {
        auto new_todo_opt = dbManager.getTodoById(new_id);
        if (new_todo_opt) {
             return {201, todoToJson(*new_todo_opt)}; // 201 Created
        }
    }
    return {500, "Failed to add todo"};
}

crow::response getTodo(int id, db::DatabaseManager& dbManager) {
    auto todo_opt = dbManager.getTodoById(id);
    if (todo_opt) {
        return {todoToJson(*todo_opt)};
    }
    return {404, "Todo not found"};
}

crow::response updateTodo(int id, const crow::request& req, db::DatabaseManager& dbManager) {
    auto body = crow::json::load(req.body);
    if (!body) {
        return {400, "Invalid JSON body"};
    }

    auto current_todo_opt = dbManager.getTodoById(id);
    if (!current_todo_opt) {
        return {404, "Todo not found to update"};
    }

    Todo updated_todo = *current_todo_opt;
    if (body.has("title")) updated_todo.title = body["title"].s();
    if (body.has("description")) updated_todo.description = body["description"].s();
    if (body.has("completed")) updated_todo.completed = body["completed"].b();

    if (dbManager.updateTodo(updated_todo)) {
        return {todoToJson(updated_todo)};
    }
    return {500, "Failed to update todo"};
}

crow::response deleteTodo(int id, db::DatabaseManager& dbManager) {
    if (dbManager.deleteTodo(id)) {
        return crow::response(204); // No Content
    }
    return {404, "Todo not found or failed to delete"};
}

crow::response toggleTodo(int id, db::DatabaseManager& dbManager) {
    if (! dbManager.toggleTodoCompleted(id))
        return {404, "Todo not found or failed to toggle"};

    auto todo_opt = dbManager.getTodoById(id);

    if (! todo_opt.has_value())
        return {500, "Todo toggled but failed to retrieve updated state."};

    return todoToJson(*todo_opt);

}


} // namespace handlers