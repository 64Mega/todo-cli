// todo-cli
// (c) 2018 Daniel Lawrence (64Mega)
// MIT License - See LICENSE for details
// --
// Dead-simple todo system for command line/terminal use.
// --
// Be prepared to see some messy code. I'm freely mixing the C standard library and C++ standard library,
// because for some things (String management, containers) I prefer the C++ library, and for certain operations
// (EG: File operations, output) I vastly prefer the classics.
// --

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>

// Note that not all compilers define the __WIN32__ symbol on Windows, but this mostly applies to older
// and weirder compilers. If you run into compilation issues, just add a define to the Makefile.
#ifdef __WIN32__    
    #define HOMEDIR "USERPROFILE"
    #define SEPARATOR "\\"    

    #include <fcntl.h>
    #include <io.h>    

    // Windows needs std::wcout for Unicode character output, so I'm cheating by #define-ing the
    // problem away.
    #define ccout std::wcout

    // Glyph definitions for Windows. Windows wants a wchar_t* for UTF-16 output
    #ifdef UNICODEGLYPHS
        const wchar_t* GLYPH_COMPLETE = L"\U00002705";
        const wchar_t* GLYPH_INCOMPLETE = L" ";
    #else
        const wchar_t* GLYPH_COMPLETE = L"[X]";
        const wchar_t* GLYPH_INCOMPLETE = L"[ ]";
    #endif

    void init() {
        #ifdef UNICODEGLYPHS
            // Set up better console output
            // Note that this is generally unsupported, but works on all versions of Windows since XP.
            // If it ever fails in the future, remove this line and set the glyphs above to something
            // within the standard ASCII set.
            _setmode(_fileno(stdout), _O_U16TEXT);        
        #endif
    }
#else
    #define HOMEDIR "HOME"
    #define SEPARATOR "/"

    // Linux and Mac are fine with a standard std::cout for their Unicode output.
    #define ccout std::cout

    // Glyph definitions for the rest of the world.
    #ifdef UNICODEGLYPHS
        const char* GLYPH_COMPLETE = "\U00002705";
        const char* GLYPH_INCOMPLETE = " ";
    #else
        const char* GLYPH_COMPLETE = "[X]";
        const char* GLYPH_INCOMPLETE = " ";
    #endif

    void init() {
        // No specific setup required
    }
#endif

#define VERSION "0.0.3a"

// Just a guess, trying to keep things 'simple' between Windows and Linux systems. So far it works on both,
// but if somebody decides to make a project 4096 characters deep and tries running todo and it breaks, feel
// free to submit a snarky pull request.
#define PATHMAX 4096

// Declarations. Better explanations of what they do further along, at their definition locations.

char* get_userdir();
std::queue<std::string>* proc_commands(char** argv, int argc);
void process_todos(std::queue<std::string>* command_list, bool local_mode = false);
void read_todos(bool local_mode);
void usage();

// Entrypoint

int main(int argc, char** argv) {
    // Initialize better output for the console if we're on Windows
    init();

    // The command loop is very simple. Convert arguements into individual std::string components,
    // recursively call process_todos until no commands are left.

    std::queue<std::string>* commands = proc_commands(argv, argc);
    if(commands->empty()) {
        read_todos(true);
    } else {
        process_todos(commands, true);
    }
    delete commands;            

    return(0);
}

// Converts command line arguments into individual std::string objects, dumped into an std::queue<>
// Note that the return result, res, is heap allocated, so it's deletion must be handled by the caller.
std::queue<std::string>* proc_commands(char** argv, int argc) {
    std::queue<std::string>* res = new std::queue<std::string>();
    for(int i = 1; i < argc; i++) {
        res->push(std::string(argv[i]));
    }

    return res;    
}

// Prints out usage information

void usage() {    
    ccout << "todo usage\n";
    ccout << "----------\n";
    ccout << "Begin with todo init.\n\n";
    ccout << "COMMAND          DESCRIPTION\n";
    ccout << "-------          -----------\n";
    ccout << "init           - Initialize .todo file\n";
    ccout << "global <cmd>   - Perform any of these commands on the user-level .todo\n";
    ccout << "add <todo>     - Add a new todo\n";
    ccout << "<no args>      - Display todos in .todo\n";
    ccout << "complete <num> - Complete the given todo\n";
    ccout << "sweep          - Clear all completed todos from the list\n";
    ccout << "delete <num>   - Remove a todo from the list\n";
    ccout << "purge          - Removed ALL TODOS from the list\n";
    ccout << "help           - Display this list\n";
    ccout << "version        - Show version information\n";
}

// Gets the current user directory for global mode.
// See the #ifdef block at the top of the file to see the 'magic values' for HOMEDIR that make
// the difference between Windows and Linux.

char* get_userdir() {
    // Get user directory
    char *path = getenv(HOMEDIR);    
    return path;
}

// Checks if the first char of each string match, for command shorthand checks.
bool match_one_or_more(std::string input, std::string compare) {
    if(input.length() > 0 && compare.length() > 0) {
        return input[0] == compare[0];
    }
    return false;
}

// Create a .todo file or re-initialize an existing one.
// If is_local is true, it creates the file in the current working directory, otherwise
// it creates it in the user folder.

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
        ccout << "Created " << path << std::endl;
        fclose(fp);
    } else {
        ccout << "Error creating " << path << std::endl;
    }    
}

// Try to read a .todo file, grab lines, put them in an std::vector<> and
// return a copy.
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
        fclose(fp);
    } else {
        usage();        
    }    
    
    return buffer;
}

// Prints out todos with their numbers, strips off the the 
// first two bytes (They'll either be X: or O: for Incomplete/Complete, respectively).
void read_todos(bool local_mode) {
    auto buffer = get_todos(local_mode);
    if(buffer.size() == 0) {
        ccout << "Nothing to do!" << std::endl;
        return;
    }
    for(int i = 0; i < buffer.size(); i++) {
        std::string todo = buffer[i];
        if(todo.length() < 2) { continue; }
        std::replace(todo.begin(), todo.end(), '\n', ' ');
        std::replace(todo.begin(), todo.end(), '\r', ' ');
        char state = todo[0];
        todo = todo.substr(2);

        ccout << "[" << i << "] ";
        ccout << (state == 'X' ? GLYPH_INCOMPLETE : GLYPH_COMPLETE) << "\t";
        ccout << todo.c_str() << std::endl; 
    }
}

// This function takes the remaining strings in a queue and concatenates them.
// Used when preparing the todo message for writing to the file.
std::string concat_remainder(std::queue<std::string>* command_list) {
    std::string s;
    while(!command_list->empty()) {
        s += command_list->front();
        command_list->pop();
        if(!command_list->empty()) { s += " "; }
    }

    return s;
}

// Writes a fresh todo to the given .todo file.
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
        ccout << "Added!" << std::endl;
        fclose(fp);
    } else {
        ccout << "Error adding todo." << std::endl;
    }    
}

// Takes a copy of a vector of todos from another function, usually modified, then
// writes them back out to their respective .todo file.
// There are obviously tidier ways of doing this.
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
            // This guard here skips any blank lines. This is a little 'cheat' to allow for
            // easier deletion. Just set the respective line to be blank, and it'll be 'skipped' 
            // during output.
            if(buffer[i].length() == 0) { continue; }
            fprintf(fp, "%s", buffer[i].c_str());
        }        
        fclose(fp);        
    } else {
        ccout << "Error updating todos." << std::endl;
    }    
}

// Marks a todo as complete, then calls rewrite_todos() to 
// apply the change.
void complete_todo(unsigned int index, bool local) {
    auto todos = get_todos(local);
    if(index > todos.size() - 1) {
        ccout << "No item with index " << index << "." << std::endl;
    } else {
        todos[index][0] = 'O';
        ccout << "Item " << index << " completed!" << std::endl;
    }

    rewrite_todos(todos, local);
}

// Deletes a todo and then calls rewrite_todos() to apply the change.
void delete_todo(unsigned int index, bool local) {
    auto todos = get_todos(local);
    if(index > todos.size() - 1) {
        ccout << "No item with index " << index << "." << std::endl;
    } else {
        todos[index] = "";
        ccout << "Item " << index << " deleted." << std::endl;
    }

    rewrite_todos(todos, local);
}

// Iterates through the todo list and removes any that don't have the "Incomplete" bytes (X:)
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
    ccout << "Removed " << count << " completed todo" << (count > 1 ? "s" : "") << " from the list!" << std::endl;     
}

// Removes ALL todos from the list, regardless of state.
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
    ccout << "Deleted " << count << " todo" << (count > 1 ? "s" : "") << " from the list." << std::endl;     
}

// Command line processor. Recursive.
void process_todos(std::queue<std::string>* command_list, bool local_mode) {
    static bool local = local_mode;

    // Recursion guard
    if(command_list->empty()) { 
        return; 
    }
    
    std::string command = command_list->front();
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);    
    command_list->pop();

    // Pretty straightforward state-changes where necessary. Some of the functions "eat" the remainder
    // of the command inputs (if any) to make sure there aren't multiple triggerings of, for instance,
    // the usage() function.
    if(match_one_or_more(command, "global")) {
        local = false;        
        if(command_list->empty()) {
            read_todos(local);
        }
    } else
    if(match_one_or_more(command, "init")) {
        todo_init(local);
    } else 
    if(match_one_or_more(command, "add")) {
        if(command_list->empty()) {
            ccout << "No todo message provided." << std::endl;
        } else {
            write_todo(concat_remainder(command_list), local);
        }
    } else
    if(match_one_or_more(command, "complete")) {
        if(command_list->empty()) {
            ccout << "No todo ID provided." << std::endl;
        } else {
            unsigned int idx = atoi(command_list->front().c_str());
            command_list->pop();
            complete_todo(idx, local);
        }
    } else
    if(match_one_or_more(command, "sweep")) {
        sweep_todos(local);
    } else
    if(match_one_or_more(command, "delete")) {
        if(command_list->empty()) {
            ccout << "No todo ID provided." << std::endl;          
        } else {
            unsigned int idx = atoi(command_list->front().c_str());
            command_list->pop();
            delete_todo(idx, local);
        }
    } else 
    if(match_one_or_more(command, "purge")) {
        purge_todos(local);
    } else
    if(match_one_or_more(command, "version")) {
        ccout << "todo-cli version " << VERSION << std::endl;       
        while(!command_list->empty()) { command_list->pop(); }
    } else
    if(match_one_or_more(command, "help")) {
        while(!command_list->empty()) { command_list->pop(); }
        usage();
    } else {
        while(!command_list->empty()) { command_list->pop(); }
        usage();
    }
    
    process_todos(command_list);    
}
