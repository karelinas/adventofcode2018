from collections import defaultdict
from functools import reduce

def main():
    with open("data.txt", "r", encoding="utf8") as data:
        twos, threes = reduce(
            lambda x, y: (x[0] + y[0], x[1] + y[1]),
            twos_and_threes(unique_counts(stripped_lines(data.readlines()))),
            (0, 0)
        )
        print("{} * {} = {}".format(twos, threes, twos * threes))

def stripped_lines(lines):
    return (line.strip() for line in lines)

def unique_counts(words):
    return (
        set(char_counts(word).values())
        for word in words
    )

def twos_and_threes(counts):
    return [(int(2 in count), int(3 in count)) for count in counts]

def char_counts(x):
    return reduce(
        lambda d, key: dict_increment(d, key),
        x,
        defaultdict(int)
    )

def dict_increment(d, key):
    d[key] += 1
    return d

if __name__ == '__main__':
    main()
