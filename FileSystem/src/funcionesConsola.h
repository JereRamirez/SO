/*
 * funcionesConsola.h
 *
 *  Created on: 29/9/2017
 *      Author: utnso
 */

#ifndef FUNCIONESCONSOLA_H_
#define FUNCIONESCONSOLA_H_

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>




/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args);

/**
	@brief Builtin command: format.
	@param args List of args. Not examined.
	@return Always return 1, to continue executing.
*/
int lsh_format(char **args);

/**
	@brief Builtin command: rm -d
	@param args List of args. args[0] is "rm". args[1] is "-d". args[2] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_rm_d(char **args);

/**
	@brief Builtin command: rm -b
	@param args List of args. args[0] is "rm". args[1] is "-b". args[2] is the file path. args[3] is the block path. args[4] is the copy number.
	@return Always return 1, to continue executing.
*/
int lsh_rm_b(char **args);

/**
	@brief Builtin command: rm
	@param args List of args. args[0] is "rm". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_rm(char **args);

/**
	@brief Builtin command: rename
	@param args List of args. args[0] is "rename". args[1] is the file or directory path. args[2] is the new name.
	@return Always return 1, to continue executing.
*/
int lsh_rename(char **args);

/**
	@brief Builtin command: mv
	@param args List of args. args[0] is "mv". args[1] is the file or directory path. args[2] is the new path.
	@return Always return 1, to continue executing.
*/
int lsh_mv(char **args);

/**
	@brief Builtin command: cat
	@param args List of args. args[0] is "cat". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_cat(char **args);

/**
	@brief Builtin command: mkdir
	@param args List of args. args[0] is "mkdir". args[1] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_mkdir(char **args);

/**
	@brief Builtin command: cpfrom
	@param args List of args. args[0] is "cpfrom". args[1] is the origin file path. args[2] is yamafs directory.
	@return Always return 1, to continue executing.
*/
int lsh_cpfrom(char **args);

/**
	@brief Builtin command: cpto
	@param args List of args. args[0] is "cpto". args[1] is the yamafs file path. args[2] is the filesystem directory.
	@return Always return 1, to continue executing.
*/
int lsh_cpto(char **args);

/**
	@brief Builtin command: cpblock
	@param args List of args. args[0] is "cpblock". args[1] is the file path. args[2] is the block number. args[3] is the node id.
	@return Always return 1, to continue executing.
*/
int lsh_cpblock(char **args);

/**
	@brief Builtin command: md5
	@param args List of args. args[0] is "md5". args[1] is the yamafs file path.
	@return Always return 1, to continue executing.
*/
int lsh_md5(char **args);

/**
	@brief Builtin command: ls
	@param args List of args. args[0] is "ls". args[1] is the directory path.
	@return Always return 1, to continue executing.
*/
int lsh_ls(char **args);

/**
	@brief Builtin command: info
	@param args List of args. args[0] is "info". args[1] is the file path.
	@return Always return 1, to continue executing.
*/
int lsh_info (char **args);

#endif /* FUNCIONESCONSOLA_H_ */
