
#ifndef UTILS_H
# define UTILS_H

#include "stdlib.h"

size_t		ft_strcmp(char *a, char *b);
size_t      ft_strlen(char *str);
size_t      ft_strchr(char *s, char c);
size_t      ft_bzero(char *s, size_t size);
size_t      ft_bcopy(char *dst, char *src, size_t size);
char		*ft_strldup(char *s, size_t size);
#endif
