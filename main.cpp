#include <iostream>
#include "crow.h"
#include "include/database/DatabaseManager.h"
#include "include/handlers/TodoHandlers.h"   // For API handlers
#include "include/models/Todo.h"             // For Todo struct

// Helper function to convert your Todo struct to a crow::json::wvalue
// This is needed to pass data to the Mustache template.
// Ensure the field names ('id', 'title', 'description', 'completed') match your Todo struct.
crow::json::wvalue todo_to_json_for_mustache(const Todo& todo) {
    crow::json::wvalue item;
    item["id"] = todo.id;
    item["title"] = todo.title; // If your struct uses 'text', change this to todo.text
    item["description"] = todo.description; // If your struct doesn't have description, you can omit or set to ""
    item["completed"] = todo.completed;
    return item;
}

int main() {
    // Initialize Database Manager
    db::DatabaseManager dbManager("todoot.sqlite");
    dbManager.syncSchema(); // Ensure table exists
    if (dbManager.getAllTodos().empty()) {
        dbManager.addTodo("Learn Crow", "Understand Mustache templating and crow.", false);
        dbManager.addTodo("Build App", "Complete the Todo List application.", true);
        dbManager.addTodo("Doing homework", "", false);
        std::cout << "Inserted initial todo items." << std::endl;
    }
    crow::SimpleApp app;
    // Set base directory for Mustache templates
    crow::mustache::set_global_base("./static");
    // --- Route to serve the main HTML page with Mustache ---
    CROW_ROUTE(app, "/home")
    ([&dbManager]() { // Capture dbManager to access database
        crow::mustache::context ctx;
        auto todos_from_db = dbManager.getAllTodos();
        crow::json::wvalue::list json_todos_list; // This will be a list of todo JSON objects
        for (const auto& todo : todos_from_db)
            json_todos_list.emplace_back(todo_to_json_for_mustache(todo));
        ctx["todos"] = std::move(json_todos_list); // "todos" is the key used in index.html {{#todos}}

        // CROW_LOG_INFO << "Rendering /home with " << todos_from_db.size() << " todos.";
        return crow::mustache::load("index.html").render(ctx);
    });
    // --- Redirect root to /home ---
    CROW_ROUTE(app, "/")([](){
        crow::response res;
        res.code = 301; // Permanent Redirect
        res.set_header("Location","/home");
        return res;
    });
    // --- API Routes for Todos (from your existing code) ---
    CROW_ROUTE(app, "/api/todos").methods(crow::HTTPMethod::Get)
        ([&dbManager](){ return handlers::listTodos(dbManager); });

    CROW_ROUTE(app, "/api/todos").methods(crow::HTTPMethod::Post)
        ([&dbManager](const crow::request& req){ return handlers::addTodo(req, dbManager); });

    CROW_ROUTE(app, "/api/todos/<int>").methods(crow::HTTPMethod::Get)
        ([&dbManager](const int id){ return handlers::getTodo(id, dbManager); });

    CROW_ROUTE(app, "/api/todos/<int>").methods(crow::HTTPMethod::Put)
        ([&dbManager](const crow::request& req, const int id){ return handlers::updateTodo(id, req, dbManager); });

    CROW_ROUTE(app, "/api/todos/<int>").methods(crow::HTTPMethod::Delete)
        ([&dbManager](const int id){ return handlers::deleteTodo(id, dbManager); });

    CROW_ROUTE(app, "/api/todos/<int>/toggle").methods(crow::HTTPMethod::Post)
        ([&dbManager](const int id){ return handlers::toggleTodo(id, dbManager); });

    app.port(18080).multithreaded().run();

}