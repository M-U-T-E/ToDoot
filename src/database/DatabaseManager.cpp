#include "database/DatabaseManager.h" // Use relative path from src
#include <iostream> // For logging errors or info

namespace db {

DatabaseManager::DatabaseManager(const std::string& db_path) : storage_(make_todo_storage(db_path)) {

}

void DatabaseManager::syncSchema() {
    try {
        storage_.sync_schema(true); // true for verbose output
        std::cout << "Database schema synchronized." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error synchronizing schema: " << e.what() << std::endl;
    }
}

std::optional<Todo> DatabaseManager::getTodoById(int id){
    try {
        std::unique_ptr<Todo> todo_ptr = storage_.get_pointer<Todo>(id);
        if (todo_ptr) // validate unique_ptr
            return *todo_ptr; // Dereference the pointer and copy/move into the optional
        return std::nullopt; // The element was not found

    } catch (const std::system_error& e) { // sqlite_orm throws std::system_error for "not found"
        // This catch block might not even be strictly necessary if get_pointer
        // returns nullptr for "not found" as is typical.
        // However, it's good for catching other potential system errors from the DB.
        if (e.code() == std::errc::no_such_file_or_directory || e.code().value() == SQLITE_NOTFOUND) {
            // This specific check might be too granular; often get_pointer just returns nullptr
            std::cerr << "Todo with ID " << id << " not found (system_error)." << std::endl;
        } else {
            std::cerr << "Error getting todo by ID " << id << ": " << e.what() << std::endl;
        }
        return std::nullopt;
    } catch (const std::exception& e) { // Catch other potential exceptions
        std::cerr << "Generic error getting todo by ID " << id << ": " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::vector<Todo> DatabaseManager::getAllTodos(){
    try {
        return storage_.get_all<Todo>();
    } catch (const std::exception& e) {
        std::cerr << "Error getting all todos: " << e.what() << std::endl;
        return {}; // Return empty vector on error
    }
}

int DatabaseManager::addTodo(const std::string& title,const std::string& description, bool completed) {
    try {
        Todo new_todo{-1, title, description, completed}; // -1 for autoincrement ID
        return storage_.insert(new_todo);
    } catch (const std::exception& e) {
        std::cerr << "Error adding todo: " << e.what() << std::endl;
        return -1; // Indicate error
    }
}

bool DatabaseManager::updateTodo(const Todo& todo) {
    try {
        storage_.update(todo);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating todo ID " << todo.id << ": " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::deleteTodo(int id) {
    try {
        storage_.remove<Todo>(id);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting todo ID " << id << ": " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::toggleTodoCompleted(int id) {
    try {
        auto todo_ptr = storage_.get_pointer<Todo>(id);
        if (!todo_ptr)
            return false; // element not found

        todo_ptr->completed = !todo_ptr->completed; //toggle the todo
        storage_.update(*todo_ptr); // update the element via ORM
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error toggling todo ID " << id << ": " << e.what() << std::endl;
        return false;
    }
}


} // namespace db
