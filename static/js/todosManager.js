// Function to re-render the list on the client side after an action
async function fetchAndRenderTodos() {
    try {
        const response = await fetch('/api/todos');

        if (!response.ok) {
            console.error('Failed to fetch todos:', response.statusText);
            alert('Failed to load todos. See console.');
            return;
        }
        const todos = await response.json();
        renderTodos(todos);
    } catch (error) {
        console.error('Error fetching todos:', error);
        alert('Error loading todos. See console.');
    }
}
function renderTodos(todos) {
    const todoList = document.getElementById('todo-list');
    todoList.innerHTML = ''; // Clear existing list

    if (!todos || todos.length === 0) {
        todoList.innerHTML = '<li>No todos yet! Add one above.</li>';
        return;
    }

    todos.forEach(todo => {
        const listItem = document.createElement('li');
        listItem.dataset.id = todo.id;

        const checkbox = document.createElement('input');
        checkbox.type = 'checkbox';
        checkbox.onchange = () => toggleTodo(todo.id);

        // Correctly set class and checkbox state
        if (todo.completed) {
            listItem.classList.add('completed');
            checkbox.checked = true;
        } else {
            listItem.classList.remove('completed'); // Ensure class is removed
            checkbox.checked = false;
        }

        const titleSpan = document.createElement('span');
        titleSpan.className = 'title';
        titleSpan.textContent = todo.title;

        const descriptionSpan = document.createElement('span');
        descriptionSpan.className = 'description';
        descriptionSpan.textContent = todo.description || '';

        const deleteButton = document.createElement('button');
        deleteButton.className = 'delete-btn';
        deleteButton.textContent = 'Delete';
        deleteButton.onclick = () => deleteTodo(todo.id);

        listItem.appendChild(checkbox);
        listItem.appendChild(titleSpan);
        listItem.appendChild(descriptionSpan);
        listItem.appendChild(deleteButton);
        todoList.appendChild(listItem);
    });
}
async function addTodo() {
    const titleInput = document.getElementById('todo-title');
    const descriptionInput = document.getElementById('todo-description');
    const title = titleInput.value.trim();
    const description = descriptionInput.value.trim();

    if (!title) {
        alert('Title cannot be empty!');
        return;
    }

    try {
        const response = await fetch('/api/todos', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ title, description, completed: false }),
        });

        if (response.ok) {
            titleInput.value = '';
            descriptionInput.value = '';
            await fetchAndRenderTodos(); // Refresh the list client-side
        } else {
            const errorData = await response.json().catch(() => ({ message: response.statusText }));
            alert('Failed to add todo: ' + (errorData.message || 'Unknown error'));
        }
    } catch (error) {
        console.error('Error adding todo:', error);
        alert('Error adding todo. See console for details.');
    }
}

async function toggleTodo(id) {
    try {
        const response = await fetch(`/api/todos/${id}/toggle`, {
            method: 'POST',
        });

        if (response.ok) {
            const updatedTodo = await response.json(); // Get the updated todo from the response
            updateSingleTodoInDOM(updatedTodo); // Update only the changed item
        } else {
            alert('Failed to toggle todo status.');
            // Optional: If the API call fails, you might want to revert the checkbox
            // visually to its state before the click. This would involve finding the
            // checkbox and setting its 'checked' property. For simplicity, we'll
            // rely on a full refresh if an error occurs or let the user retry.
        }
    } catch (error) {
        console.error('Error toggling todo:', error);
        alert('Error toggling todo. See console for details.');
    }
}

// NEW helper function to update a single todo item in the DOM
function updateSingleTodoInDOM(todo) {
    const listItem = document.querySelector(`li[data-id='${todo.id}']`);
    if (!listItem) {
        console.warn(`Could not find list item with id ${todo.id} to update. Falling back to full refresh.`);
        fetchAndRenderTodos(); // Fallback if the specific item isn't found
        return;
    }

    const checkbox = listItem.querySelector('input[type="checkbox"]');

    // Update class and checkbox state
    if (todo.completed) {
        listItem.classList.add('completed');
        if (checkbox) checkbox.checked = true;
    } else {
        listItem.classList.remove('completed');
        if (checkbox) checkbox.checked = false;
    }

    // You could also update title/description if they were part of the 'updatedTodo'
    // const titleSpan = listItem.querySelector('.title');
    // if (titleSpan) titleSpan.textContent = todo.title;
    // const descriptionSpan = listItem.querySelector('.description');
    // if (descriptionSpan) descriptionSpan.textContent = todo.description || '';
}

async function deleteTodo(id) {
    if (!confirm('Are you sure you want to delete this todo?')) {
        return;
    }
    try {
        const response = await fetch(`/api/todos/${id}`, {
            method: 'DELETE',
        });

        if (response.ok || response.status === 204) {
            await fetchAndRenderTodos(); // Refresh the list
        } else {
            alert('Failed to delete todo.');
        }
    } catch (error) {
        console.error('Error deleting todo:', error);
        alert('Error deleting todo. See console for details.');
    }
}
document.addEventListener('DOMContentLoaded', () => {
    const themeToggleButton = document.getElementById('theme-toggle-button');
    const currentTheme = localStorage.getItem('theme');

    // Apply saved theme on load
    if (currentTheme === 'light') {
        document.body.classList.add('light-mode');
    }

    if (themeToggleButton) {
        themeToggleButton.addEventListener('click', () => {
            document.body.classList.toggle('light-mode');
            let theme = 'dark';
            if (document.body.classList.contains('light-mode')) {
                theme = 'light';
            }
            localStorage.setItem('theme', theme); // Save preference
        });
    }
});

// Initial list is rendered by Mustache on the server.
// JavaScript functions above are for interactions after the page loads.
// If you want to ensure the client-side is fully synced on load,
// you could optionally call fetchAndRenderTodos() once when the DOM is ready:
// document.addEventListener('DOMContentLoaded', fetchAndRenderTodos);
// However, this would mean the client always re-renders what the server just sent.
// The Mustache rendering should be sufficient for the initial state.