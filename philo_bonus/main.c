/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:32:47 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/12 15:53:44 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_statics	*statics;
	t_table		**table;

	table = NULL;
	f_unlink(table);
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
	pthread_create(&(*table)->statics->pt_finished, NULL, &all_ate, *table);
	pthread_detach((*table)->statics->pt_finished);
	kill_a(*table);
	sem_wait((*table)->statics->mutex);
	(*table)->statics->close_all = 1;
	sem_post((*table)->statics->mutex);
	sem_post((*table)->statics->finished);
	return (f_unlink(table), f_lstclear(table, statics->nbr), free(statics), 0);
}

int	philo_create(t_table **table)
{
	t_table		*philo;
	size_t		i;

	philo = *table;
	i = 0;
	while (i < philo->statics->nbr)
	{
		philo->philo_pid = fork();
		if (philo->philo_pid < 0)
			return (write(2, "Error\nduring child creation\n", 30) - 29);
		if (philo->philo_pid == 0)
			philo_job(philo);
		philo = philo->next;
		i++;
	}
	return (0);
}

void	philo_job(t_table *philo)
{
	pthread_create(&philo->pt_time, NULL, &check_time, philo);
	pthread_detach(philo->pt_time);
	if (philo->philo_id % 2 == 0)
		usleep(10000);
	while (1)
	{
		usleep(150);
		sem_wait(philo->statics->forks_nbr);
		print_state("has taken a fork\033[0m🍴", "\033[33m", philo);
		sem_wait(philo->statics->forks_nbr);
		print_state("has taken a fork\033[0m🍴", "\033[33m", philo);
		print_state("is eating\033[0m🍴🥗", "\033[32m", philo);
		sem_wait(philo->statics->time);
		philo->last_meal = write_time(0);
		sem_post(philo->statics->time);
		usleep(philo->statics->eat_time * 1000);
		philo->eaten_num++;
		if (philo->eaten_num == philo->statics->cicles)
			sem_post(philo->statics->finished);
		sem_post(philo->statics->forks_nbr);
		sem_post(philo->statics->forks_nbr);
		print_state("is sleeping \033[0m💤", "\033[34m", philo);
		usleep(philo->statics->sleep_time * 1000);
		print_state("is thinking \033[0m🤔", "\033[35m", philo);
	}
}

void	kill_a(t_table *table)
{
	size_t	i;

	i = 0;
	sem_wait(table->statics->kill_all);
	usleep(100);
	while (++i <= table->statics->nbr)
	{
		kill(table->philo_pid, SIGKILL);
		table = table->next;
	}
}

void	f_unlink(t_table **table)
{
	usleep(10000);
	sem_unlink("/forks_pool");
	sem_unlink("/death");
	sem_unlink("/kill");
	sem_unlink("/time");
	sem_unlink("/mutex");
	sem_unlink("/finished");
	if (table != NULL)
		sem_close((*table)->statics->forks_nbr);
	if (table != NULL)
		sem_close((*table)->statics->death);
	if (table != NULL)
		sem_close((*table)->statics->kill_all);
	if (table != NULL)
		sem_close((*table)->statics->time);
	if (table != NULL)
		sem_close((*table)->statics->mutex);
	if (table != NULL)
		sem_close((*table)->statics->finished);
}
