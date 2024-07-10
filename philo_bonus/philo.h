/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:49:48 by sfabi             #+#    #+#             */
/*   Updated: 2024/03/12 15:43:02 by sfabi            ###   ########.fr       */
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
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <sys/mman.h>
# include <signal.h>
# include <stdbool.h>

typedef struct s_statics
{
	pthread_t		pt_finished;
	size_t			close_all;
	size_t			nbr;
	size_t			sleep_time;
	size_t			eat_time;
	size_t			death_time;
	size_t			cicles;
	size_t			start_time;
	sem_t			*finished;
	sem_t			*death;
	sem_t			*forks_nbr;
	sem_t			*kill_all;
	sem_t			*time;
	sem_t			*mutex;
}	t_statics;

typedef struct s_table
{
	pthread_t			pt_time;
	int					philo_pid;
	size_t				philo_id;
	size_t				eaten_num;
	size_t				last_meal;
	struct s_table		*next;
	struct s_table		*prev;
	struct s_statics	*statics;
}	t_table;

int		set_statics(t_statics **list, char **argv, int argc);
int		set_table(t_table **list, t_statics *statics);
t_table	*lst_new(size_t i, t_statics *statics);
void	f_lstclear(t_table **table, size_t i);
size_t	f_atos(char	*str);
int		philo_create(t_table **table);
void	philo_job(t_table *philo);
void	print_state(char *str, char *color, t_table *table);
size_t	write_time(size_t delta_time);
int		check_over(t_table *table);
void	*check_time(void *arg);
void	ft_while(t_table *philo);
void	kill_a(t_table *table);
void	*all_ate(void *arg);
void	f_unlink(t_table **table);

#endif