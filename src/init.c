/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:44:32 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/01 23:10:47 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

// Calculate the map height by reading from the map file (.cub)
static int map_height(char *av)
{
    int fd;
    int h;
    char *line;

    fd = open(av, O_RDONLY);
    if (fd == -1)
    {
        puts("Error\nOpen failed\n");
        exit(1);
    }
    h = 0;
    line = get_next_line(fd);
    while (line)
    {
        free(line);
        h++;
        line = get_next_line(fd);
    }
    close(fd);
    return h;
}

static t_map *init_map_height(int h)
{
    t_map *map;

    map = malloc(sizeof(t_map));
    if (!map)
        exit(1);
    
    // Initialize all struct members
    map->height = h;
    map->width = 0;
    map->mlx = NULL;
    map->win = NULL;
    map->img = NULL;
    map->img_data = NULL;
    map->img_bpp = 0;
    map->img_size_line = 0;
    map->img_endian = 0;
    map->player.player_x = 0;
    map->player.player_y = 0;
    map->player.angle = 0.0;

    // Initialize rays array with a while loop
    int i = 0;
    while (i < RAY_NUM)
    {
        map->rays[i].wall_x = 0;
        map->rays[i].wall_y = 0;
        map->rays[i].distance = 0;
        map->rays[i].hit_side = 0;
        i++;
    }
    
    // Allocate map array
    map->map = malloc(sizeof(char *) * (h + 1));
    if (!map->map)
    {
        free(map);
        exit(1);
    }
    return map;
}

static void fill_map_content(t_map *map, char *av)
{
    int fd;
    int row;
    char *line;

    fd = open(av, O_RDONLY);
    if (fd == -1)
    {
        puts("Error\nFailed to open map file\n");
        exit(1);
    }

    row = 0;
    while (row < map->height)
    {
        line = get_next_line(fd);
        if (!line)
        {
            puts("Error\nUnexpected end of file\n");
            close(fd);
            exit(1);
        }
        map->map[row] = line;
        row++;
    }

    line = get_next_line(fd);
    if (line)
        free(line);
    close(fd);
    map->map[row] = NULL;
}

// Validate the map structure
static void validate_map(t_map *map)
{
    if (!map || !map->map || map->height <= 0)
    {
        puts("Error\nInvalid map structure\n");
        exit(1);
    }
    
    if (!map->map[0] || ft_strlen1(map->map[0]) <= 0)
    {
        puts("Error\nInvalid map content\n");
        exit(1);
    }
    
    printf("Map loaded: %dx%d\n", map->width, map->height);
}

// Filling the map from the file
t_map *fill_map(char *av)
{
    t_map *map;
    int h;

    h = map_height(av);
    if (h < 1)
    {
        puts("Error\nThe map is invalid");
        exit(1);
    }

    map = init_map_height(h);
    fill_map_content(map, av);
    
    if (!map->map[0])
    {
        puts("Error\nEmpty map file\n");
        exit(1);
    }
    
    map->width = ft_strlen1(map->map[0]);
    
    // Validate map before returning
    validate_map(map);
    
    return map;
}