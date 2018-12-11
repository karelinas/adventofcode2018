def main():
    serial = 6392
    value, x, y = max_coordinate(sum_neighbors(fuel_grid(serial)))
    print("part 1: ({}, {}) = {}".format(x, y, value))

    # part 2 -- best total power seems to go into the negative when sizes
    #           get too big, so simply stop searching once that happens
    best_size = (0, 0, 0, 0)
    for size in range(1, 301):
        value, x, y = max_coordinate(sum_neighbors(fuel_grid(serial), size))
        if value > best_size[2]:
            best_size = (x, y, value, size)
        if value < 0:
            break
    x, y, value, size = best_size
    print("part 2: size({}) => ({}, {}) = {}".format(size, x, y, value))

def fuel_grid(serial, xsize=300, ysize=300):
    return [[fuel_cell(x, y, serial) for x in range(xsize)] for y in range(ysize)]

def fuel_cell(x, y, serial):
    rack_id = x + 10
    return hundreds(((rack_id * y) + serial) * rack_id) - 5

def hundreds(num):
    return abs(num) // 100 % 10

def sum_neighbors(grid, size=3):
    ylen = len(grid)
    xlen = min([len(line) for line in grid])
    return [[sum([sum(grid[row][x:x+size]) for row in range(y, y+size)])
             for x in range(xlen-size+1)] for y in range(ylen-size+1)]

def max_coordinate(grid):
    return max([
        max([(val, x, y) for x, val in enumerate(line)], key=lambda t: t[0])
        for y, line in enumerate(grid)
    ], key=lambda t: t[0])

if __name__ == '__main__':
    main()
