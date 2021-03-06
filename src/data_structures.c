#include <stdlib.h>
#include "data_structures.h"
#include "utils.h"


argument *
init_argument(void) {
	argument *arg = malloc(sizeof (argument));
	check_allocation(arg);

	arg->argument_value = NULL;

	return (arg);
}

argument_list *
argument_list_init(void) {
	argument_list *args = malloc(sizeof (argument_list));
	check_allocation(args);

	STAILQ_INIT(&args->head);

	return (args);
}

redirection *
redirection_init(void) {
	redirection *redirect = malloc(sizeof (redirection));
	check_allocation(redirect);

	redirect->in_file = NULL;
	redirect->out_file_r = NULL;
	redirect->out_file_a = NULL;

	return (redirect);
}

struct command *
command_init(void) {
	struct command *command = malloc(sizeof (struct command));
	check_allocation(command);

	command->command_name = NULL;
	command->argument_list = NULL;
	command->redirection = NULL;

	return (command);
}

command_list *
command_list_init(void) {
	command_list *commands = malloc(sizeof (command_list));
	check_allocation(commands);

	STAILQ_INIT(&commands->head);

	return (commands);
}

pipe_list *
pipe_list_init(void) {
	pipe_list *pipes = malloc(sizeof (pipe_list));
	check_allocation(pipes);

	STAILQ_INIT(&pipes->head);

	return (pipes);
}

void
argument_list_insert_tail(argument_list *where, argument *what) {

	STAILQ_INSERT_TAIL(&where->head, what, entries);
}

void
command_list_insert_head(command_list *where, struct command *what) {

	STAILQ_INSERT_HEAD(&where->head, what, entries);
}

void
pipe_list_insert_head(pipe_list *where, command_list *what) {

	STAILQ_INSERT_HEAD(&where->head, what, entries);
}

void
pipe_list_insert_simple_head(pipe_list *where, struct command *what) {

	command_list *to_add = command_list_init();
	command_list_insert_head(to_add, what);

	pipe_list_insert_head(where, to_add);
}

void
argument_list_deallocate(argument_list *what) {
	if (what == NULL)
		return;

	argument *n1, *n2;

	n1 = STAILQ_FIRST(&what->head);
	while (n1 != NULL) {
		n2 = STAILQ_NEXT(n1, entries);

		free(n1->argument_value);
		free(n1);

		n1 = n2;
	}
	free(what);
}

void
command_list_deallocate(command_list *what) {
	if (what == NULL)
		return;

	struct command *n1, *n2;

	n1 = STAILQ_FIRST(&what->head);
	while (n1 != NULL) {
		n2 = STAILQ_NEXT(n1, entries);

		command_deallocate(n1);

		n1 = n2;
	}

	free(what);
}

void
pipe_list_deallocate(pipe_list *what) {
	if (what == NULL)
		return;

	command_list *n1, *n2;

	n1 = STAILQ_FIRST(&what->head);
	while (n1 != NULL) {
		n2 = STAILQ_NEXT(n1, entries);

		command_list_deallocate(n1);

		n1 = n2;
	}

	free(what);
}

void
command_deallocate(struct command *what) {
	if (what == NULL)
		return;

	argument_list_deallocate(what->argument_list);

	redirection_deallocate(what->redirection);

	free(what->command_name);
	free(what);
}

void
redirection_deallocate(redirection *what) {
	if (what == NULL)
		return;

	free(what->in_file);
	free(what->out_file_a);
	free(what->out_file_r);

	free(what);
}
