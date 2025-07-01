from enum import Enum


class ChessPieceType(Enum):
    PAWN = 0
    KNIGHT = 1
    BISHOP = 2
    ROOK = 3
    QUEEN = 4
    KING = 5


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
        ChessOffset.NORTH.value,
        ChessOffset.SOUTH.value,
        2 * ChessOffset.NORTH.value,
        2 * ChessOffset.SOUTH.value,
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
        2 * ChessOffset.EAST.value,
        2 * ChessOffset.WEST.value,
    ],
}

moves = [0] * 256
directions = [0] * 256

for piece_type in [ChessPieceType.PAWN, ChessPieceType.KNIGHT, ChessPieceType.KING]:
    for piece_offset in PIECE_OFFSETS[piece_type]:
        moves[piece_offset + 0x77] |= 1 << piece_type.value

for piece_type in [ChessPieceType.BISHOP, ChessPieceType.ROOK, ChessPieceType.QUEEN]:
    for piece_offset in PIECE_OFFSETS[piece_type]:
        offset = piece_offset + 0x77
        while 0 <= offset and offset < 256 and offset % 16 != 15 and offset // 16 != 15:
            moves[offset] |= 1 << piece_type.value
            directions[offset] = piece_offset
            offset += piece_offset


def offset_to_string(offset):
    if offset == 0:
        return "0"

    def coefficient_to_string(coefficient):
        return str(coefficient) + " * " if coefficient != 1 else ""

    for o in sorted(ChessOffset, key=lambda o: -abs(o.value)):
        if offset % o.value == 0 and 0 < offset // o.value and offset / o.value < 8:
            return coefficient_to_string(offset // o.value) + "CHESS_OFFSET_" + o.name

    horizontal = offset % 16
    vertical = offset // 16

    if horizontal >= 8:
        horizontal -= 16
        vertical += 1

    return (
        coefficient_to_string(abs(horizontal))
        + "CHESS_OFFSET_"
        + ("EAST" if horizontal > 0 else "WEST")
        + " + "
        + coefficient_to_string(abs(vertical))
        + "CHESS_OFFSET_"
        + ("NORTH" if vertical > 0 else "SOUTH")
    )


offsets = [
    coefficient * offset.value + 0x77
    for offset in ChessOffset
    for coefficient in range(8)
]
for y in range(0, 16):
    for x in range(0, 16):
        if y * 16 + x not in offsets:
            offsets.append(y * 16 + x)

print("static CHESS_CONSTEXPR uint8_t moves[256] = {")
for offset in offsets:
    if moves[offset] == 0:
        continue

    print(
        f"\t[{offset_to_string(offset - 0x77)} + 0x77] = {' | '.join('1U << CHESS_PIECE_TYPE_' + type.name for type in ChessPieceType if moves[offset] & (1 << type.value) != 0)},",
    )
print("};")

print("static CHESS_CONSTEXPR ChessOffset directions[256] = {")
for offset in offsets:
    if directions[offset] == 0:
        continue

    print(
        f"\t[{offset_to_string(offset - 0x77)} + 0x77] = {offset_to_string(directions[offset])},",
    )
print("};")
