/*
 * pruebas.c
 *
 *  Created on: 10/9/2017
 *      Author: utnso
 */
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
  Function Declarations for builtin shell commands:
 */
int lsh_exit(char **args);
int lsh_format(char **args);
int lsh_rm(char **args);
int lsh_rename(char **args);
int lsh_mv(char **args);
int lsh_cat(char **args);
int lsh_mkdir(char **args);
int lsh_cpfrom(char **args);
int lsh_cpto(char **args);
int lsh_cpblock(char **args);
int lsh_md5(char **args);
int lsh_ls(char **args);
int lsh_info(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "exit",
  "format",
  "rm",
  "rename",
  "mv",
  "cat",
  "mkdir",
  "cpfrom",
  "cpto",
  "cpblock",
  "md5",
  "ls",
  "info"
};

int (*builtin_func[]) (char **) = {
  &lsh_exit,
  &lsh_format,
  &lsh_rm,
  &lsh_rename,
  &lsh_mv,
  &lsh_cat,
  &lsh_mkdir,
  &lsh_cpfrom,
  &lsh_cpto,
  &lsh_cpblock,
  &lsh_md5,
  &lsh_ls,
  &lsh_info
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;
}

/**
	@brief Builtin command: format.
	@param args List of args. Not examined.
	@return Always return 1, to continue executing.
*/
int lsh_format(char **args)
{
	if (args[1] != NULL){
		fprintf(stderr, "lsh: incorrect number of arguments\n");
	} else {
	printf("Estoy andando\n");
	}
	return 1;
}

/**
	@brief Builtin command: rm -d
	@param args List of args. args[0] is "rm". args[1] is "-d". args[2] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_rm_d(char **args)
{
	if (args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm -d\"\n"); /*puede haber error aca, ojo*/
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia eliminar un directorio\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: rm -b
	@param args List of args. args[0] is "rm". args[1] is "-b". args[2] is the file path. args[3] is the block path. args[4] is the copy number.
	@return Always return 1, to continue executing.
*/
int lsh_rm_b(char **args)
{
	if (args[2] == NULL || args[3] == NULL || args[4] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm -b\"\n"); /*puede haber error aca, ojo*/
	} else {
		if(args[5] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo debería eliminar la copia de un bloque\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: rm
	@param args List of args. args[0] is "rm". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_rm(char **args)
{
	if(args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rm\"\n");
	} else {
		if(strcmp(args[1], "-d") == 0){
			lsh_rm_d(args);
		} else {
			if(strcmp(args[1], "-b") == 0){
				lsh_rm_b(args);
			} else {
				if(args[2] != NULL){
					fprintf(stderr, "lsh: incorrect number of arguments\n");
				} else {
					printf("Yo deberia eliminar un archivo\n");
				}
			}
		}
	}
	return 1;
}

/**
	@brief Builtin command: rename
	@param args List of args. args[0] is "rename". args[1] is the file or directory path. args[2] is the new name.
	@return Always return 1, to continue executing.
*/
int lsh_rename(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"rename\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia cambiarle el nombre a un archivo o directorio\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: mv
	@param args List of args. args[0] is "mv". args[1] is the file or directory path. args[2] is the new path.
	@return Always return 1, to continue executing.
*/
int lsh_mv(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"mv\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mover un archivo o directorio\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: cat
	@param args List of args. args[0] is "cat". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_cat(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cat\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mostrar el contenido del archivo\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: mkdir
	@param args List of args. args[0] is "mkdir". args[1] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_mkdir(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"mkdir\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia crear un directorio\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: cpfrom
	@param args List of args. args[0] is "cpfrom". args[1] is the origin file path. args[2] is yamafs directory.
	@return Always return 1, to continue executing.
*/
int lsh_cpfrom(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpfrom\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia copiar un archivo a yamafs\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: cpto
	@param args List of args. args[0] is "cpto". args[1] is the yamafs file path. args[2] is the filesystem directory.
	@return Always return 1, to continue executing.
*/
int lsh_cpto(char **args)
{
	if (args[1] == NULL || args[2] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpto\"\n");
	} else {
		if(args[3] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia copiar un archivo local a yamafs\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: cpblock
	@param args List of args. args[0] is "cpblock". args[1] is the file path. args[2] is the block number. args[3] is the node id.
	@return Always return 1, to continue executing.
*/
int lsh_cpblock(char **args)
{
	if (args[1] == NULL || args[2] == NULL || args[3] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cpblock\"\n");
	} else {
		if(args[4] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo debería crear una copia de un bloque de un archivo en el nodo dado\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: md5
	@param args List of args. args[0] is "md5". args[1] is the yamafs file path.
	@return Always return 1, to continue executing.
*/
int lsh_md5(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"md5\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia solicitar el MD5 de un archivo en yamafs\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: ls
	@param args List of args. args[0] is "ls". args[1] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_ls(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"ls\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia listar los archivos de un directorio\n");
		}
	}
	return 1;
}

/**
	@brief Builtin command: info
	@param args List of args. args[0] is "info". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_info (char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"info\"\n");
	} else {
		if(args[2] != NULL){
			fprintf(stderr, "lsh: incorrect number of arguments\n");
		} else {
			printf("Yo deberia mostrar toda la informacion de un archivo\n");
		}
	}
	return 1;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
	line = readline(">");
	if(line)
		add_history(line);
	if(!strncmp(line, "exit", 4)) {
		free(line);
		break;
	}
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}
