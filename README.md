<h1 align=center>🎮 Zappy - Virtual World Simulator</h1>
<p align="center">
    <img src="https://img.shields.io/badge/Game-Simulation-orange" alt="Game"/>
    <img src="https://img.shields.io/badge/Language-C/C++-blue" alt="Language"/>
    <img src="https://img.shields.io/badge/Status-In%20Development-green" alt="Status"/>
</p>

# <p align=center> 📜 Overview </p>

#### Zappy is a network game where several teams confront each other on a tile map containing resources. The game simulates a virtual world called Trantor where AI-controlled beings called "trantorians" must evolve and survive.

# <p align=center> 📖 Lore of Trantor </p>

<p align="center">
    <img src="docs/static/img/trantor.png" alt="Trantor World" width="600"/>
</p>

Trantor is a donut-shaped world recently discovered by humanity, the different nation on Earth have each sent drones, named trantorians, to the planets to harvest its resources. Due to the weird geography of Trantor, the drones have been broken and unable to contact Earth, but the abundant resources of the planet can be used to repair themselves. The drones have then started to upgrade their body, and the first nations that will repair six of their drones will have the necessary power to contact Earth and end new rockets for extraction.

To survive on Trantor, trantorians must keep their energy levels high by consuming resources. Luckly for them, the world grows batteries in the soil, but also rare stones that can be used to upgrade their modules.

Each kingdom needs 6 trantorians with a mini-gun to win the game. The last team standing wins the game and becomes the ruler of Trantor.

# <p align=center> 🌍 Project Architecture </p>

### The Zappy project is divided into 3 components:

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

# <p align=center> 💻 Usage </p>

<details> <summary> <b><span style="font-size: 1.2em;">🖥️ Server</span></b> </summary>

```sh
USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq
```

| Option | Parameter | Description |
|--------|-----------|-------------|
| -p | port | port number |
| -x | width | width of the world |
| -y | height | height of the world |
| -n | name1 name2 ... | name of the team |
| -c | clientsNb | number of authorized clients per team |
| -f | freq | reciprocal of time unit for execution of actions |
| -v, -vv, -vvv | ---- | set verbose level (WARNINGS, INFOS, DEBUGS) |

> The team name GRAPHIC is reserved for the GUI to authenticate itself as such to the server.

</details>

<details> <summary> <b><span style="font-size: 1.2em;">🌍 GUI</span></b> </summary>

```sh
USAGE: ./zappy_gui -p port -h machine
```

| Option | Parameter | Description |
|--------|-----------|-------------|
| -p | port | port number |
| -h | machine | hostname of the server |
| -v, -vv, -vvv | ---- | set verbose level (WARNINGS, INFOS, DEBUGS) |

</details>

<details> <summary> <b><span style="font-size: 1.2em;">🤖 AI Client</span></b> </summary>

```sh
USAGE: ./zappy_ai -p port -n name -h machine
```

| Option | Parameter | Description |
|--------|-----------|-------------|
| -p | port | port number |
| -n | name | name of the team |
| -h | machine | name of the machine; localhost by default |
| -v, -vv, -vvv | ---- | set verbose level (WARNINGS, INFOS, DEBUGS) |

> The team name GRAPHIC is reserved for the GUI to authenticate itself as such to the server.

</details>

# <p align=center> 🚀 Getting Started </p>

<details> <summary> <b><span style="font-size: 1.2em;">⚙️ Prerequisites</span></b> </summary>

- C/C++ compiler (gcc/g++)
- Make
- Irrlicht 3D library

</details>

<details> <summary> <b><span style="font-size: 1.2em;">🛜 Installation</span></b> </summary>

1. Clone the repository
```sh
git clone https://github.com/ArthuryanLoheac/ZAPPY.git
cd ZAPPY
```

2. Compile the project
```sh
make
```

This will build all three components: the server, AI clients, and GUI.

If you want to compile only one part, you can use
```sh
make <name-of-the-binary>
```

Name of the binary is either `zappy-server`, `zappy-ai` or `zappy-gui`.

</details>

<details> <summary> <b><span style="font-size: 1.2em;">🏃‍♂️ Running a game</span></b> </summary>

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
./zappy_ai -p 4242 -n team2 -h localhost
```

</details>

# <p align=center> 🏗️ Project Structure </p>

```sh
├── zappy_server_src/  # Server implementation
├── zappy_ai_src/      # AI client implementation
├── zappy_gui_src/     # GUI implementation
├── docs/              # Documentation
└── tests/             # Test files
```

# <p align=center> 👥 Contributors </p>

- [ArthuryanLoheac](https://github.com/ArthuryanLoheac)
- [Léopold PINTARD](https://github.com/Leopold-PINTARD)
- [Naat](https://github.com/NathanBezard)
- [Mathys Fillion](https://github.com/mathys-f)
- [Matthieu](https://github.com/MatthieuGA)
- [Jean BIZEUL](https://github.com/JeanBizeul)
