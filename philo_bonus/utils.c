/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:42:27 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/12 15:24:50 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	f_atos(char	*str)
{
	size_t	res;
	size_t	i;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	if (!str[i])
		return (res);
	return (0);
}

size_t	write_time(size_t delta_time)
{
	struct timeval	restrict_tp;
	size_t			rs;

	gettimeofday(&restrict_tp, NULL);
	rs = (size_t)(restrict_tp.tv_sec * 1000 + restrict_tp.tv_usec / 1000);
	rs = rs - delta_time;
	return (rs);
}

void	print_state(char *str, char *color, t_table *table)
{
	sem_wait(table->statics->death);
	printf("%ld %s%ld %s\n", write_time(table->statics->start_time),
		color, table->philo_id, str);
	sem_post(table->statics->death);
}

void	*check_time(void *arg)
{
	t_table	*philo;
	size_t	death;

	usleep(100);
	philo = (t_table *)arg;
	while (!philo->statics->close_all)
	{
		usleep(10);
		sem_wait(philo->statics->time);
		death = write_time(philo->last_meal);
		if (death > philo->statics->death_time)
		{
			print_state("died \033[0m💀", "\033[0;31m", philo);
			sem_wait(philo->statics->death);
			sem_post(philo->statics->kill_all);
			return (NULL);
		}
		sem_post(philo->statics->time);
	}
	return (NULL);
}

void	*all_ate(void *arg)
{
	t_table	*philo;
	size_t	i;

	philo = (t_table *)arg;
	i = 0;
	while (++i <= philo->statics->nbr)
	{
		sem_wait(philo->statics->finished);
		sem_wait(philo->statics->mutex);
		if (philo->statics->close_all == 1)
			return (NULL);
		sem_post(philo->statics->mutex);
	}
	sem_post(philo->statics->kill_all);
	return (NULL);
}
