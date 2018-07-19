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

Shorthand can be used for any command. For example:
`todo g a My task message` to add a global entry.

## Changelog
### Version 0.0.3
+ Refactored most output to utilize Unicode if necessary.
+ Compile-flag to use ✅ for completed tasks.

### Version 0.0.2a
+ Allow command shorthand
+ Change Makefile to compile without debug info by default

## Global Mode
In addition to running in local directories, `todo` can also run at the user level in both Windows and Linux environments.

Any of the commands can be prefixed with global to work on the user-level todo list.

For example:

`todo global init` - Creates a .todo file in the user's home directory.

`todo global` - Lists todos in the global file

`todo global add <msg>` - Adds a todo to the global file

## Dependencies
A working C++ compiler.

## Building
With any GCC-based toolchain installed, run your `make` command (`make` or `mingw32-make`) in the cloned project directory.

When rebuilding, run `make clean` followed by `make`.

It _should_ work out-of-the-box on any system that can run standard C++/C code, it doesn't use anything fancy.

Tested on:
 + Windows 10
 + Windows 10 (WSL, Ubuntu)
 + Manjaro Linux 17.1.11 (Arch Linux)
 + Mac OS High Sierra (10.13)

## Enabling ✅ output for completed tasks
In the Makefile, uncomment `-DUNICODEGLYPHS` to allow outputting `✅` instead of `[X]` for completed tasks.

Note that this doesn't work out-of-the-box on many terminals, as it requires UTF-8/16 support as well as a font that contains the required codepage and character.

Tested on:
+ ConEmu (With UTF16 mode enabled)
+ Visual Studio Code Terminal
+ Hyper
+ iTerm2 with Powerline fonts
+ ZSH with Powerline fonts

Your mileage may vary.

## Code Notes
The code is messy, this is the result of a spontaneous one-night self-challenge!

I'll tidy it up a little in the future, and add more features (Like Console API/Escape Code usage for prettier output!), but for now this is what it is. 

Feel free to fork it and mess around, or make a pull request if my mixing of old C library functions and C++ library classes is particularly odious to you.

## License
Licensed under the MIT License. See LICENSE file in root of repo.