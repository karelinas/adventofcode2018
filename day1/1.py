with open("data.txt", "r", encoding="utf8") as f:
    print(sum([int(line.strip()) for line in f.readlines()]))