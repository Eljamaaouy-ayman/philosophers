/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:04:46 by ael-jama          #+#    #+#             */
/*   Updated: 2025/06/20 22:58:13 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex_and_philos(t_data *data)
{
	if (init_forks(data))
		return (1);
	if (init_mutexes(data))
		return (1);
	init_philosophers(data);
	return (0);
}

t_data	*initialize_data(t_initial_data init)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = init.num_philos;
	data->time_to_die = init.time_to_die;
	data->time_to_eat = init.time_to_eat;
	data->time_to_sleep = init.time_to_sleep;
	data->num_times_must_eat = init.num_times_must_eat;
	data->dead = 0;
	data->philos = malloc(init.num_philos * sizeof(t_philo));
	data->forks = malloc(init.num_philos * sizeof(pthread_mutex_t));
	if (!data->philos || !data->forks)
		return (NULL);
	if (init_mutex_and_philos(data))
		return (NULL);
	gettimeofday(&data->start_time, NULL);
	return (data);
}

t_data	*init_data(int cap, int *num)
{
	t_initial_data	init;

	init.num_philos = num[0];
	init.time_to_die = num[1];
	init.time_to_eat = num[2];
	init.time_to_sleep = num[3];
	if (cap == 6)
		init.num_times_must_eat = num[4];
	else
		init.num_times_must_eat = 2147483647;
	return (initialize_data(init));
}

int	check_if_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->data->death_mutex);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (dead);
}

void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000)
			- (philo->data->start_time.tv_sec * 1000)
			- (philo->data->start_time.tv_usec / 1000));
	if (check_if_dead(philo))
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%lld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
