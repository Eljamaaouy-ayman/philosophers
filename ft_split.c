/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:30:31 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/06/19 17:12:34 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ftstrlcpy(const char *src, char *dst, int c)
{
	int	i;

	i = 0;
	while (i < c - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

static int	safe_malloc(char **s, int k, int c)
{
	s[k] = (char *)malloc(c * sizeof(char) + 1);
	if (s[k] == NULL)
	{
		while (k >= 0)
		{
			free(s[k]);
			k--;
		}
		return (0);
	}
	return (1);
}

static int	count_words(const char *s, char c)
{
	int	i;
	int	is_c;
	int	words;

	i = 0;
	is_c = 0;
	words = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && is_c == 0)
		{
			words++;
			is_c = 1;
		}
		if (is_c == 1 && s[i] == c)
			is_c = 0;
		i++;
	}
	return (words);
}

static int	fill(char **s1, const char *s, char c)
{
	int	i;
	int	len;
	int	k;

	k = 0;
	i = 0;
	while (s[i] != '\0')
	{
		len = 0;
		while (s[i] == c)
			i++;
		while (s[i] != '\0' && s[i] != c)
		{
			i++;
			len++;
		}
		if (s[i] != '\0' || (s[i] == '\0' && s[i - 1] != c))
		{
			if (safe_malloc(s1, k, len) == 0)
				return (1);
			ftstrlcpy(&s[i - len], s1[k], len + 1);
			k++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**s1;

	if (s == NULL)
		return (NULL);
	words = count_words(s, c);
	s1 = (char **)malloc((words + 1) * sizeof(char *));
	if (s1 == NULL)
		return (NULL);
	if (fill(s1, s, c) == 1)
	{
		free(s1);
		return (0);
	}
	s1[words] = NULL;
	return (s1);
}
