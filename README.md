# Tetris Client-Server Application

This project contains a client-server setup for a basic Tetris application using SFML for graphics and ENet for networking. Before you can build and run the project, you need to install the required dependencies.

## Requirements

The project requires the following libraries:

- **SFML (Simple and Fast Multimedia Library)**
- **ENet (Reliable UDP networking library)**

### Installing Dependencies

#### On Ubuntu/Debian:

You can install both **SFML** and **ENet** using the package manager:

```bash
sudo apt-get update
sudo apt-get install libsfml-dev libenet-dev
```

### Building the Project

Once the required libraries are installed, you can compile the project using the provided `Makefile`.

1. Navigate to the project directory:

   ```bash
   mkdir build && cd build && cmake ..
   ```

2. Build and run the client or server separately:

   To build and run the client:
   
   ```bash
   make run-client
   ```

   To build and run the server:

   ```bash
   make run-server
   ```

### Cleaning Up

To clean up the generated files (object files and executables), run:

```bash
make clean
```

### Troubleshooting

//TODO: post commom errors