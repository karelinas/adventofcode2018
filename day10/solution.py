# Run with: python3 solution.py < data.txt

from static_vars import static_vars
from sys import stdin, stdout
import re

def main():
    points = [point(line.strip()) for line in stdin]
    seconds, packed = find_packed(points)
    packed = move_to_origin(packed)
    rect = bounding_rect(packed)
    grid = make_grid(rect['right']+1, rect['bottom']+1)
    plot_points(grid, packed)
    print("Found the message in {} s".format(seconds))
    write_grid(stdout, grid)

def find_packed(points):
    seconds = 0
    area = rect_area(bounding_rect(points))
    while True:
        points = forward_all(points)
        new_area = rect_area(bounding_rect(points))
        if new_area > area:
            return (seconds, reverse_all(points))
        area = new_area
        seconds += 1

def write_grid(out, grid):
    for line in grid:
        out.write("".join(line))
        out.write('\n')

def plot_points(grid, points):
    for point in points:
        x = point['x']
        y = point['y']
        grid[y][x] = '#'

def make_grid(x, y):
    return [[' ' for _ in range(x)] for _ in range(y)]

def move_to_origin(points):
    x_min = min((p['x'] for p in points))
    y_min = min((p['y'] for p in points))
    return [move_point(p, -x_min, -y_min) for p in points]

def move_point(point, x_delta, y_delta):
    return {
        'x'  : point['x'] + x_delta,
        'y'  : point['y'] + y_delta,
        'v_x': point['v_x'],
        'v_y': point['v_y']
    }

def forward_all(points):
    return [forward_point(p) for p in points]

def forward_point(point):
    return move_point(point, point['v_x'], point['v_y'])

def reverse_all(points):
    return [reverse_point(p) for p in points]

def reverse_point(point):
    return move_point(point, -point['v_x'], -point['v_y'])

def bounding_rect(points):
    return {
        'left'  : min((p['x'] for p in points)),
        'right' : max((p['x'] for p in points)),
        'top'   : min((p['y'] for p in points)),
        'bottom': max((p['y'] for p in points)),
    }

def rect_area(rect):
    return height(rect) * width(rect)

def height(rect):
    return rect['bottom'] - rect['top']

def width(rect):
    return rect['right'] - rect['left']

@static_vars(matcher = re.compile(
    r'^position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>$'
))
def point(line):
    m = point.matcher.match(line)
    if not m:
        raise Exception("Invalid input")
    return {
        'x'  : int(m.group(1)),
        'y'  : int(m.group(2)),
        'v_x': int(m.group(3)),
        'v_y': int(m.group(4))
    }

if __name__ == '__main__':
    main()