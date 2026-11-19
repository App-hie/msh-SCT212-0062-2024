
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

/* Reap completed background processes to prevent zombies. */
void reap_background_processes(void)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("\n[Background process %ld finished]\n", (long)pid);
    }
}

/* Convert a child's exit status into the shell's status value. */
int get_exit_status(int status)
{
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }

    return 1;
}

/* Display information about the shell. */
void display_help(void)
{
    printf("Mini Shell (msh) - Available commands:\n");
    printf("  cd [directory]  Change the current directory\n");
    printf("  help            Display this help message\n");
    printf("  exit            Exit the shell\n");
    printf("  command         Run an external command\n");
    printf("  command &       Run a command in the background\n");
    printf("  command > file  Redirect command output to a file\n");
}

/* Execute an external command. */
int execute_command(char **args, int background, char *output_file)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("msh: fork failed");
        return 1;
    }

    if (pid == 0) {
        /* The child process should respond normally to Ctrl+C. */
        signal(SIGINT, SIG_DFL);

        /* Handle output redirection in the child process only. */
        if (output_file != NULL) {
            int fd = open(output_file,
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);

            if (fd < 0) {
                perror("msh: cannot open output file");
                _exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("msh: dup2 failed");
                close(fd);
                _exit(1);
            }

            close(fd);
        }

        execvp(args[0], args);

        /* This line runs only if execvp fails. */
        perror("msh: command not found");
        _exit(127);
    }

    if (background) {
        printf("[Started background process %ld]\n", (long)pid);
        return 0;
    }

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result < 0 && errno == EINTR);

    if (result < 0) {
        perror("msh: waitpid failed");
        return 1;
    }

    return get_exit_status(status);
}

int main(void)
{
    char *line = NULL;
    size_t capacity = 0;
    ssize_t characters_read;
    int last_status = 0;
    int should_exit = 0;

    /* The shell itself ignores Ctrl+C. */
    signal(SIGINT, SIG_IGN);

    while (!should_exit) {
        reap_background_processes();

        printf("msh[%d]> ", last_status);
        fflush(stdout);

        characters_read = getline(&line, &capacity, stdin);

        /* Ctrl+D causes getline to return -1 at end-of-file. */
        if (characters_read == -1) {
            printf("\n");
            break;
        }

        /* Store the individual words entered by the user. */
        char **tokens = NULL;
        size_t token_count = 0;
        char *token = strtok(line, " \t\r\n");

        while (token != NULL) {
            char **new_tokens = realloc(
                tokens, (token_count + 1) * sizeof(char *)
            );

            if (new_tokens == NULL) {
                perror("msh: memory allocation failed");
                free(tokens);
                last_status = 1;
                tokens = NULL;
                token_count = 0;
                break;
            }

            tokens = new_tokens;
            tokens[token_count++] = token;
            token = strtok(NULL, " \t\r\n");
        }

        /* Ignore empty lines and lines containing only whitespace. */
        if (token_count == 0) {
            free(tokens);
            continue;
        }

        /* Check whether the final token is the background symbol. */
        int background = 0;

        if (strcmp(tokens[token_count - 1], "&") == 0) {
            background = 1;
            token_count--;
        }

        if (token_count == 0) {
            free(tokens);
            continue;
        }

        /* Prepare the argument list and detect output redirection. */
        char **args = calloc(token_count + 1, sizeof(char *));

        if (args == NULL) {
            perror("msh: memory allocation failed");
            free(tokens);
            last_status = 1;
            continue;
        }

        char *output_file = NULL;
        size_t argc = 0;
        size_t i = 0;

        while (i < token_count) {
            if (strcmp(tokens[i], ">") == 0) {
                if (i + 1 >= token_count) {
                    fprintf(stderr, "msh: missing filename after >\n");
                    last_status = 2;
                    output_file = NULL;
                    argc = 0;
                    break;
                }

                output_file = tokens[i + 1];
                i += 2;
            } else {
                args[argc++] = tokens[i++];
            }
        }

        args[argc] = NULL;

        if (argc == 0) {
            free(args);
            free(tokens);
            continue;
        }

        /* Built-in command: exit */
        if (strcmp(args[0], "exit") == 0) {
            should_exit = 1;
        }

        /* Built-in command: help */
        else if (strcmp(args[0], "help") == 0) {
            display_help();
            last_status = 0;
        }

        /* Built-in command: cd */
        else if (strcmp(args[0], "cd") == 0) {
            const char *directory = args[1];

            if (directory == NULL) {
                directory = getenv("HOME");
            }

            if (directory == NULL || chdir(directory) != 0) {
                perror("msh: cd failed");
                last_status = 1;
            } else {
                last_status = 0;
            }
        }

        /* All other commands are external programs. */
        else {
            last_status = execute_command(
                args, background, output_file
            );
        }

        free(args);
        free(tokens);
    }

    free(line);
    return 0;
}
