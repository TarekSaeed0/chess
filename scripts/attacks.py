from enum import Enum


class ChessPieceType(Enum):
    NONE = 0

    PAWN = 1
    KNIGHT = 2
    BISHOP = 3
    ROOK = 4
    QUEEN = 5
    KING = 6


class ChessOffset(Enum):
    NORTH = 16
    EAST = 1
    SOUTH = -NORTH
    WEST = -EAST

    NORTH_EAST = NORTH + EAST
    SOUTH_EAST = SOUTH + EAST
    SOUTH_WEST = SOUTH + WEST
    NORTH_WEST = NORTH + WEST


PIECE_OFFSETS = {
    ChessPieceType.PAWN: [
        ChessOffset.NORTH_EAST.value,
        ChessOffset.NORTH_WEST.value,
        ChessOffset.SOUTH_EAST.value,
        ChessOffset.SOUTH_WEST.value,
    ],
    ChessPieceType.KNIGHT: [
        2 * ChessOffset.NORTH.value + ChessOffset.EAST.value,
        2 * ChessOffset.NORTH.value + ChessOffset.WEST.value,
        2 * ChessOffset.EAST.value + ChessOffset.NORTH.value,
        2 * ChessOffset.EAST.value + ChessOffset.SOUTH.value,
        2 * ChessOffset.SOUTH.value + ChessOffset.EAST.value,
        2 * ChessOffset.SOUTH.value + ChessOffset.WEST.value,
        2 * ChessOffset.WEST.value + ChessOffset.NORTH.value,
        2 * ChessOffset.WEST.value + ChessOffset.SOUTH.value,
    ],
    ChessPieceType.BISHOP: [
        ChessOffset.NORTH_EAST.value,
        ChessOffset.SOUTH_EAST.value,
        ChessOffset.SOUTH_WEST.value,
        ChessOffset.NORTH_WEST.value,
    ],
    ChessPieceType.ROOK: [
        ChessOffset.NORTH.value,
        ChessOffset.EAST.value,
        ChessOffset.SOUTH.value,
        ChessOffset.WEST.value,
    ],
    ChessPieceType.QUEEN: [
        ChessOffset.NORTH.value,
        ChessOffset.EAST.value,
        ChessOffset.SOUTH.value,
        ChessOffset.WEST.value,
        ChessOffset.NORTH_EAST.value,
        ChessOffset.SOUTH_EAST.value,
        ChessOffset.SOUTH_WEST.value,
        ChessOffset.NORTH_WEST.value,
    ],
    ChessPieceType.KING: [
        ChessOffset.NORTH.value,
        ChessOffset.EAST.value,
        ChessOffset.SOUTH.value,
        ChessOffset.WEST.value,
        ChessOffset.NORTH_EAST.value,
        ChessOffset.SOUTH_EAST.value,
        ChessOffset.SOUTH_WEST.value,
        ChessOffset.NORTH_WEST.value,
    ],
}

attacks = [0] * 256
directions = [0] * 256

for piece_type in [ChessPieceType.PAWN, ChessPieceType.KNIGHT, ChessPieceType.KING]:
    for piece_offset in PIECE_OFFSETS[piece_type]:
        attacks[piece_offset + 0x77] |= 1 << piece_type.value

for piece_type in [ChessPieceType.BISHOP, ChessPieceType.ROOK, ChessPieceType.QUEEN]:
    for piece_offset in PIECE_OFFSETS[piece_type]:
        offset = piece_offset + 0x77
        while 0 <= offset and offset < 256 and offset % 16 != 15 and offset // 16 != 15:
            attacks[offset] |= 1 << piece_type.value
            directions[offset] = piece_offset
            offset += piece_offset

print("static const uint8_t attacks[256] = {")
for y in range(0, 16):
    print("\t", end="")
    for x in range(0, 16):
        print(f"{attacks[y * 16 + x]:3}", end=",")
    print("")
print("};")

print("static const enum chess_offset directions[256] = {")
for y in range(0, 16):
    print("\t", end="")
    for x in range(0, 16):
        print(
            # f"{'CHESS_OFFSET_' + ChessOffset(directions[y * 16 + x]).name if directions[y * 16 + x] != 0 else 0}",
            f"{directions[y * 16 + x]:3}",
            end=",",
        )
    print("")
print("};")
