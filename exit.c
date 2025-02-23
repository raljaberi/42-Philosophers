/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:35 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:15:35 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*free_table(t_table *table)
{
	unsigned int	i;

	if (!table)
		return (NULL);
	if (table->forks != NULL)
		free(table->forks);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->num_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

void	destroy_mutexes(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
}

int	print_error_message(char *str, char *detail, int n)
{
	if (!detail)
		printf("%s", str);
	else
		printf(str, detail);
	return (n);
}

int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (print_error_message(str, details, 0));
}

void	*error_malloc(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	print_error_message(str, details, 1);
	return (NULL);
}
