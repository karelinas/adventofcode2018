import pytest

from solution import fuel_cell, fuel_grid, sum_neighbors, max_coordinate, hundreds

fuel_cell_data = [
    (3, 5, 8, 4),
    (122, 79, 57, -5),
    (217, 196, 39, 0),
    (101, 153, 71, 4)
]

@pytest.mark.parametrize("x,y,serial,expected", fuel_cell_data)
def test_fuel_cell(x, y, serial, expected):
    assert(fuel_cell(x, y, serial) == expected)


grid_data = [
    (18, 33, 45, 29),
    (42, 21, 61, 30),
    (6392, 20, 58, 30)
]

@pytest.mark.parametrize("serial,expected_x,expected_y,expected_val", grid_data)
def test_max_coordinate(serial, expected_x, expected_y, expected_val):
    val, x, y = max_coordinate(sum_neighbors(fuel_grid(serial)))
    assert(x == expected_x)
    assert(y == expected_y)
    assert(val == expected_val)


hundreds_data = [
    (100, 1),
    (-100, 1),
    (1, 0),
    (24515, 5),
    (-24515, 5),
    (-1, 0),
    (-11, 0),
    (-111, 1)
]

@pytest.mark.parametrize("val,expected", hundreds_data)
def test_hundreds(val, expected):
    assert(hundreds(val) == expected)
