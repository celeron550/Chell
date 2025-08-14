# chell

chell is a simple command-line shell written in C. It supports running external programs and includes a few built-in commands:

## Features

- Run external commands (e.g., `ls`, `echo`, etc.)
- Built-in commands:
  - `cd` – Change the current directory
  - `help` – Display help information
  - `exit` – Exit the shell

## Usage

Compile with:

```sh
gcc -o chell main.c
```

And run with: 
```sh
./chell
```

## TODO
- support pipe ```|``` commands
- make history

## Credits

Based on Stephen Brennan's [LSH](https://github.com/brenns10/lsh/)
