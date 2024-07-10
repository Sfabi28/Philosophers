/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:38:18 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/13 11:20:38 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_statics	*statics;
	t_table		**table;
	size_t		i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (write(2, "Error!\nwrong number of input\n", 30) - 29);
	statics = malloc(sizeof(t_statics));
	if (!statics)
		return (write(2, "Error!\nmalloc failed\n", 22) - 21);
	if (set_statics(&statics, av, ac))
		return (free(statics), 1);
	table = malloc(sizeof(t_table *));
	if (!table)
		return (free(statics), write(2, "Error!\nmalloc failed\n", 22) - 21);
	if (set_table(table, statics) || philo_create(table))
		return (1);
	while (i++ < statics->nbr * 2)
	{
		if (i % 2 != 0)
			pthread_join((*table)->pt_philo, NULL);
		(*table) = (*table)->next;
	}
	return (f_lstclear(table, --i), free(statics), 0);
}

int	philo_create(t_table **table)
{
	t_table		*philo;
	size_t		i;

	philo = *table;
	i = 0;
	while (i < philo->statics->nbr * 2)
	{
		if (i % 2 == 0)
		{
			if (pthread_create(&philo->pt_philo, NULL, &philo_job, philo) == -1)
				return (write(2, "Error\nduring thread creation\n", 30) - 29);
		}
		philo = philo->next;
		i++;
	}
	return (0);
}

void	*philo_job(void *arg)
{
	t_table	*philo;

	philo = (t_table *)arg;
	pthread_create(&philo->pt_time, NULL, &check_time, philo);
	pthread_detach(philo->pt_time);
	if ((philo->philo_id % 2 == 0 || philo->philo_id == philo->statics->nbr)
		&& philo->fork == 0)
	{
		print_state("is thinking \033[0m🤔", "\033[35m", philo);
		usleep(100);
	}
	ft_while(philo);
	return (NULL);
}

void	ft_while(t_table *philo)
{
	while (check_over(philo))
	{
		pthread_mutex_lock(&philo->next->fork_mutex);
		pthread_mutex_lock(&philo->prev->fork_mutex);
		print_state("has taken a fork\033[0m🍴", "\033[33m", philo);
		print_state("has taken a fork\033[0m🍴", "\033[33m", philo);
		print_state("is eating\033[0m🍴🥗", "\033[32m", philo);
		pthread_mutex_lock(&philo->statics->time_mutex);
		philo->last_meal = write_time(0);
		pthread_mutex_unlock(&philo->statics->time_mutex);
		usleep(philo->statics->eat_time * 1000);
		philo->eaten_num++;
		pthread_mutex_unlock(&philo->next->fork_mutex);
		pthread_mutex_unlock(&philo->prev->fork_mutex);
		if (philo->eaten_num == philo->statics->cicles)
		{
			pthread_mutex_lock(&philo->statics->finished_mutex);
			philo->statics->finished++;
			pthread_mutex_unlock(&philo->statics->finished_mutex);
		}
		print_state("is sleeping \033[0m💤", "\033[34m", philo);
		usleep(philo->statics->sleep_time * 1000);
		print_state("is thinking \033[0m🤔", "\033[35m", philo);
	}
}

int	check_over(t_table *table)
{
	pthread_mutex_lock(&table->statics->death_mutex);
	if (table->statics->death == 1)
	{
		pthread_mutex_unlock(&table->statics->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&table->statics->death_mutex);
	pthread_mutex_lock(&table->statics->finished_mutex);
	if (table->statics->finished == table->statics->nbr)
	{
		pthread_mutex_unlock(&table->statics->finished_mutex);
		return (0);
	}
	pthread_mutex_unlock(&table->statics->finished_mutex);
	return (1);
}
