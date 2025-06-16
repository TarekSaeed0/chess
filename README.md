# Chess

[![Build Status](https://img.shields.io/github/actions/workflow/status/TarekSaeed0/chess/cmake-multi-platform.yml)](https://github.com/TarekSaeed0/chess/actions?query=workflow%3A%22%22CMake%20on%20multiple%20platforms%22%22%20%20)
[![GitHub Stars](https://img.shields.io/github/stars/TarekSaeed0/chess?style=flat&label=stars)](https://github.com/TarekSaeed0/chess/stargazers)
[![Github License](https://img.shields.io/github/license/TarekSaeed0/chess)](LICENSE)

A portable C library for representing and manipulating chess positions, and generating legal moves. This library provides a clean API for chess engines, GUIs, and tools.

## Features

- Full representation of chess positions, moves, and pieces
- Move generation and validation
- FEN parsing and generation
- Algebraic notation parsing and formatting
- Castling, en passant, promotion, and all standard chess rules
- No dependencies required, [cmocka](https://gitlab.com/cmocka/cmocka) is optionally required for testing

## Usage

Include the main header in your project:

```c
#include <chess.h>
```

### Example

Creating a new position and generating moves

```c
#include <chess.h>
#include <stdio.h>

int main(void) {
  ChessPosition position = chess_position_new();
  ChessMoves moves = chess_moves_generate(&position);

  printf("There is %zu legal moves:\n", moves.count);
  for (size_t i = 0; i < moves.count; i++) {
    char buffer[16];
    chess_move_to_algebraic(&position, moves.moves[i], buffer, sizeof(buffer));
    printf("%s\n", buffer);
  }

  ChessPosition_drop(&position);

  return 0;
}
```

## Building

This library is written in standard C and should compile with any modern C compiler. It requires at least C11 support. To build using cmake:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## API Overview

- `chess_position_new()`: Create a new position with the standard starting position
- `chess_position_drop()`: Destroy the position, freeing all resources held by it.
- `chess_moves_generate()`: Generate all legal moves
- `chess_move_do()`: Make a move on a position
- `chess_position_from_fen()`, `chess_position_to_fen()`: convert to and from FEN
- `*_from_algebraic()`, `*_to_algebraic()`: convert to and from algebraic notation

See the `include/chess/` headers for full API documentation.

## License

MIT License. See [LICENSE](LICENSE) for details.
