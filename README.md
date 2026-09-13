# minitop

minitop is a lightweight Linux process monitor written in C. It provides clear views of running processes in basic, detailed, and raw formats, inspired by the `top` command.

## Features

- Shows process information in basic, detailed, or raw modes
- Displays PID, command, state, user, memory usage, and CPU time
- Refreshes output at a configurable interval

## Usage

Build the project:

```sh
make all
```

Run minitop with options:

```sh
./minitop [-s <seconds>] [-basic | -detailed | -raw]
```

- `-s <seconds>`: Set refresh interval (default: 1 second)
- `-basic`: Show basic process info
- `-detailed`: Show detailed info (default)
- `-raw`: Show raw process info
- `-help` or `--help`: Show usage

## Example

```sh
./minitop -s 2 -basic
```

## Project Structure

- `src/main.c`: Main entry point and argument parsing
- `src/proc_reader.c` / `src/proc_reader.h`: Process info reading and display functions

## License
