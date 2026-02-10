/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 23:12:23 by lrain             #+#    #+#             */
/*   Updated: 2026/02/10 23:05:48 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE BUFSIZ
# endif

typedef enum e_gnl_rflags
{
	e_gnl_eof = 1,
	e_gnl_err = 2,
}					t_gnl_rflags;

typedef enum e_read_flags
{
	e_r_err = -1,
	e_r_eof = 0,
}					t_read_flags;

typedef struct s_gnl_buf
{
	unsigned char	*buf;
	int				flags;
	size_t			rd_len;
	unsigned char	*rd_pos;
	unsigned char	*rd_end;
}					t_gnl_buf;

typedef struct s_gnl_currop
{
	int				eof_state;
	unsigned char	*outbuf;
	unsigned char	*temp;
	unsigned char	*delim;
	size_t			len;
	size_t			cap;
	size_t			newcap;
	size_t			copy_len;
}					t_gnl_currop;

char				*get_next_line(int fd);

#endif