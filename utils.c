/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:16:13 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:16:13 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval	time_val;

	gettimeofday(&time_val, NULL);
	return ((time_val.tv_sec * 1000) + (time_val.tv_usec / 1000));
}

void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (check_simulation(table))
			break ;
		usleep(100);
	}
}

void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}

static void	print_status(t_philo *philo, char *str)
{
	printf("[%10ld] %03d %s\n", get_time_in_ms()
		- philo->table->start_time, philo->id + 1, str);
}

void	log_status(t_philo *philo, bool monitor, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (check_simulation(philo->table) == true && monitor == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died 💀");
	else if (status == EATING)
		print_status(philo, "is eating 🍝");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping 😴");
	else if (status == THINKING)
		print_status(philo, "is thinking 💭");
	else
		print_status(philo, "has taken a fork 🍴");
	pthread_mutex_unlock(&philo->table->write_lock);
}
