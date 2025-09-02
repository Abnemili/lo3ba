/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/09/02 10:47:16 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;

    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;

    // Check bounds
    if (tile_x < 0 || tile_x >= map->width ||
        tile_y < 0 || tile_y >= map->height)
        return 0;

    // Check if center tile is free (not a wall)
    if (map->map[tile_y][tile_x] == '1')
        return 0;

    return 1;
}

void clear_player_area(t_map *map)
{
    int x = -1;
    int y;

    while (x <= 1)
    {
        y = -1;
        while (y <= 1)
        {
            int tile_x = (map->player.player_x + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE + x;
            int tile_y = (map->player.player_y + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE + y;

            if (tile_x >= 0 && tile_x < map->width && tile_y >= 0 && tile_y < map->height)
            {
                int color;

                if (map->map[tile_y][tile_x] == '1')
                    color = COLOR_WALL;
                else if (map->map[tile_y][tile_x] == '0')
                    color = COLOR_FREE;
                else
                {
                    y++;
                    continue;
                }

                draw_square(map, tile_x, tile_y, color);
            }
            y++;
        }
        x++;
    }
}

int handle_key_input(int keycode, t_map *map)
{
    double new_x = map->player.player_x;
    double new_y = map->player.player_y;

    if (keycode == KEY_ESC) // the escape key was pressed
        return handle_close(map);

    // Handle rotation (arrow keys)
    if (keycode == 65361) // Left arrow key
    {
        map->player.angle -= 5.0;
        if (map->player.angle < 0)
            map->player.angle += 360;

        render_game_view(map);
        return 0;
    }
    else if (keycode == 65363) // Right arrow key
    {
        map->player.angle += 5.0;
        if (map->player.angle >= 360)
            map->player.angle -= 360;

        render_game_view(map);
        return 0;
    }

    // Handle FPS-style movement (WASD relative to view direction)
    if (keycode == KEY_W) // W - Move forward in facing direction
    {
        new_x += cos(DEG_TO_RAD(map->player.angle)) * MOVE_SPEED;
        new_y += sin(DEG_TO_RAD(map->player.angle)) * MOVE_SPEED;
    }
    else if (keycode == KEY_S) // S - Move backward (opposite of facing direction)
    {
        new_x -= cos(DEG_TO_RAD(map->player.angle)) * MOVE_SPEED;
        new_y -= sin(DEG_TO_RAD(map->player.angle)) * MOVE_SPEED;
    }
    else if (keycode == KEY_A) // A - Strafe left
    {
        new_x += cos(DEG_TO_RAD(map->player.angle - 90)) * MOVE_SPEED;
        new_y += sin(DEG_TO_RAD(map->player.angle - 90)) * MOVE_SPEED;
    }
    else if (keycode == KEY_D) // D - Strafe right  
    {
        new_x += cos(DEG_TO_RAD(map->player.angle + 90)) * MOVE_SPEED;
        new_y += sin(DEG_TO_RAD(map->player.angle + 90)) * MOVE_SPEED;
    }
    else
        return 0;

    // Check if move is valid
    if (is_valid_move(map, (int)new_x, (int)new_y))
    {
        map->player.player_x = new_x;
        map->player.player_y = new_y;
        render_game_view(map);
    }

    return 0;
}

int handle_close(t_map *map)
{
    if (map->win)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }
    exit(0);
    return 0;
}
