#include "utils.h"

size_t		ft_strcmp(char *a, char *b)
{
	while (a && b && *a && *b && *a == *b)
	{
		a++;
		b++;
	}
	return (*b - *a);
}


size_t      ft_strlen(char *s)
{
	size_t i;

    i = 0;
 	while (s && s[i])
		i++;
	return (i);
}



size_t      ft_strchr(char *s, char c)
{
	size_t	i;

	i = 0;
	while (s && s[i] && s[i] != c)
		i++;
	return (i);
}



size_t      ft_bzero(char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		str[i++] = 0;
	return (i);
}



size_t      ft_bcopy(char *dst, char *src, size_t size)
{	
	size_t	i;
	
	i = 0;
	while (i < size)
	{
		dst[i] = src[i];
		i++;	
	}	
	return (i);
}



char		*ft_strldup(char *str, size_t size)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = malloc((size + 1) * sizeof(char));
	while (i < size && str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

void		ft_strlcpy(char *dst, char *src, size_t len)
{
	while (dst && src && *src && len-- > 0)
	{
		*dst++ = *src++;
	}
}
int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_valid_var_char(char c)
{
	return (!is_ifs(c) && c != 0 && c != 61 && c != 36 && c != 32 && ((c >= 97 && c <= 126) || (c >= 65 && c <= 91)));
}

int skip_ifs(char *str)
{
	int i;

	i = 0;
	while (str && str[i] && is_ifs(str[i]))
		i++;
	return (i);
}
