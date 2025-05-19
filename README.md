# FILTR
![FILTR Logo](/images/filtr.png)

**FILTR**  is a real-time log filtering tool with a dual-panel ncurses interface.  
It helps you monitor and interactively filter system logs, making it easier to focus on what matters.

## Features

- **Dual-panel interface:**  
  - **Top panel:** Shows a live stream of logs, excluding lines matching your highlighted patterns.
  - **Bottom panel:** Shows all logs and lets you highlight lines to exclude from the top panel.
- **Interactive highlighting:**  
  - Press `h` to highlight the last line in the bottom panel (adds it to the exclusion filter).
  - Press `q` to quit.
- **Dynamic filtering:**  
  - Highlights are used to build a dynamic `grep` command with `-v -e` for each pattern, excluding those lines from the top panel in real time.

## Usage

1. **Build the application:**
    ```sh
    make
    ```
2. **Run FILTR (you may need sudo for log access):**
    ```sh
    sudo ./filtr
    ```
3. **Controls:**
    - `h`: Highlight the last line in the bottom panel (exclude from top panel)
    - `q`: Quit the application

## How it Works

- FILTR tails your system logs (`/var/log/*.log` and `/var/log/*/*.log`).
- The bottom panel shows all log lines.
- When you highlight a line, FILTR adds it to a list of patterns and rebuilds the top panel’s filter to exclude those patterns using `grep -v -e`.
- The top panel always shows the filtered log stream.

## Requirements

- Linux (tested on Ubuntu)
- ncurses library
- gcc

## Building

```sh
make
```

## Customization

- You can change which logs are tailed by editing the command in `main.c`.
- Highlight matching can be improved by editing the pattern extraction logic.

## License

MIT License

---

*FILTR — Focus on what matters in your logs.*