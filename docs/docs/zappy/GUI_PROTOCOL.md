# ZAPPY - GUI Protocol

## 📖 Overview

This document describes the client/server communication protocol used in the Zappy graphical user interface (GUI). It includes symbol definitions, server-to-client and client-to-server command formats, and explanations of game events and data structures.

---

## 🧩 Symbol Definitions

| Symbol | Meaning                            |
|--------|-------------------------------------|
| `X`    | Width or horizontal position        |
| `Y`    | Height or vertical position         |
| `q0`   | Resource 0 (Food) quantity          |
| `q1`   | Resource 1 (Linemate) quantity      |
| `q2`   | Resource 2 (Deraumere) quantity     |
| `q3`   | Resource 3 (Sibur) quantity         |
| `q4`   | Resource 4 (Mendiane) quantity      |
| `q5`   | Resource 5 (Phiras) quantity        |
| `q6`   | Resource 6 (Thystame) quantity      |
| `n`    | Player number                       |
| `O`    | Orientation: `1=N`, `2=E`, `3=S`, `4=W` |
| `L`    | Player or incantation level         |
| `e`    | Egg number                          |
| `T`    | Time unit                           |
| `N`    | Team name                           |
| `R`    | Incantation result                  |
| `M`    | Message                             |
| `i`    | Resource number                     |

---

## 🔁 Server → Client Commands

| Command Format | Description |
|----------------|-------------|
| `msz X Y` | Map size |
| `bct X Y q0 q1 q2 q3 q4 q5 q6` | Content of a single tile |
| `tna N` | Name of a team |
| `pnw #n X Y O L N` | Connection of a new player |
| `ppo #n X Y O` | Player position |
| `plv #n L` | Player level |
| `pin #n X Y q0 q1 q2 q3 q4 q5 q6` | Player inventory |
| `pex #n` | Expulsion |
| `pbc #n M` | Broadcast message |
| `pic X Y L #n #n ...` | Start of an incantation |
| `pie X Y R` | End of an incantation |
| `pfk #n` | Player laid an egg |
| `pdr #n i` | Player dropped a resource |
| `pgt #n i` | Player collected a resource |
| `pdi #n` | Player died |
| `enw #e #n X Y` | Egg laid by player |
| `ebo #e` | Egg connects as a player |
| `edi #e` | Egg died |
| `sgt T` | Time unit value |
| `sst T` | Modify time unit |
| `seg N` | End of game |
| `smg M` | Server message |
| `suc` | Unknown command |
| `sbp` | Command parameter error |
| `ping` | Wait the response to see the delay with the server |

---

## 🧭 Client → Server Commands

| Command Format | Description |
|----------------|-------------|
| `ppo #n` | Request player position |
| `mct #n` | Request map datas |
| `plv #n` | Request player level |
| `pin #n` | Request player inventory |
| `sgt` | Request time unit |
| `sst T` | Set time unit |

---

## 📝 Notes

- Tous les messages échangés se terminent par un saut de ligne (`\n`).
- Les identifiants de joueurs et d'œufs sont précédés du symbole `#`, par exemple `#1`, `#42`.
- Les ressources sont indexées de `0` à `6`, selon la table suivante :
  - `0` — Nourriture (food)
  - `1` — Linemate
  - `2` — Deraumere
  - `3` — Sibur
  - `4` — Mendiane
  - `5` — Phiras
  - `6` — Thystame

---

## 📌 Version

**Protocol Version:** `1v 3.3`
