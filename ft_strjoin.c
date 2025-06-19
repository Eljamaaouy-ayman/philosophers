/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:29:48 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/06/19 17:12:38 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	r;

	int (i), (s);
	i = 0;
	s = 1;
	r = 0;
	if (!str)
		return (-1);
	while ((str[i]) && (str[i] <= 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			s *= -1;
		i++;
	}
	while ((str[i]) && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i++] - 48);
		if (r < 0 || r > INT_MAX)
			return (-1);
	}
	if (str[i])
		return (-1);
	return (s * r);
}

int	ft_isdigit(int ch)
{
	if (ch < '0' || ch > '9')
		return (0);
	else
		return (1);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	int		i;
	int		j;
	int		k;

	if (s1 == NULL)
		return ((char *)s2);
	if (s2 == NULL)
		return ((char *)s1);
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	i = -1;
	k = ft_strlen(s1);
	j = ft_strlen(s2);
	s3 = malloc(k + j + 1);
	if (s3 == NULL)
		return (NULL);
	while (++i < k)
		s3[i] = s1[i];
	i--;
	while (i++ < k + j - 1)
		s3[i] = s2[i - k];
	s3[i] = '\0';
	return (s3);
}
