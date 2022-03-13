/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agardet <agardet@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 15:15:30 by agardet           #+#    #+#             */
/*   Updated: 2022/03/13 18:07:53 by agardet          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "stdlib.h"

# define QUOTE_NONE   0
# define QUOTE_SINGLE 1
# define QUOTE_DOUBLE 2

int			get_quote(char c);
int			check_quote(char c);

size_t		ft_strcmp(char *a, char *b);
size_t		ft_strlen(char *str);
size_t		ft_strchr(char *s, char c);
char		*ft_strchr_bis(const char *str, int c);
size_t		ft_bzero(char *s, size_t size);
size_t		ft_bcopy(char *dst, char *src, size_t size);
char		*ft_strldup(char *s, size_t size);
void		ft_strlcpy(char *dst, char *src, size_t len);
char		*ft_substr(char *src, unsigned int start, size_t len);
int			is_ifs(char c);
int			is_valid_var_char(char c);
int			skip_ifs(char *str);
int			skip_redir(char *str);
int			skip_car(char *str);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
void		skip_spaces(char **line);


#endif
