/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:13:33 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/05/27 16:34:46 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

typedef struct s_philo t_philo;
typedef struct initial_data
{
    int num_philos;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    int num_times_must_eat;
} t_initial_data;

typedef struct s_data
{
    int num_philos;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    int num_times_must_eat;
    int dead;
	struct timeval		start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	t_philo				*philos;
}   t_data;

typedef struct s_philo
{
    int id;
	int					meals_eaten;
	int			    	has_fork;
	struct timeval		last_meal_time;
	pthread_t			thread;
	pthread_t			monitor_thread;
	pthread_t			monitor_dead;
	pthread_mutex_t		meal_mutex;
	t_data				*data;
} t_philo;


void *minitor_death(void *arg);
int start_simulation(t_data *data);
void take_a_sleep(t_philo *philo, long long duration);
long long get_time_in_ms();
void release_forks(t_philo *philo);
void pick_forks(t_philo *philo);
void print_status(t_philo *philo, const char *status);
int check_if_dead(t_philo *philo);
struct timeval get_current_time();
int	ft_isdigit(int ch);
int init_forks(t_data *data);
int init_mutexes(t_data *data);
void init_philosophers(t_data *data);
int init_mutex_and_philos(t_data *data);
t_data *initialize_data(t_initial_data init);
t_data *init_data(int cap, int *num);
int check_meals(t_data *data);
int check_philo_death(t_data *data, int i, struct timeval current_time);
void *monitor_death(void *arg);
void *philosopher_routine(void *arg);
int start_simulation(t_data *data);
int ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
int    ft_atoi(const char *str);