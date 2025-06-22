/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:02:12 by ael-jama          #+#    #+#             */
/*   Updated: 2025/06/21 14:28:31 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*parsing(int ac, char **av)
{
	int	i;
	int	*tab;

	if (ac != 5 && ac != 6)
		return (printf("Error: Wrong number of arguments\n"), exit(1), NULL);
	tab = malloc((ac - 1) * sizeof(int));
	if (!tab)
		return (printf("Error: allocation failed\n"), exit(1), NULL);
	i = 1;
	while (i < ac)
	{
		if (ft_atoi(av[i]) < 0)
			return (printf("Error: Invalid argument %s\n", av[i]), free(tab),
				exit(1), NULL);
		tab[i - 1] = ft_atoi(av[i]);
		i++;
	}
	return (tab);
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (write(2, "Error: Mutex initialization failed\n", 36), 1);
		i++;
	}
	return (0);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL)
		|| pthread_mutex_init(&data->meal_mutex, NULL)
		|| pthread_mutex_init(&data->death_mutex, NULL))
		return (write(2, "Error: Mutex initialization failed\n", 36), 1);
	return (0);
}

struct timeval	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
		pthread_mutex_init(&data->philos[i].meal_mutex, NULL);
		i++;
	}
}
