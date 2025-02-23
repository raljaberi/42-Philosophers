/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:39 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:15:39 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	*init_forks(t_table *table)
{
	unsigned int	i;
	int				*forks;

	forks = malloc(sizeof(int) * table->num_philos);
	if (!forks)
		return (error_malloc(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->num_philos)
	{
		forks[i] = 1;
		i++;
	}
	return (forks);
}

static t_philo	**init_philos(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!philos)
		return (error_malloc(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->num_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_malloc(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (error_malloc(STR_ERR_MUTEX, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_mutexes(t_table *table)
{
	pthread_mutex_t	*fork_locks;
	unsigned int	i;

	fork_locks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (!fork_locks)
		return (error_malloc(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&fork_locks[i], 0) != 0)
			return (error_failure(STR_ERR_MUTEX, NULL, table));
		i++;
	}
	table->fork_locks = fork_locks;
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	return (true);
}

t_table	*init_table(int argc, char **argv, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_malloc(STR_ERR_MALLOC, NULL, 0));
	table->num_philos = ft_atoi(argv[i++]);
	table->time_to_die = ft_atoi(argv[i++]);
	table->time_to_eat = ft_atoi(argv[i++]);
	table->time_to_sleep = ft_atoi(argv[i++]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = ft_atoi(argv[i]);
	table->philos = init_philos(table);
	if (!table->philos)
		return (NULL);
	table->forks = init_forks(table);
	if (!table->forks)
		return (NULL);
	if (!init_mutexes(table))
		return (NULL);
	table->sim_stop = false;
	return (table);
}
