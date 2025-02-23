/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raljaber <raljaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:15:54 by raljaber          #+#    #+#             */
/*   Updated: 2024/09/12 16:15:54 by raljaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	ft_atoi(char *str)
{
	unsigned long long int	num;
	int						i;

	i = 0;
	num = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (num > INT_MAX)
		return (-1);
	return ((int)num);
}

bool	is_valid_input(int argc, char **argv)
{
	int	i;
	int	num;

	i = 1;
	while (i < argc)
	{
		if (!ft_isdigit(argv[i]))
			return (print_error_message(STR_ERR_INPUT_DIGIT, argv[i], 0));
		num = ft_atoi(argv[i]);
		if (i == 1 && (num <= 0 || num > 200))
			return (print_error_message(STR_ERR_PHILO_COUNT, argv[i], 0));
		if (i != 1 && num == -1)
			return (print_error_message(STR_ERR_INPUT_DIGIT, argv[i], 0));
		i++;
	}
	return (true);
}
