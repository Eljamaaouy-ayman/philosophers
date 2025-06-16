/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:59:45 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/06/15 19:59:21 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int *parsing(int ac, char **av)
{
    int (i), (*tab);

    if (ac != 5 && ac != 6)
        return (printf("Error: Wrong number of arguments\n"), NULL);
    tab = malloc((ac - 1) * sizeof(int));
    if(!tab)
        return (printf("Error: allocation failed\n"), NULL);
    i = 1;
    while (i < ac)
    {
        write(1, &i, 1);
        if (ft_atoi(av[i]) < 0)
            return (printf("Error: Invalid argument %d\n", ft_atoi(av[i])), NULL);
        tab[i - 1] = ft_atoi(av[i]);
        i++;
        printf("%d", tab[i - 1]);
    }
    return (tab);
}

int init_forks(t_data *data)
{
    int i;

    i = 0;
    while(i < data->num_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
            return (write(2, "Error: Mutex initialization failed\n", 36), 1);
        i++;
    }
    return (0);
}

int init_mutexes(t_data *data)
{
    if (pthread_mutex_init(&data->print_mutex, NULL) || pthread_mutex_init(
        &data->meal_mutex, NULL)|| pthread_mutex_init(&data->death_mutex, NULL))
        return (write(2, "Error: Mutex initialization failed\n", 36) , 1);
    return (0);
}

struct timeval get_current_time()
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return (tv);
}


void init_philosophers(t_data *data)
{
    int i;
    
    i = 0;
    while(i > data->num_philos)
    {
        data->philos[i].id = i + 1;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = get_current_time();
        data->philos[i].data = data;
        pthread_mutex_init(&data->philos[i].meal_mutex , NULL);
        i++;
    }
}

int init_mutex_and_philos(t_data *data)
{
    if (init_forks(data))
        return (1);
    if (init_mutexes(data))
        return (1);
    init_philosophers(data);
    return (0);
}

t_data *initialize_data(t_initial_data init)
{
    t_data *data;
    
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

t_data *init_data(int cap, int *num)
{
    t_initial_data init;
    
    init.num_philos = num[0];
    init.time_to_die = num[1];
    init.time_to_eat = num[2];
    init.time_to_sleep = num[3];
    if (cap == 6)
        init.num_times_must_eat = num[4];
    else
        init.num_times_must_eat = 2147483647;
    return(initialize_data(init));
} 

int check_if_dead(t_philo *philo)
{
    int dead;

    pthread_mutex_lock(&philo->data->death_mutex);
    dead = philo->data->dead;
    pthread_mutex_unlock(&philo->data->death_mutex);
    return (dead);
}

void print_status(t_philo *philo, const char *status)
{
    struct timeval current_time;
    long long timestamp;
    
    gettimeofday(&current_time, NULL);
    timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000)
    - (philo->data->start_time.tv_sec * 1000)
    - (philo->data->start_time.tv_usec / 1000));
    if(check_if_dead(philo))
        return;
    if(!check_if_dead(philo))
    {
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%lld %d %s\n", timestamp, philo->id, status);
        pthread_mutex_unlock(&philo->data->print_mutex);
    }
}

void pick_forks(t_philo *philo)
{
    int left_fork;
    int right_fork;

    left_fork = philo->id - 1;
    right_fork = philo->id % philo->data->num_philos;
    if(philo->id % 2 != 0)
        usleep(2000);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->data->forks[right_fork]);
        pthread_mutex_lock(&philo->data->forks[left_fork]);
    }
    else
    {
        pthread_mutex_lock(&philo->data->forks[left_fork]);
        pthread_mutex_lock(&philo->data->forks[right_fork]);
    }
}

void release_forks(t_philo *philo)
{
    int left_fork;
    int right_fork;
    
    left_fork = philo->id - 1;
    right_fork = philo->id % philo->data->num_philos;
    if (philo->id % 2 != 0)
        usleep(100);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(&philo->data->forks[right_fork]);
        pthread_mutex_unlock(&philo->data->forks[left_fork]);
    }
    else
    {
        pthread_mutex_unlock(&philo->data->forks[left_fork]);
        pthread_mutex_unlock(&philo->data->forks[right_fork]);
    }
}
long long get_time_in_ms()
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec * 1000LL) + ((long long)tv.tv_usec / 1000LL));
}

void take_a_sleep(t_philo *philo, long long duration)
{
    long long start_time;
    
    start_time = get_time_in_ms();
    while (get_time_in_ms() - start_time < duration && !check_if_dead(philo))
        usleep(50);
}

int main(int ac, char **av)
{
    t_data *data;
    int *num;
    
    num = parsing(ac, av);
    
    int i = 0;
    while(i < ac - 1)
    {
        printf("\n%d", num[i++]);
    }
    exit(0);
    data =  init_data(ac, num);
    if (!data)
        return (write(2, "error, memory allocation\n", 26), 1);
    if (data->num_philos == 1)
    {
        printf("0 1 is thinking\n");
        usleep(data->time_to_die * 1000);
        printf("%d 1 died\n", data->time_to_die);
        return (0);
    }
    if (data->num_times_must_eat == 0)
        return(printf("all philosophers have finished their meals\n"), 0);
    start_simulation(data);
    return (0);
}
