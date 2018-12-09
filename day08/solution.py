class Node:
    def __init__(self, children, metadata):
        self.children = children
        self.metadata = metadata

    def size(self):
        return sum([child.size() for child in self.children]) + \
               len(self.metadata) + 2

    def value(self):
        if len(self.children) == 0:
            return sum(self.metadata)
        else:
            return sum([self._child_value(pos-1) for pos in self.metadata])

    def _child_value(self, pos):
        if pos >= 0 and pos < len(self.children):
            return self.children[pos].value()
        else:
            return 0

    def sum_metadata(self):
        return sum([child.sum_metadata() for child in self.children]) + \
               sum(self.metadata)

    def from_list(lst):
        childcount = lst[0]
        metadatacount = lst[1]

        # read children recursively
        children = []
        offset = 2
        for _ in range(childcount):
            child = Node.from_list(lst[offset:])
            children.append(child)
            offset += child.size()

        metadata = lst[offset:offset+metadatacount]
        return Node(children, metadata)


def main():
    with open("data.txt", "r", encoding="utf8") as data:
        integerdata = [int(x) for x in data.read().split()]
        root = Node.from_list(integerdata)
        print("Sum of metadata: {}".format(root.sum_metadata()))
        print("Root node value: {}".format(root.value()))

if __name__ == '__main__':
    main()
