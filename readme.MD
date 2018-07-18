# Todo-CLI
Dead-simple todo list management from the command line.
Run `todo help` to get help.

![Preview](https://thumbs.gfycat.com/WetPitifulFoal-size_restricted.gif)

## What it does
`todo` implements a very simple file-based todo list system that uses normal text files.

While this does mean it's limited in functionality compared to todo list 'suites', it's easier to integrate into your command-line/terminal workflow.

Running `todo init` creates a `.todo` file in the current directory.

From this point, you can `add`, `complete`, `delete`, `sweep` and `purge` entries from the list.

To view your list, just run `todo` with no arguments, and if a `.todo` file exists in the current working directory, a list will be displayed along with the completion status of each task.

When displaying the list, all entries will be given an index entry (Starting from 0). This is used by the `complete` and `delete` commands.

For example, if you wish to mark task number 3 as complete, you run `todo complete 3`.
Similarly, to delete task number 3, you run `todo delete 3`.

`todo add` takes in a message as the remainder of its argument that can be as long as your terminal allows. Some terminals require that you quote or escape any quotes in your message.
E.G: `todo add This won't work` will cause Powershell to keep seeking input until a matching quote is found.

To get around this, just quote text like so: `todo add "Now it'll work"`

## Global Mode
In addition to running in local directories, `todo` can also run at the user level in both Windows and Linux environments.

Any of the commands can be prefixed with global to work on the user-level todo list.

For example:

`todo global init` - Creates a .todo file in the user's home directory.

`todo global` - Lists todos in the global file

`todo global add <msg>` - Adds a todo to the global file

## Dependencies
A working C++ compiler.

## Building on Windows
With any GCC-based toolchain installed, run your `make` command (`make` or `mingw32-make`) in the cloned project directory.

Feel free to add the resulting binary to your `PATH` variable for easier usage.

## Building on Linux
Tested, compiles on Linux.
Just run `make`, assuming you have GCC installed.

## Building on WSL
Same as Linux, also tested to be working.

## Code Notes
The code is messy, this is the result of a spontaneous one-night self-challenge!

I'll tidy it up a little in the future, and add more features (Like Console API/Escape Code usage for prettier output!), but for now this is what it is. 

Feel free to fork it and mess around, or make a pull request if my mixing of old C library functions and C++ library classes is particularly odious to you.

## License
Licensed under the MIT License. See LICENSE file in root of repo.