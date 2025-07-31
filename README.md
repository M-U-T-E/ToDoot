# ToDoot 📝

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus)
![GCC](https://img.shields.io/badge/GCC-Required-4E8A09?style=for-the-badge&logo=gcc)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-064F8C?style=for-the-badge&logo=cmake)
![SQLite](https://img.shields.io/badge/SQLite-3-003B57?style=for-the-badge&logo=sqlite)

---

## About The Project

**ToDoot** is a minimalist web application for managing your daily tasks. It's built from the ground up using C++ and modern tools. The backend is powered by the **Crow C++ microframework** for handling HTTP requests and **SQLite** for persistent data storage, managed through the elegant `sqlite_orm` library.

The goal was to create a self-contained, high-performance To-Do application that is both easy to compile and run on a Linux system.

### Key Features ✨

* **RESTful API**: A clean API for creating, retrieving, updating, and deleting tasks.
* **Simple Web UI**: A basic but functional web interface to interact with your To-Do list.
* **Lightweight**: Built with performance in mind using efficient C++ libraries.
* **Persistent Storage**: Your tasks are saved in an SQLite database file (`db.sqlite`).

---

## Demo 🎬

<img src="https://github.com/M-U-T-E/ToDoot/blob/main/assets/Demo.gif?raw=true" alt="Demo.gif" width="500"/>

---

## Getting Started 🚀

Follow these instructions to get a local copy up and running on your Linux machine.

### Prerequisites

You'll need the **GCC C++ compiler**, CMake, and a few libraries installed through your distribution's package manager. `sqlite_orm` is bundled with the project and does not need to be installed separately.

* **GCC C++ Compiler** (**Required**: This project is currently only compatible with GCC).
* **CMake** (version 3.16 or higher).
* **Crow framework**, **Zlib**, and **SQLite3** development libraries.

### Installation & Build

1.  **Install System Dependencies**
    Install the required development libraries using your distro's package manager.

    * On **Debian/Ubuntu-based systems**:
        ```sh
        sudo apt update
        sudo apt install libcrow-dev zlib1g-dev libsqlite3-dev
        ```
    * On **Fedora/CentOS-based systems**:
        ```sh
        sudo dnf install crow-devel zlib-devel sqlite-devel
        ```
    * On **Arch Linux-based systems**:
        ```sh
        yay -Syu base-devel cmake crow zlib sqlite
        # you can use paru as well
        ```

2.  **Clone Project & Sub-dependencies**
    Clone the main repository and the `sqlite_orm` library into the `vendor` directory.

    ```sh
    # Clone the ToDoot project
    git clone https://github.com/M-U-T-E/ToDoot.git
    cd ToDoot

    # Clone sqlite_orm into the vendor directory
    git clone https://github.com/fnc12/sqlite_orm.git vendor/sqlite_orm
    ```

3.  **Configure and Build with CMake**
    Make sure your system is configured to use GCC as the default C++ compiler.

    ```sh
    # Create a build directory
    mkdir build && cd build

    # Run CMake (it will find sqlite_orm automatically)
    cmake ..

    # Compile the project
    cmake --build .
    ```

4.  **Run the Server**
    Once the build is complete, you can find the executable in the `build` directory.

    ```sh
    ./ToDoot
    ```

5.  **Access the Application**
    Open your web browser and navigate to `http://localhost:18080/home`.

---

## API Endpoints 🌐

The server provides a REST API to manage To-Do items. The base URL is `http://localhost:18080`.

| Method | Endpoint                 | Description                                                              | Request Body (JSON)                                                  | Response Body (Success)                                                              |
| :----- | :----------------------- | :----------------------------------------------------------------------- | :------------------------------------------------------------------- | :----------------------------------------------------------------------------------- |
| `GET`  | `/`                      | Redirects (301) to `/home`.                                              | *None* | *Empty Body* |
| `GET`  | `/home`                  | Serves the main `index.html` page.                                       | *None* | HTML Content                                                                         |
| `GET`  | `/api/todos`             | Retrieves a list of all To-Do items.                                     | *None* | `[{"id": 1, "title": "...", "description": "...", "completed": false}]`               |
| `POST` | `/api/todos`             | Adds a new To-Do item. **description** and **completed** are optional.   | `{"title": "New Todo", "description": "...", "completed": false}`    | `{"id": 2, "title": "New Todo", "description": "...", "completed": false}`            |
| `GET`  | `/api/todos/<id>`        | Retrieves a single To-Do item by its ID.                                 | *None* | `{"id": 1, "title": "...", "description": "...", "completed": false}`                 |
| `PUT`  | `/api/todos/<id>`        | Updates a To-Do's fields. All fields are optional.                       | `{"title": "Updated", "description": "...", "completed": true}`      | `{"id": 1, "title": "Updated", "description": "...", "completed": true}`             |
| `POST` | `/api/todos/<id>/toggle` | Toggles the completion status of a To-Do.                                | *None* | `{"id": 1, "title": "...", "description": "...", "completed": true}`                  |
| `DELETE`| `/api/todos/<id>`      | Deletes a specific To-Do item by its ID.                                 | *None* | `{"message": "Todo deleted successfully"}`                                           |

---

## Project Structure 📂

```
ToDoot/
├── CMakeLists.txt
├── main.cpp
├── build/            # Build output (created after running CMake)
├── include/          # Header files
│   ├── database/
│   │   └── DatabaseManager.h
│   ├── handlers/
│   │   └── TodoHandlers.h
│   └── models/
│       └── Todo.h
├── src/              # Source files
│   ├── database/
│   │   └── DatabaseManager.cpp
│   └── handlers/
│       └── TodoHandlers.cpp
├── static/           # Static assets
│   ├── css/
│   │   └── style.css
│   ├── js/
│   │   └── todosManager.js
│   └── index.html
└── vendor/           # Vendored dependencies
└── sqlite_orm/
```
---

## License 📜

Distributed under the MIT License. See `LICENSE` file for more information.

---

## Author 👨‍💻

**M-U-T-E** - [GitHub Profile](https://github.com/M-U-T-E)


