/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:16:08 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:16:08 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat_sleep_routine(t_philo *philo)
{
	take_forks(philo);
	log_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (check_simulation(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	put_forks(philo);
	log_status(philo, false, SLEEPING);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_die - (get_time_in_ms()
				- philo->last_meal) - philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think <= 0)
		time_to_think = 0;
	if (time_to_think > 600)
		time_to_think = 200;
	log_status(philo, false, THINKING);
	philo_sleep(philo->table, time_to_think);
}

static void	*single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->l_fork]);
	log_status(philo, false, GOT_FORK);
	philo_sleep(philo->table, philo->table->time_to_die);
	log_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->l_fork]);
	return (NULL);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->num_philos == 1)
		return (single_philo_routine(philo));
	else if (philo->id % 2)
		eat_sleep_routine(philo);
	while (check_simulation(philo->table) == false)
	{
		think_routine(philo);
		eat_sleep_routine(philo);
	}
	return (NULL);
}
