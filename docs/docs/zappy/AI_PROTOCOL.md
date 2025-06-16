# ZAPPY - AI Protocol

## 📖 Overview

This document describes the client/server communication protocol used in the Zappy Artificial Inteligence (AI). It includes the name of the commands, their formats, and explanations of the effects of each command.

⚠️ Every command must end with a newline character (`\n`).

---

## 🧭 Client → Server Commands

|            Action            |       Command      | Time limit |                 Response                 |
|:----------------------------:|:------------------:|:----------:|:----------------------------------------:|
|       move up one tile       |     __Forward__    |     7/f    |                    ok                    |
|        turn 90° right        |      __Right__     |     7/f    |                    ok                    |
|         turn 90° left        |      __Left__      |     7/f    |                    ok                    |
|          look around         |      __Look__      |     7/f    |            [tile1, tile2,...]            |
|           inventory          |    __Inventory__   |     1/f    |        [linemate n, sibur n, ...]        |
|        broadcast text        | __Broadcast text__ |     7/f    |                    ok                    |
|  number of team unused slots |   __Connect_nbr__  |      -     |                   value                  |
|         fork a player        |      __Fork__      |    42/f    |                    ok                    |
| eject players from this tile |      __Eject__     |     7/f    |                   ok/ko                  |
|          take object         |   __Take object__  |     7/f    |                   ok/ko                  |
|        set object down       |   __Set object__   |     7/f    |                   ok/ko                  |
|       start incantation      |   __Incantation__  |    300/f   | Elevation underway | Current level: k/ko |

> In case of a bad/unknown command, the server answer “ko”.

## ✨ Special Commands

### 👋 WELCOME command

The welcome command is sent by the server to the client when it connects.
The AI must reply with the team name to authenticate itself.

After the team name is sent, the server will send the map size and the number of slots available for the team.

It should look like this, where `->` is the server and `<-` is the client:

```
-> WELCOME
<- {team_name}
-> {map_x} {map_y}
-> {slots_available}
```

### ☠️ DEAD command

The dead command is sent by the server to the client when the AI dies.
The AI does not need to send any command to get this information, and it also does not need to reply to it.
