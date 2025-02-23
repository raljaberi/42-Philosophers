/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:49 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:15:49 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
	table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

bool	check_simulation(t_table *table)
{
	bool	check;

	check = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		check = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (check);
}

static bool	check_philo_death(t_philo *philo)
{
	time_t	current_time;

	current_time = get_time_in_ms();
	if ((current_time - philo->last_meal) >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		log_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			meal_count_reached;

	meal_count_reached = true;
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_lock(&table->philos[i]->meal_time_lock);
		if (check_philo_death(table->philos[i]))
			return (true);
		if (table->must_eat_count != -1)
			if (table->philos[i]->times_ate
				< (unsigned int)table->must_eat_count)
				meal_count_reached = false;
		pthread_mutex_unlock(&table->philos[i]->meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && meal_count_reached == true)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

void	*monitor(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
