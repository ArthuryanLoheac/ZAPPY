/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** calculate the direction of the broadcast
*/

static int facing_west(int dir)
{
    int new_dir = dir + 6;

    if (new_dir <= 8)
        return (new_dir);
    return (new_dir - 8);
}

static int facing_south(int dir)
{
    int new_dir = dir + 4;

    if (new_dir <= 8)
        return (new_dir);
    return (new_dir - 8);
}

static int facing_east(int dir)
{
    int new_dir = dir + 2;

    if (new_dir <= 8)
        return (new_dir);
    return (new_dir - 8);
}

int calibrating_direction(int dir, int orient)
{
    if (dir == 0)
        return (dir);
    if (orient == 0)
        return (facing_west(dir));
    if (orient == 2)
        return (facing_east(dir));
    if (orient == 3)
        return (facing_south(dir));
    return (dir);
}
