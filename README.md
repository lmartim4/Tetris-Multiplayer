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

#### On Fedora:

```bash
sudo dnf install SFML-devel enet-devel
```

#### On macOS (using Homebrew):

```bash
brew update
brew install sfml enet
```

#### On Windows:

1. **SFML**: Download and install SFML from [SFML official website](https://www.sfml-dev.org/download.php).
   - Follow the guide for setting up SFML with your compiler (e.g., MinGW or Visual Studio).

2. **ENet**: Download and build ENet from the [GitHub repository](https://github.com/lsalzman/enet).
   - You'll need CMake and a C++ compiler installed on your system to build ENet.

### Building the Project

Once the required libraries are installed, you can compile the project using the provided `Makefile`.

1. Navigate to the project directory:

   ```bash
   cd path/to/project
   ```

2. Build both the client and server:

   ```bash
   make
   ```

3. Alternatively, you can build and run the client or server separately:

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

### Project Structure

- `client.cpp`: The client-side application.
- `server.cpp`: The server-side application.
- `Makefile`: Build instructions for both the client and server.
- `/Source/Headers`: Contains header files.
- `/Source/Resources/Images`: Stores resources like images.

### License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

### Troubleshooting

If you run into any issues, ensure that the SFML and ENet libraries are installed correctly and that your C++ compiler can find the required headers and libraries.
```

You can now copy this entire block and paste it into a `README.md` file in your project directory.