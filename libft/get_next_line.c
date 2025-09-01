/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:43:22 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/27 14:26:47 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*extract_remainder(char *str)
{
	int		i;
	int		j;
	int		k;
	char	*result;

	i = 0;
	j = 0;
	k = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	while (str[i + j])
		j++;
	result = malloc(sizeof(char) * (j + 1));
	if (!result)
		return (free(str), NULL);
	while (str[i + k])
	{
		result[k] = str[i + k];
		k++;
	}
	result[k] = '\0';
	free(str);
	return (result);
}

char	*extract_line(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	result = malloc(sizeof(char) * (i + 1));
	if (!result)
		return (NULL);
	while (j < i && str[j])
	{
		result[j] = str[j];
		j++;
	}
	result[j] = '\0';
	return (result);
}

char	*looping_reading(int fd, char *temp, char *result, char *buffer)
{
	int	n;

	n = 1;
	while (n > 0)
	{
		temp = ft_strjoin(result, buffer);
		free(result);
		if (!temp)
			return (NULL);
		result = temp;
		if (is_n(buffer))
			break ;
		n = read(fd, buffer, BUFFER_SIZE);
		if (n <= 0)
		{
			if (result[0] != '\0')
				return (result);
			return (free(result), NULL);
		}
		buffer[n] = '\0';
	}
	return (result);
}

char	*reading(int fd, char *remainder, char *buffer)
{
	int		n;
	char	*result;
	char	*temp;

	temp = NULL;
	if (remainder == NULL)
		remainder = ft_strdup("");
	if (!remainder)
		return (NULL);
	result = ft_strjoin(remainder, buffer);
	free(remainder);
	if (!result)
		return (NULL);
	n = read(fd, buffer, BUFFER_SIZE);
	if (n <= 0)
	{
		if (result[0] != '\0')
			return (result);
		return (free(result), NULL);
	}
	buffer[n] = '\0';
	result = looping_reading(fd, temp, result, buffer);
	if (!result)
		return (NULL);
	return (result);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*remainder = NULL;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(remainder), remainder = NULL, NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (free(remainder), remainder = NULL, NULL);
	buffer[0] = '\0';
	if (!is_n(remainder))
		remainder = reading(fd, remainder, buffer);
	free(buffer);
	if (!remainder)
		return (NULL);
	line = extract_line(remainder);
	if (!line)
		return (free(remainder), remainder = NULL, NULL);
	remainder = extract_remainder(remainder);
	if (!remainder)
		return (free(line), NULL);
	return (line);
}
/*
int	main(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
	{
		write(1, "Not enough or too much args\n", 28);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		return (1);
	}
	char *str = get_next_line(fd);
	while (str)
	{
		//printf("%s",str);
		free(str);
		str = get_next_line(fd);
	}
	close(fd);
}
*/
