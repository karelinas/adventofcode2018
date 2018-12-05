def main():
    with open("data.txt", "r", encoding="utf8") as data:
        print(find_similar((line.strip() for line in data.readlines())))

def find_similar(words):
    seen = set()
    for word in words:
        wildcards = make_wildcards(word)
        match = find_one_in_set(seen, wildcards)
        if match is not None:
            print("{} matches {}".format(word, match))
            return remove_wildcard(match)
        seen.update(wildcards)
    return None

def find_one_in_set(haystack, needles):
    for needle in needles:
        if needle in haystack:
            return needle
    return None

def remove_wildcard(word, wildcard='?'):
    return word.replace(wildcard, '')

def make_wildcards(word, wildcard='?'):
    chars = list(word)
    return [
        "".join((chars[:i] + [wildcard] + chars[i+1:])) 
        for i in range(len(chars))
    ]

if __name__ == '__main__':
    main()
