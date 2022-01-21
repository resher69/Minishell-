
#include "minishell.h"

t_cmd	*expand(char *cmd)
{
    t_cmd *command;

//    assertfalse(cmd);
    command = malloc(sizeof(t_cmd));
    if (!command)
        return (NULL);
    *command->av = cmd;
    command->ac = 0;
    expand_variables(command);
    expand_words(command);
    expand_quotes(command);
    return (command);
}

