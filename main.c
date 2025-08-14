#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHL_RL_BUFFSIZE 1024

#define CHL_TOK_BUFFSIZE 64
#define CHL_TOK_DELIM " \t\r\n\a"

char *chl_readline(void) {
    int buffsize = CHL_RL_BUFFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffsize);
    int c;

    if(!buffer) { // buffer == NULL
        fprintf(stderr, "chl: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;

        } else {
            buffer[position] = c;
        }
        ++position;

        if (position >= buffsize){
            buffsize += CHL_RL_BUFFSIZE;
            buffer = realloc(buffer, buffsize);
            if (!buffer) {
                fprintf(stderr, "chl: allocation error\n");
                exit(EXIT_FAILURE);
            }
            
        }
        
        
    }
    


}

char **chl_splitargs(char* line) {
    int buffsize = CHL_TOK_BUFFSIZE;
    int position = 0;
    char **tokens = malloc(sizeof(char*) * buffsize);
    char *token;

    if(!tokens) {
        fprintf(stderr, "chl: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, CHL_TOK_DELIM);
    
    while (token != NULL) {
        tokens[position] = token;
        ++position;

        if (position >= buffsize)
        {
            buffsize += CHL_TOK_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "chl: allocation error\n");
                exit(EXIT_FAILURE);
            }
            
        }
        token = strtok(NULL, CHL_TOK_DELIM);
        return tokens;
    }
    
}

int chl_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) { // child
        if (execvp(args[0], args) == -1 ) {
            perror("chl");
        }
    } else if (pid < 0) { // error when forking
        perror("chl");
    } else {  // parent
        do {
        wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));

    }
    return 1;


}

void chl_loop(void) {
    char* line;
    char** args;
    int status;
    do {
        printf("> ");
        line = chl_readline();
        args = chl_splitargs(line);
        status = chl_exec(args);

        free(line);
        free(args);
    } while (status);
    
}

// declaracoes
int chl_cd(char **args);
int chl_help(char **args);
int chl_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = { // array of function pointers
  &chl_cd,
  &chl_help,
  &chl_exit
};

int chl_num_builtins() {
    return sizeof(builtin_str) / sizeof(char*);
}

int chl_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "chl: expected argument to \"cd\"\n");
    } else {
    if (chdir(args[1]) != 0) {
      perror("chl");
    }
  }
  return 1;
}

int chl_help(char **args) {
  int i;
  printf("chell : a simple shell mimic based on Stephen Brennan's LSH \n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < chl_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int chl_exit(char **args)
{
  return 0;
}

int chl_exec(char **args)
{
  int i;

  if (args[0] == NULL) {
    // empty command
    return 1;
  }

  for (i = 0; i < chl_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return chl_launch(args);
}

int main(int argc, char const **argv) {
    chl_loop();
    return EXIT_SUCCESS;
}
