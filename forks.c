/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:38:37 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/15 12:38:37 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_philo *philo)
{
	philo->l_fork = philo->id;
	philo->r_fork = (philo->id + 1) % philo->table->num_philos;
	if (philo->id % 2)
	{
		philo->l_fork = (philo->id + 1) % philo->table->num_philos;
		philo->r_fork = philo->id;
	}
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->l_fork]);
	while (philo->table->forks[philo->l_fork] == 0)
	{
		pthread_mutex_unlock(&philo->table->fork_locks[philo->l_fork]);
		usleep(100);
		pthread_mutex_lock(&philo->table->fork_locks[philo->l_fork]);
	}
	philo->table->forks[philo->l_fork] = 0;
	log_status(philo, false, GOT_FORK);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->l_fork]);
	pthread_mutex_lock(&philo->table->fork_locks[philo->r_fork]);
	while (philo->table->forks[philo->r_fork] == 0)
	{
		pthread_mutex_unlock(&philo->table->fork_locks[philo->r_fork]);
		usleep(100);
		pthread_mutex_lock(&philo->table->fork_locks[philo->r_fork]);
	}
	philo->table->forks[philo->r_fork] = 0;
	log_status(philo, false, GOT_FORK);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->r_fork]);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->l_fork]);
	philo->table->forks[philo->l_fork] = 1;
	pthread_mutex_unlock(&philo->table->fork_locks[philo->l_fork]);
	pthread_mutex_lock(&philo->table->fork_locks[philo->r_fork]);
	philo->table->forks[philo->r_fork] = 1;
	pthread_mutex_unlock(&philo->table->fork_locks[philo->r_fork]);
}
