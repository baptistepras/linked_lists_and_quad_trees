# Linked Lists and Quadtrees

Algorithmic exercises in C on recursion, linked lists and quadtrees.

| File | Content |
| --- | --- |
| `PART1.c` | Recursion: Ackermann functions, recursive sequences, float versus double precision |
| `PART2.c` | Linked lists, with iterative, recursive and tail-recursive versions |
| `PART2P.c` | Lists of lists and permutations |
| `PART2Z.c` | Lists with a pointer to their last element |
| `PART3.c` | Black and white images stored as quadtrees: display, reading, area, compression, dilation, checkerboard counting |

## Usage

Each file is a standalone program:

```bash
gcc PART1.c -o part1 && ./part1
```

`InterUnion` and `CompteDamiers` in `PART3.c` do not work in every case (see the comments at the top of the file).

## Authors

Javier Peña Castaño and Baptiste Pras.
