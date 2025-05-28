# 🎮 Zappy - Virtual World Simulator

![Game](https://img.shields.io/badge/Game-Simulation-orange)
![Language](https://img.shields.io/badge/Language-C/C++-blue)
![Status](https://img.shields.io/badge/Status-In%20Development-green)

## 📜 Overview

Zappy is a network game where several teams confront each other on a tile map containing resources. The game simulates a virtual world called Trantor where AI-controlled beings called "trantorians" must evolve and survive.

## 🌍 Project Architecture

The Zappy project is divided into 3 components:

### 🖥️ Server
The server manages the simulation of the Trantor world, including:
- Communication between AI clients and the GUI
- Resource spawning and distribution
- Verification of trantorian deaths
- Time unit management for action execution

### 🤖 AI Client
The AI client connects to the server and controls the trantorians with the goal of:
- Gathering resources necessary for evolution
- Having 6 trantorians of the team reach level 8
- Implementing survival strategies in a competitive environment

### 🎨 GUI (Graphical User Interface)
The GUI renders the Trantor world in 3D to help users visualize:
- Trantorian positions and movements
- Resource distribution on the map
- Team progress and evolution levels

## 💻 Usage

### Server

```sh
./zappy_server -help
```

```text
USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq

option                           description
-p port                          port number
-x width                         width of the world
-y height                        height of the world
-n name1 name2 ...               name of the team
-c clientsNb                     number of authorized clients per team
-f freq                          reciprocal of time unit for execution of actions
```

### GUI

```sh
./zappy_gui -help
```

```text
USAGE: ./zappy_gui -p port -h machine

option                           description
-p port                          port number
-h machine                       hostname of the server
```

### AI Client

```sh
./zappy_ai -help
```

```text
USAGE: ./zappy_ai -p port -n name -h machine

option                           description
-p port                          port number
-n name                          name of the team
-h machine                       name of the machine; localhost by default
```

## 🚀 Getting Started

### Prerequisites

- C/C++ compiler (gcc/g++)
- Make
- Any 3D graphics library dependencies for the GUI

### Installation

1. Clone the repository
```sh
git clone <repository-url>
cd ZAPPY
```

2. Compile the project
```sh
make
```

This will build all three components: the server, AI clients, and GUI.

### Running a Game Session

1. Start the server:
```sh
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 5 -f 100
```

2. Launch the GUI:
```sh
./zappy_gui -p 4242 -h localhost
```

3. Connect AI clients:
```sh
./zappy_ai -p 4242 -n team1 -h localhost
```

## 🏗️ Project Structure

```sh
├── zappy_server_src/  # Server implementation
├── zappy_ai_src/      # AI client implementation
├── zappy_gui_src/     # GUI implementation
├── docs/              # Documentation
├── tests/             # Test files
└── private/           # Additional resources
```

## 👥 Contributors

- [ArthuryanLoheac](https://github.com/ArthuryanLoheace)
- [Léopold PINTARD](https://github.com/Leopold-PINTARD)
- [Naat](https://github.com/NathanBezard)
- [Mathys Fillion](https://github.com/mathys-f)
- [Matthieu](https://github.com/MatthieuGA)
- [Jean BIZEUL](https://github.com/JeanBizeul)

## 📝 License

This project is part of the EPITECH curriculum and follows its guidelines.
