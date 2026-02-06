/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrain <lrain@students.42berlin.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 23:12:23 by lrain             #+#    #+#             */
/*   Updated: 2026/02/06 14:34:39 by lrain            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE BUFSIZ
# endif

# ifndef MEMSIZE_INIT
#  if BUFFER_SIZE > BUFSIZ
#   define MEMSIZE_INIT BUFFER_SIZE
#  else
#   define MEMSIZE_INIT BUFSIZ
#  endif
# endif

# ifndef STREAM_EOF
#  define STREAM_EOF 1
# endif

# ifndef DO_MEMCHR
#  define DO_MEMCHR NULL
# endif

# ifndef NOCHR
#  define NOCHR 0
# endif

# ifndef MAXFREES
#  define MAXFREES 3
# endif

typedef enum e_std_rflags
{
	e_stdr_err = -1,
	e_stdr_eof = 0,
	e_strr_normal = 1,
}					t_std_rflags;

typedef enum e_gnl_rflags
{
	e_gnl_eof = 1,
}					t_gnl_rflags;

typedef enum e_read_flags
{
	e_r_err = -1,
	e_r_eof = 0,
	e_r_normal_bounds = 1,
}					t_read_flags;

typedef struct s_gnl_buf
{
	int				flags;
	unsigned char	*buf;
	size_t			rd_len;
	unsigned char	*rd_pos;
	unsigned char	*rd_end;
}					t_gnl_buf;

typedef struct s_gnl_currop
{
	unsigned char	*outbuf;
	size_t			cap;
	size_t			len;
	size_t			newcap;
	unsigned char	*delim;
	size_t			copy_len;
}					t_gnl_currop;

typedef struct s_to_free
{
	size_t			num;
	void			*ptrs[MAXFREES];
}					t_to_free;

char				*get_next_line(int fd);

#endif