#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
// #include <errno.h>

#define MAX_INPUT 1024

//structure to hold background process info
//one for pid another points to next bg_process(linked struct)
typedef struct bg_process {
    pid_t pid;
    struct bg_process *next;
} bg_process_t;
// a handler before new bg_process is generated
static bg_process_t *bg_processes = NULL;
//volatile for detecting changes which should not be optimize by compiler
static volatile sig_atomic_t chd_flag = 0;

static void print_prompt() {
    printf("mish> ");
    fflush(stdout);
}
//assign a new space for bg_process
static void add_bg_process(pid_t pid) {
    bg_process_t *new_process = malloc(sizeof(bg_process_t));
    new_process->pid = pid;
    new_process->next = bg_processes;
    bg_processes = new_process;
}
//free the space after the process is finished and printed
void remove_bg_process(pid_t pid) {
    //double pointer because struct is itself like an array handling multiple elements
    bg_process_t **current = &bg_processes;
    //free the space for each until the last elements(NULL) is reached
    while (*current != NULL) {
        if ((*current)->pid == pid) {
            bg_process_t *temp = *current;
            *current = (*current)->next;
            free(temp);
            return;
        }
        current = &(*current)->next;
    }
}

void chd_handler(int sig) {
    chd_flag = 1;
}

static void int_handler(int sig){
    printf("\n");
}

void handle_processes() {
    int status;
    pid_t pid;
    //collect the zombie processes, -1 to collect all of them
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Exit status [%d] = %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Signal [%d] =%d\n", pid, WTERMSIG(status));
        }
        remove_bg_process(pid);
    }
}

void execute_command(char *input) {
    // divided by two for the buffer like space or somethings to handle, +1 for NULL
    char *args[MAX_INPUT/2+1];
    int background = 0;
    //points to first token
    char *token = strtok(input, " \t\n");
    int i = 0;

    while (token != NULL) {
        //detect of bg symbol exists, else cpy the token
        if (strcmp(token, "&") == 0) {
            background = 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    //last byte for termination
    args[i] = NULL;
    //no command entered
    if (i == 0) {
        return;
    }

    pid_t pid = fork();
    //enter chd process successfully
    if (pid == 0) {

        if (background) {
            signal(SIGINT, SIG_IGN);
        }
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    }
    //enter failed
    else if (pid < 0) {
        perror("fork");
    }
    //default
    else {

        if (!background) {
            //setup block mask
            sigset_t mask, oldmask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGCHLD);
            sigprocmask(SIG_BLOCK, &mask, &oldmask);
            //wait for signal
            while (!chd_flag) {
                sigsuspend(&oldmask);
            }
            chd_flag = 0;
            //restore signal mask
            sigprocmask(SIG_SETMASK, &oldmask, NULL);

            handle_processes();
        } else {
            add_bg_process(pid);
            printf("Started [%d]\n", pid);
        }
    }
}

int main() {
    //handle chd process
    struct sigaction sa;
    sa.sa_handler = chd_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    //handle ctrl c signal
    struct sigaction sa_int;
    sa_int.sa_handler=int_handler;
    sigfillset(&sa_int.sa_mask);
    sa_int.sa_flags=0;
    sigaction(SIGINT,&sa_int,NULL);

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    char input[MAX_INPUT];

    while (1) {

        handle_processes();
        print_prompt();

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        //handle ended background processes
        if (chd_flag) {
            handle_processes();
            chd_flag = 0;
        }

        execute_command(input);
    }

    return 0;
}