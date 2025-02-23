/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:16:03 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:16:03 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define STR_ARGS \
	"Expected arguments: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_eargch_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT \
	"Invalid input: %s: expecting a valid unsigned integer \
between 0 and 2147483647.\n"
# define STR_ERR_PHILO_COUNT \
	"Invalid input: %s: the number of philosophers must be between 1 and 200.\n"
# define STR_ERR_THREAD "error: could not create thread.\n"
# define STR_ERR_MALLOC "error: memory allocation failed.\n"
# define STR_ERR_MUTEX "error: mutex initialization failed.\n"

// Why did I calls this here?
typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t				start_time;
	unsigned int		num_philos;
	pthread_t			monitor;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	int					must_eat_count;
	bool				sim_stop;
	pthread_mutex_t		sim_stop_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	int					*forks;
	t_philo				**philos;
}						t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		l_fork;
	unsigned int		r_fork;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_table				*table;
}						t_philo;

typedef enum e_status
{
	DIED,
	EATING,
	SLEEPING,
	THINKING,
	GOT_FORK
}						t_status;

bool					is_valid_input(int argc, char **argv);
int						ft_atoi(char *str);
t_table					*init_table(int argc, char **argv, int i);
void					assign_forks(t_philo *philo);
void					take_forks(t_philo *philo);
void					put_forks(t_philo *philo);
void					*philo_routine(void *data);
time_t					get_time_in_ms(void);
void					philo_sleep(t_table *table, time_t sleep_time);
void					sim_start_delay(time_t start_time);
void					log_status(t_philo *philo, bool monitor,
							t_status status);
void					*error_malloc(char *str, char *details, t_table *table);
int						print_error_message(char *str, char *detail, int n);
void					*monitor(void *data);
bool					check_simulation(t_table *table);
int						error_failure(char *str, char *details, t_table *table);
void					*free_table(t_table *table);
void					destroy_mutexes(t_table *table);

#endif