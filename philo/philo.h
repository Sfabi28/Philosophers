/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:49:48 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/13 11:17:37 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdint.h>
# include <limits.h>

typedef struct s_statics
{
	pthread_mutex_t	finished_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	death_mutex;
	size_t			nbr;
	size_t			sleep_time;
	size_t			eat_time;
	size_t			death_time;
	size_t			cicles;
	size_t			start_time;
	size_t			finished;
	size_t			death;
}	t_statics;

typedef struct s_table
{
	pthread_mutex_t		eaten_num_mutex;
	pthread_mutex_t		fork_mutex;
	pthread_t			pt_philo;
	pthread_t			pt_time;
	size_t				philo_id;
	size_t				fork;
	size_t				eaten_num;
	size_t				last_meal;
	struct s_table		*next;
	struct s_table		*prev;
	struct s_statics	*statics;
}	t_table;

int		set_statics(t_statics **list, char **argv, int argc);
int		set_table(t_table **list, t_statics *statics);
t_table	*lst_new(size_t i, size_t *philo_num, t_statics *statics);
void	f_lstclear(t_table **table, size_t i);
size_t	f_atos(char	*str);
int		philo_create(t_table **table);
void	*philo_job(void *arg);
void	print_state(char *str, char *color, t_table *table);
size_t	write_time(size_t delta_time);
int		check_over(t_table *table);
void	*check_time(void *arg);
void	ft_while(t_table *philo);
int		mutex_init(t_statics **list);
void	ft_while_2(t_table *philo, size_t *death);

#endif