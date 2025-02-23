/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:59 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:15:59 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + (table->num_philos * 2 * 10);
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_create(&table->philos[i]->thread, NULL, &philo_routine,
				table->philos[i]) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
		i++;
	}
	if (table->num_philos > 1)
	{
		if (pthread_create(&table->monitor, NULL, &monitor, table) != 0)
			return (error_failure(STR_ERR_THREAD, NULL, table));
	}
	return (true);
}

static void	stop_simulation(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->philos[i]->thread, NULL);
		i++;
	}
	if (table->num_philos > 1)
		pthread_join(table->monitor, NULL);
	destroy_mutexes(table);
	free_table(table);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	table = NULL;
	if (argc < 5 || argc > 6)
		return (print_error_message(STR_ARGS, NULL, 1));
	if (!is_valid_input(argc, argv))
		return (1);
	table = init_table(argc, argv, 1);
	if (!table)
		return (1);
	if (!start_simulation(table))
		return (1);
	stop_simulation(table);
	return (0);
}
