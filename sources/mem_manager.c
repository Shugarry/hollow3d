/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_memory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:50:38 by frey-gal          #+#    #+#             */
/*   Updated: 2025/06/26 01:06:39 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	*memlist_alloc(t_data *data, size_t size)
{
	t_list	*node;
	void	*ptr;

	if (!data)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		clean_exit(data, "Memory allocation failure", EXIT_FAILURE);
	node = ft_lstnew(ptr);
	if (!node)
	{
		free(ptr);
		clean_exit(data, "Memory allocation failure", EXIT_FAILURE);
	}
	ft_lstadd_front(&data->memlist, node);
	return (ptr);
}

void	*memlist_add(t_data *data, void *ptr)
{
	t_list	*node;

	if (!data)
		return (NULL);
	if (!ptr)
		return (NULL);
	node = ft_lstnew(ptr);
	if (!node)
	{
		free(ptr);
		clean_exit(data, "Memory allocation failure", EXIT_FAILURE);
	}
	ft_lstadd_front(&data->memlist, node);
	return (ptr);
}

void	memlist_free_ptr(t_data *data, void *ptr)
{
	t_list	*current;
	t_list	*prev;

	if (!data || !ptr || !data->memlist)
		return ;
	if ((data->memlist)->content == ptr)
	{
		current = data->memlist;
		data->memlist = (data->memlist)->next;
		ft_lstdelone(current, free);
		return ;
	}
	prev = data->memlist;
	current = (data->memlist)->next;
	while (current)
	{
		if (current->content == ptr)
		{
			prev->next = current->next;
			ft_lstdelone(current, free);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	clean_exit(t_data *data, char *error_str, int error_num)
{
	ft_lstclear(&data->memlist, free);
	if (error_num && error_str)
		perror(error_str);
	exit(error_num);
}
