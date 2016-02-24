class GTNode:
    def __init__(self, name, parent=None, branches=set()):
        self.name = name
        self.parent = parent
        self.branches = branches

class GTreeGraph:
    def __init__(self):
        self.root = GTNode("master")
        self.node_map = {"master": self.root}

    def addBranch(self, parent, child):
        p = self.node_map[parent]
        c = GTNode(child, p)
        self.node_map[child] = c
        p.branches.add(c)

    def removeBranch(self, parent, child):
        p = self.node_map[parent]
        p.branches.remove(self.node_map[child])
        self.node_map.pop(child, None)

    def show(self, subtree="master"):
        pass

    def current(self, current):
        pass
