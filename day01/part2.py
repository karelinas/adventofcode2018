
def main():
    with open("data.txt", "r", encoding="utf8") as f:
        deltas = [int(line.strip()) for line in f.readlines()]
        frequency = 0
        seen = set()
        while True:
            for delta in deltas:
                frequency = frequency + delta
                if frequency in seen:
                    print("found {}".format(frequency))
                    return
                seen.add(frequency)

if __name__ == '__main__':
    main()
