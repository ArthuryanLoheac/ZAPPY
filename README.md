# Zappy

## Description

The goal of this project is to create a network game where several teams confront each other on a tile map containing resources.

## Usage

```
> ./zappy_server -help
USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq

option                           description
-p port                          port number
-x width                         width of the world
-y height                        height of the world
-n name1 name2 . . .             name of the team
-c clientsNb                     number of authorized clients per team
-f freq                          reciprocal of time unit for execution of actions
```

```
> ./zappy_gui -help
USAGE: ./zappy_gui -p port -h machine

option                           description
-p port                          port number
-h machine                       hostname of the server
```

```
> ./zappy_ai -help
USAGE: ./zappy_ai -p port -n name -h machine

option                           description
-p port                          port number
-n name                          name of the team
-h machine                       name of the machine; localhost by default
```
