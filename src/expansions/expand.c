
#include "minishell.h"

t_cmd	*expand(char *cmd)
{
    t_cmd *command;

    command = malloc(sizeof(t_cmd));
    if (!command)
        return (NULL);
    command->av = malloc(sizeof(char*));
    if (!command->av)
        return (NULL);
    *command->av = cmd;
    command->ac = 0;
    expand_variables(command);
    expand_words(command);
    expand_quotes(command);
    return (command);
}

