# ncurses Log Viewer

## Overview
The ncurses Log Viewer is a terminal-based application that allows users to monitor and filter log output in real-time. It features two panels: the first panel displays filtered log information using `grep`, while the second panel allows users to highlight specific log entries, which are then excluded from the first panel's output.

## Features
- Real-time log monitoring from the `logs` command.
- Two-panel interface:
  - **Panel 1**: Displays log output filtered by `grep`.
  - **Panel 2**: Allows users to highlight specific log entries.
- Dynamic updating of both panels based on user input.

## Installation
To build the project, ensure you have the necessary development tools installed, including `gcc` and `ncurses` library. You can install the `ncurses` library using your package manager. For example, on Ubuntu, you can run:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Clone the repository and navigate to the project directory:

```bash
git clone <repository-url>
cd ncurses-log-viewer
```

## Build Instructions
To compile the application, run the following command in the project directory:

```bash
make
```

This will generate the executable file for the ncurses Log Viewer.

## Usage
To run the application, execute the following command:

```bash
./ncurses-log-viewer
```

The application will start displaying log output. Use the controls in the second panel to highlight specific entries, which will be excluded from the first panel's display.

## Dependencies
- `gcc` (GNU Compiler Collection)
- `ncurses` library

## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.