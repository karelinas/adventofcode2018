import re
from static_vars import static_vars

def main():
    with open("data.txt", "r", encoding="utf8") as data:
        grid = new_grid()
        for x, y, width, height in [parse_claim(line.strip())
                                    for line in data.readlines()]:
            fill(grid, x, y, width, height)
        print("Overlaps: {}".format(count_overlaps(grid)))

def new_grid(xsize=1000, ysize=1000):
    return [[0 for _ in range(xsize)] for _ in range(ysize)]

def fill(grid, x, y, width, height):
    for y_prime in range(y, y+height):
        for x_prime in range(x, x+width):
            grid[y_prime][x_prime] += 1

def count_overlaps(grid):
    return sum([sum([1 if x > 1 else 0 for x in line]) for line in grid])

@static_vars(matcher = re.compile(r'^#\d+ @ (\d+),(\d+): (\d+)x(\d+)$'))
def parse_claim(claim):
    m = parse_claim.matcher.match(claim)
    if m:
        return (int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)))
    else:
        return (0, 0, 0, 0)

if __name__ == '__main__':
    main()
