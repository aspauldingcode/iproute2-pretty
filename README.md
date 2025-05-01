# iproute2-pretty

A command-line utility that provides a more readable and user-friendly output for the `ip a` command from the iproute2 package.

## Overview

iproute2-pretty parses the output of the standard `ip a` command and renders it in a clean, formatted table that makes network interface information easier to read and understand.

## Features

- Displays network interfaces in a well-formatted table
- Shows key information including:
  - Interface name
  - MAC address
  - IPv4 address
  - IPv6 address
  - Interface status (UP/DOWN)
  - MTU value
- Color-coded output for better readability

## Project Structure

The project follows a modular design with clear separation of concerns:

- `src/` - Source code files
  - `main.c` - Program entry point
  - `interface.c` - Interface data structure implementation
  - `parser.c` - Parses the output of `ip a` command
  - `renderer.c` - Renders the parsed data as a formatted table
  - `utils.c` - Utility functions

- `include/` - Header files
  - `interface.h` - Interface data structure definition
  - `parser.h` - Parser function declarations
  - `renderer.h` - Renderer function declarations
  - `utils.h` - Utility function declarations

- `bin/` - Compiled executable
- `obj/` - Object files generated during compilation

## Building from Source

### Prerequisites

- GCC or compatible C compiler
- Make

### Compilation

```bash
# Clone the repository
git clone https://github.com/aspauldingcode/iproute2-pretty.git
cd iproute2-pretty

# Build the project
make
```

The executable will be created in the `bin/` directory.

## Usage

Simply run the executable:

```bash
./bin/ip_a_pretty
```

The program will execute the `ip a` command, parse its output, and display a formatted table of network interfaces.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request
