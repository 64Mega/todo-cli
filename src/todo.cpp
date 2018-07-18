// Testing compilation

#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <stdlib.h>

#ifdef __WIN32__    
    #define HOMEDIR "USERPROFILE"
    #define SEPARATOR "\\"
#else
    #define HOMEDIR "HOME"
    #define SEPARATOR "/"
#endif

#define PATHMAX 4096

FILE* todo_handle;

char* get_userdir();

std::queue<std::string>* proc_commands(char** argv, int argc) {
    std::queue<std::string>* res = new std::queue<std::string>();
    for(int i = 1; i < argc; i++) {
        res->push(std::string(argv[i]));
    }

    return res;    
}

void process_todos(std::queue<std::string>* command_list, bool local_mode = false);

void read_todos(bool local_mode);

void usage();

int main(int argc, char** argv) {
    std::queue<std::string>* commands = proc_commands(argv, argc);
    if(commands->empty()) {
        read_todos(true);
    } else {
        process_todos(commands, true);
    }
    delete commands;        

    printf("\n");

    return(0);
}

void usage() {     
    printf("todo usage\n");
    printf("----------\n");
    printf("Begin with todo init.\n\n");
    printf("COMMAND          DESCRIPTION\n");
    printf("-------          -----------\n");
    printf("init           - Initialize .todo file\n");
    printf("global <cmd>   - Perform any of these commands on the user-level .todo\n");
    printf("add <todo>     - Add a new todo\n");
    printf("<no args>      - Display todos in .todo\n");
    printf("complete <num> - Complete the given todo\n");
    printf("sweep          - Clear all completed todos from the list\n");
    printf("delete <num>   - Remove a todo from the list\n");
    printf("purge          - Removed ALL TODOS from the list\n");
}

char* get_userdir() {
    // Get user directory
    char *path = getenv(HOMEDIR);    
    return path;
}

void todo_init(bool is_local) {
    char* userdir = get_userdir();
    char path[PATHMAX];
    if(is_local) {
        sprintf(path, ".%s.todo", SEPARATOR);
    } else {
        sprintf(path, "%s%s.todo", userdir, SEPARATOR);
    }

    FILE* fp = fopen(path, "w");
    if(fp) {
        printf("Created %s\n", path);
    } else {
        printf("Error creating %s\n", path);
    }
    fclose(fp);
}

std::vector<std::string> get_todos(bool local_mode)  {
    char* userdir = get_userdir();
    char path[PATHMAX];
    if(local_mode) {
        sprintf(path, ".%s.todo", SEPARATOR);
    } else {
        sprintf(path, "%s%s.todo", userdir, SEPARATOR);
    }

    FILE* fp = fopen(path, "r");
    std::vector<std::string> buffer;
    if(fp) {
        do {
            char line[PATHMAX];
            char* res = fgets(line, PATHMAX, fp);
            if(res) {
                buffer.push_back(std::string(res));
            } else {
                break;
            }
        } while(true);
    } else {
        usage();        
    }

    fclose(fp);
    return buffer;
}

void read_todos(bool local_mode) {
    auto buffer = get_todos(local_mode);
    if(buffer.size() == 0) {
        printf("No todos added yet!\n");
        return;
    }
    for(int i = 0; i < buffer.size(); i++) {
        std::string todo = buffer[i];
        if(todo.length() < 2) { continue; }
        char state = todo[0];
        todo = todo.substr(2);
        printf("[%2d]%s %s", i, state == 'X' ? "" : "[Complete]", todo.c_str());
    }
}

std::string concat_remainder(std::queue<std::string>* command_list) {
    std::string s;
    while(!command_list->empty()) {
        s += command_list->front();
        command_list->pop();
        if(!command_list->empty()) { s += " "; }
    }

    return s;
}

void write_todo(std::string msg, bool local_mode) {
    char* userdir = get_userdir();
    char path[PATHMAX];
    if(local_mode) {
        sprintf(path, ".%s.todo", SEPARATOR);
    } else {
        sprintf(path, "%s%s.todo", userdir, SEPARATOR);
    }

    FILE* fp = fopen(path, "a");
    if(fp) {
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "X:%s\n", msg.c_str());
        printf("Added!\n");
    } else {
        printf("Error adding todo!\n");
    }
    fclose(fp);
}

void rewrite_todos(std::vector<std::string> buffer, bool local) {
    char* userdir = get_userdir();
    char path[PATHMAX];
    if(local) {
        sprintf(path, ".%s.todo", SEPARATOR);
    } else {
        sprintf(path, "%s%s.todo", userdir, SEPARATOR);
    }

    FILE* fp = fopen(path, "w");
    if(fp) {
        for(int i = 0; i < buffer.size(); i++) {
            if(buffer[i].length() == 0) { continue; }
            fprintf(fp, "%s", buffer[i].c_str());
        }        
    } else {
        printf("Error updating todos!\n");
    }
    fclose(fp);
}

void complete_todo(unsigned int index, bool local) {
    auto todos = get_todos(local);
    if(index > todos.size() - 1) {
        printf("No such item!\n");
    } else {
        todos[index][0] = 'O';
        printf("Completed!\n");
    }

    rewrite_todos(todos, local);
}

void delete_todo(unsigned int index, bool local) {
    auto todos = get_todos(local);
    if(index > todos.size() - 1) {
        printf("No such item!\n");
    } else {
        todos[index] = "";
        printf("Deleted!\n");
    }

    rewrite_todos(todos, local);
}

void sweep_todos(bool local) {
    auto todos = get_todos(local);
    int count = 0;
    for(int i = 0; i < todos.size(); i++) {
        if(todos[i].length() > 2) {
            if(todos[i][0] != 'X') {
                todos[i] = "";
                count++;
            }
        }
    }
    
    rewrite_todos(todos, local);
    printf("Removed %d completed todos from the list!\n", count);
}

void purge_todos(bool local) {
    auto todos = get_todos(local);
    int count = 0;
    for(int i = 0; i < todos.size(); i++) {
        if(todos[i].length() > 2) {            
            todos[i] = "";
            count++;            
        }
    }
    
    rewrite_todos(todos, local);
    printf("Removed %d completed todos from the list!\n", count);
}

void process_todos(std::queue<std::string>* command_list, bool local_mode) {
    static bool local = local_mode;

    // Recursion guard
    if(command_list->empty()) { 
        return; 
    }
    
    std::string command = command_list->front();
    // std::transform(command.begin(), command.end(), command.begin(), ::tolower);    
    command_list->pop();

    if(command == "global") {
        local = false;        
        if(command_list->empty()) {
            read_todos(local);
        }
    } else
    if(command == "init") {
        todo_init(local);
    } else 
    if(command == "add") {
        if(command_list->empty()) {
            printf("No todo to add!\n");
        } else {
            write_todo(concat_remainder(command_list), local);
        }
    } else
    if(command == "complete") {
        if(command_list->empty()) {
            printf("You need to pass in a todo ID to complete!\n");            
        } else {
            unsigned int idx = atoi(command_list->front().c_str());
            command_list->pop();
            complete_todo(idx, local);
        }
    } else
    if(command == "sweep") {
        sweep_todos(local);
    } else
    if(command == "delete") {
        if(command_list->empty()) {
            printf("You need to pass in a todo ID to delete!\n");            
        } else {
            unsigned int idx = atoi(command_list->front().c_str());
            command_list->pop();
            delete_todo(idx, local);
        }
    } else 
    if(command == "purge") {
        purge_todos(local);
    } else
    if(command == "help") {
        while(!command_list->empty()) { command_list->pop(); }
        usage();
    } else {
        while(!command_list->empty()) { command_list->pop(); }
        usage();
    }
    
    process_todos(command_list);    
}