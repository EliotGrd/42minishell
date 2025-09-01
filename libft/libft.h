/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:23:41 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/18 16:38:04 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 2000

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_bzero(void *s, size_t n);
int					ft_lstsize(t_list *lst);
int					ft_atoi(const char *nptr);
void				*ft_calloc(size_t nmemb, size_t size);
void				*ft_memset(void *s, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				*ft_memchr(const void *s, int c, size_t n);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strdup(const char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
char				*ft_itoa(int n);
void				ft_putstr_fd(char *s, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
int					ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *str);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
char				**ft_split(char const *s, char c);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtok(char *str, char *separators);
char				*ft_strndup(char *str, size_t n);
int					is_n(char *str);
char				*extract_remainder(char *str);
char				*extract_line(char *str);
char				*looping_reading(int fd, char *temp, char *result,
						char *buffer);
char				*reading(int fd, char *remainder, char *buffer);
char				*get_next_line(int fd);
int					ft_atoi_base(char *str, char *base);
void				ft_free_all(char **result, int i);
int					ft_printf(const char *format, ...);
int					ft_convert(char format, va_list args);
int					ft_printf_char(char c);
int					ft_printf_str(char *str);
int					ft_printf_numbers(int n);
int					ft_printf_unsigned_numbers(unsigned int n);
int					ft_printf_hex(unsigned int n, char format);
int					ft_printf_address(unsigned long long n);
int					ft_printf_ptr(void *ptr);
void				ft_free_tab(char **result);
void				*ft_realloc(void *to_realloc, size_t old_size,
						size_t new_size);
int					ft_iswspace(int c);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_free(void **ptr);

#endif
