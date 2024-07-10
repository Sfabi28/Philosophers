/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:42:27 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/13 11:18:34 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	f_atos(char	*str)
{
	size_t	res;
	size_t	i;

	i = 0;
	res = 0;
	while (str[i] >= 48 && str[i] <= '9')
	{
		res = res * 10 + str[i] - 48;
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
	pthread_mutex_lock(&table->statics->print_mutex);
	pthread_mutex_lock(&table->statics->death_mutex);
	if (table->statics->death == 0)
		printf("%ld %s%ld %s\n", write_time(table->statics->start_time),
			color, table->philo_id, str);
	pthread_mutex_unlock(&table->statics->death_mutex);
	pthread_mutex_unlock(&table->statics->print_mutex);
}

void	*check_time(void *arg)
{
	t_table	*philo;
	size_t	death;

	usleep(100);
	philo = (t_table *)arg;
	ft_while_2(philo, &death);
	pthread_mutex_unlock(&philo->statics->finished_mutex);
	return (NULL);
}

void	ft_while_2(t_table *philo, size_t *death)
{
	while (1)
	{
		usleep(100);
		pthread_mutex_lock(&philo->statics->finished_mutex);
		if (philo->statics->finished == philo->statics->nbr)
			break ;
		pthread_mutex_lock(&philo->statics->time_mutex);
		*death = write_time(philo->last_meal);
		if (*death >= philo->statics->death_time || philo->statics->death == 1)
		{
			pthread_mutex_unlock(&philo->statics->time_mutex);
			print_state("died \033[0m💀", "\033[0;31m", philo);
			pthread_mutex_lock(&philo->statics->death_mutex);
			philo->statics->death = 1;
			pthread_mutex_unlock(&philo->statics->death_mutex);
			pthread_mutex_unlock(&philo->statics->finished_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->statics->time_mutex);
		pthread_mutex_unlock(&philo->statics->finished_mutex);
	}
}
