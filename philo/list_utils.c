/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:35:40 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/13 11:14:05 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_statics(t_statics **list, char **argv, int argc)
{
	(*list)->nbr = f_atos(argv[1]);
	(*list)->death_time = f_atos(argv[2]);
	if ((*list)->nbr == 1)
		usleep((*list)->death_time * 1000);
	if ((*list)->nbr == 1)
		return (printf("%lu \033[0;31m1 died \033[0m💀\n", (*list)->death_time));
	(*list)->eat_time = f_atos(argv[3]);
	(*list)->sleep_time = f_atos(argv[4]);
	(*list)->cicles = LLONG_MAX;
	(*list)->finished = 0;
	(*list)->death = 0;
	if (argc == 6)
		(*list)->cicles = f_atos(argv[5]);
	if (!(*list)->nbr || !(*list)->death_time
		|| !(*list)->eat_time || !(*list)->sleep_time || !(*list)->cicles)
		return (write(2, "Error!\nwrong input", 19) - 18);
	(*list)->start_time = write_time(0);
	if (mutex_init(list))
		return (1);
	return (0);
}

int	set_table(t_table **list, t_statics *statics)
{
	size_t	i;
	size_t	philo_num;
	t_table	*prev;
	t_table	*temp;

	i = 0;
	philo_num = 1;
	prev = NULL;
	while (++i <= statics->nbr * 2)
	{
		temp = lst_new(i, &philo_num, statics);
		if (!temp)
			return (f_lstclear(list, i), write(2, "Error!\nmalloc fail\n", 20));
		if (i == 1)
			*list = temp;
		temp->prev = prev;
		if (i > 1)
			prev->next = temp;
		prev = temp;
	}
	(*list)->prev = prev;
	prev->next = *list;
	return (0);
}

t_table	*lst_new(size_t i, size_t *philo_num, t_statics *statics)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (free(statics), NULL);
	if (i % 2 != 0)
		table->philo_id = (*philo_num)++;
	if (i % 2 != 0)
		table->fork = 0;
	if (i % 2 == 0)
		table->philo_id = 0;
	if (i % 2 == 0)
		table->fork = 1;
	table->eaten_num = 0;
	table->next = NULL;
	table->prev = NULL;
	table->statics = statics;
	table->last_meal = write_time(0);
	table->pt_philo = 0;
	table->pt_time = 0;
	if (pthread_mutex_init(&table->eaten_num_mutex, NULL))
		return (NULL);
	if (pthread_mutex_init(&table->fork_mutex, NULL))
		return (NULL);
	return (table);
}

void	f_lstclear(t_table **table, size_t i)
{
	size_t	h;
	t_table	*temp;
	t_table	*prev;

	h = 0;
	temp = *table;
	while (h < i && temp->next != NULL)
	{
		temp = temp->next;
		h++;
	}
	while (h != 0 && temp->prev != NULL)
	{
		prev = temp->prev;
		free(temp);
		temp = prev;
		h--;
	}
	free(table);
}

int	mutex_init(t_statics **list)
{
	if (pthread_mutex_init(&((*list)->finished_mutex), NULL))
		return (write(2, "Error!\nmutex init failed", 25) - 24);
	if (pthread_mutex_init(&((*list)->print_mutex), NULL))
		return (pthread_mutex_destroy(&((*list)->finished_mutex))
			+ write(2, "Error!\nmutex init failed", 25) - 24);
	if (pthread_mutex_init(&((*list)->time_mutex), NULL))
		return (pthread_mutex_destroy(&((*list)->finished_mutex))
			+ pthread_mutex_destroy(&((*list)->print_mutex))
			+ write(2, "Error!\nmutex init failed", 25) - 24);
	if (pthread_mutex_init(&((*list)->death_mutex), NULL))
		return (pthread_mutex_destroy(&((*list)->finished_mutex))
			+ pthread_mutex_destroy(&((*list)->print_mutex))
			+ pthread_mutex_destroy(&((*list)->time_mutex))
			+ write(2, "Error!\nmutex init failed", 25) - 24);
	return (0);
}
