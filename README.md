# Mini Shell (msh)

**Student Name:** INDAKWA PAULINE APPHIE
**Student ID:** SCT212-0062/2024
**Unit:** ICS 2305 – Systems Programming
**Programming Language:** C

## 1. Project Description

This project implements a simple command-line shell called **Mini Shell (msh)** using the C programming language. The shell accepts user commands, executes external programs, supports built-in commands, handles background processes, and provides basic output redirection.

The implementation demonstrates fundamental operating system concepts, particularly process creation, program execution, process synchronization, and signal handling.

## 2. Building and Running the Program

### Requirements

- Ubuntu/Linux environment
- GCC compiler
- GNU Make

### Build using Make

```bash
make
```

### Run the Mini Shell

```bash
./msh
```

### Build directly using GCC

```bash
gcc -Wall -Wextra -std=c11 -o msh msh.c
```

### Clean generated executable

```bash
make clean
```

## 3. Implementation Checklist

The following table summarizes the implementation of the eight practical tasks.

- [x] **Task 1: Shell prompt and input** – Displays the shell prompt, accepts user input using `getline()`, and exits cleanly when Ctrl+D is pressed.
- [x] **Task 2: Command tokenization** – Uses `strtok()` to separate commands and arguments using spaces and tabs. Empty input is ignored, and the argument list ends with `NULL`.
- [x] **Task 3: External command execution** – Uses `fork()`, `execvp()`, and `waitpid()` to execute external commands and handle execution errors.
- [x] **Task 4: Built-in commands** – Implements `cd`, `help`, and `exit`.
- [x] **Task 5: Exit status reporting** – Displays the previous command's exit status in the prompt and handles normal exits and signal termination.
- [x] **Task 6: Background execution** – Supports commands ending with `&`, displays the background process ID, and checks for completed background processes without blocking.
- [x] **Task 7: Output redirection** – Supports `>` to create or truncate a file and redirect command output into it.
- [x] **Task 8: Signal handling** – Handles Ctrl+C so that a foreground command can be interrupted without terminating the shell.

## 4. Design and Implementation Explanation

The Mini Shell uses process management functions provided by the operating system to execute commands. When a user enters an external command, the shell first reads and tokenizes the input into a command name and its arguments. The argument list is prepared in the format required by the execution functions, with a `NULL` pointer marking its end.

The `fork()` system call creates a new child process from the shell process. This allows the shell to execute another program while keeping the original shell process running. The child process then uses `execvp()` to replace its current program with the requested command. The function searches for the executable using the directories specified in the `PATH` environment variable and passes the supplied arguments to the program. If execution fails, an error is reported and the child process terminates.

The parent process uses `waitpid()` to collect the child's termination status when executing a foreground command. This prevents the shell from displaying the next prompt before the foreground command finishes. The status is interpreted using `WIFEXITED()`, `WEXITSTATUS()`, and `WIFSIGNALED()` so that the shell can report normal exit codes and signal-related statuses.

The `cd` and `exit` commands are implemented as built-ins because they must affect the shell process itself. If `cd` were executed in a child process, only that child would change its working directory, leaving the parent shell unchanged. Similarly, `exit` must terminate the shell process rather than a separate child.

Background processes are handled without making the shell wait for them immediately. The shell periodically checks completed background children using `waitpid()` with the `WNOHANG` option. This allows it to collect their termination statuses without blocking, helping prevent completed background processes from remaining as zombies.

## 5. Known Limitations

Known limitations include:

- Quoted arguments containing spaces are not fully supported.
- Pipes (`|`) are not implemented.
- Input redirection (`<`) and append redirection (`>>`) are not implemented.
- Environment-variable expansion is not implemented.
- Command history and tab completion are not implemented.
- Output redirection supports the basic `>` operator.

## 6. Sources and References

The following resources were used as references during the development and understanding of the project:

- Linux manual pages: `fork(2)`, `execvp(3)`, `waitpid(2)`, `getline(3)`, `dup2(2)`, and `open(2)`.
- GNU C Library documentation.
- Course practical instructions and learning materials.
- Class notes on Linux processes, system calls, and command-line programming.
ChatGPT was used as a learning tool to explain systems programming concepts, assist with understanding C system calls, and provide guidance during implementation, and testing.
