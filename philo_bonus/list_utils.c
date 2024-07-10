/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:35:40 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/12 15:37:25 by sfabi            ###   ########.fr       */
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
	if (argc == 6)
		(*list)->cicles = f_atos(argv[5]);
	if (!(*list)->nbr || !(*list)->death_time
		|| !(*list)->eat_time || !(*list)->sleep_time || !(*list)->cicles)
		return (write(2, "Error!\nwrong input", 19) - 18);
	(*list)->start_time = write_time(0);
	(*list)->forks_nbr = sem_open("/forks_pool", O_CREAT, 0644, (*list)->nbr);
	(*list)->death = sem_open("/death", O_CREAT, 0644, 1);
	(*list)->finished = sem_open("/finished", O_CREAT, 0644, 0);
	(*list)->kill_all = sem_open("/kill", O_CREAT, 0644, 0);
	(*list)->time = sem_open("/time", O_CREAT, 0644, 1);
	(*list)->mutex = sem_open("/mutex", O_CREAT, 0644, 1);
	(*list)->pt_finished = 0;
	(*list)->close_all = 0;
	return (0);
}

int	set_table(t_table **list, t_statics *statics)
{
	size_t	i;
	t_table	*prev;
	t_table	*temp;

	i = 0;
	prev = NULL;
	while (++i <= statics->nbr)
	{
		temp = lst_new(i, statics);
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

t_table	*lst_new(size_t i, t_statics *statics)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (free(statics), NULL);
	table->philo_id = i;
	table->eaten_num = 0;
	table->next = NULL;
	table->prev = NULL;
	table->statics = statics;
	table->last_meal = write_time(0);
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
