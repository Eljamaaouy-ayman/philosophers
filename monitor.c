/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:22:53 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/06/15 19:18:51 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
int check_meals(t_data *data)
{
    int i;
    int meals;
    
    i = 0;
    meals = 0;
    while (i < data->num_philos)
    {
        pthread_mutex_lock(&data->philos[i].meal_mutex);
        if (data->philos[i].meals_eaten >= data->num_times_must_eat)
            meals++;
        pthread_mutex_unlock(&data->philos[i].meal_mutex);
        i++;
    }
    if (data->num_philos == meals)
    {
        pthread_mutex_lock(&data->print_mutex);
        pthread_mutex_lock(&data->death_mutex);
        data->dead = 1;
        printf("all philosophers have finished their meals\n");
        pthread_mutex_unlock(&data->print_mutex);
        pthread_mutex_unlock(&data->death_mutex);
        return (1);
    }
    return (0);
}

int check_philo_death(t_data *data, int i, struct timeval current_time)
{
    long long time_last_meal;
    long long current_time_in_ms;
    long long timestamp;
    
    pthread_mutex_lock(&data->philos[i].meal_mutex);
    time_last_meal = (data->philos[i].last_meal_time.tv_sec * 1000)
        + (data->philos[i].last_meal_time.tv_usec / 1000);
    current_time_in_ms = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
    pthread_mutex_unlock(&data->philos[i].meal_mutex);
    if (current_time_in_ms - time_last_meal >= data->time_to_die)
    {
        pthread_mutex_lock(&data->death_mutex);
        data->dead = 1;
        gettimeofday(&current_time, NULL);
        timestamp = (current_time.tv_sec * 1000 + current_time.tv_usec /1000
            - data->start_time.tv_sec * 1000 - data->start_time.tv_usec / 1000);
        pthread_mutex_lock(&data->print_mutex);
        printf("%lld %d died", timestamp, data->philos[i].id);
        pthread_mutex_unlock(&data->print_mutex);
        pthread_mutex_unlock(&data->death_mutex);
        return (1);
    }
    return (0);
}

void *monitor_death(void *arg)
{
    t_data *data;
    int i;
    struct timeval tv;
    
    data = (t_data *)arg;
    while(1)
    {
        i = 0;
        if (check_meals(data))
            return (NULL);
        while(i < data->num_philos)
        {
            gettimeofday(&tv, NULL);
            if (check_philo_death(data, i, tv))
                return (NULL);
            i++;
        }
        if (data->num_philos % 2 != 0)
            usleep(200);
    }
    return (NULL);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    while(1)
    {
        if(check_if_dead(philo))
            return NULL;
        print_status(philo, "is thinking");
        pick_forks(philo);
        pthread_mutex_lock(&philo->meal_mutex);
        philo->last_meal_time = get_current_time();
        pthread_mutex_unlock(&philo->meal_mutex);
        print_status(philo, "has taken a fork");
        print_status(philo, "is eating");
        pthread_mutex_lock(&philo->meal_mutex);
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_mutex);
        take_a_sleep(philo, philo->data->time_to_eat);
        release_forks(philo);
        if(check_if_dead(philo))
            return NULL;
        print_status(philo, "is sleeping");
        take_a_sleep(philo, philo->data->time_to_eat);    
    }
    return (NULL);
}

int start_simulation(t_data *data)
{
    int i;
    pthread_t monitor;

    i = 0;
    printf("hhhh");
    while(i < data->num_philos)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]) != 0)
            return (write(2, "Error, failed to create philo thread\n", 38), 1);
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_death, data) != 0)
        return (write(2, "Error, failed to create monitor thread\n", 40), 1);
    i = 0;
    while(i < data->num_philos)
    {
        pthread_join(data->philos[i].thread, NULL);
        i = 0;
    }
    pthread_join(monitor, NULL);
    return (0);
}