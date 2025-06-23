/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:05:56 by ael-jama          #+#    #+#             */
/*   Updated: 2025/06/23 10:23:47 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("0 1 is thinking\n");
	usleep(philo->data->time_to_die * 1000);
	printf("%d 1 died\n", philo->data->time_to_die);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	*data;
	int		*num;

	num = parsing(ac, av);
	data = init_data(ac, num);
	free(num);
	if (!data)
		return (write(2, "error, memory allocation\n", 26), free_data(data), 1);
	if (data->num_philos == 1)
	{
		if (pthread_create(&data->philos[0].thread, NULL, one_philo,
				&data->philos[0]) != 0)
			return (write(2, "Error: Failed to create philosopher thread\n",
					44), 1);
		pthread_join(data->philos[0].thread, NULL);
		return (free_data(data), 0);
	}
	if (data->num_times_must_eat == 0)
		return (printf("all philosophers have finished their meals\n"),
			free_data(data), 0);
	start_simulation(data);
	return (0);
}
