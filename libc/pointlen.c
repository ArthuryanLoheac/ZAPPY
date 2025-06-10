/*
** EPITECH PROJECT, 2023
** get function
** File description:
** contain most function that search for things
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int pointlen(char **str)
{
    int i = 0;

    i = 0;
    if (str == NULL)
        return (0);
    while (str[i] != NULL) {
        i++;
    }
    return (i);
}
