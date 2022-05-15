import subprocess

class GTNode:
    def __init__(self, name, parent=None, branches=set()):
        self.name = name
        self.parent = parent
        self.branches = branches

class GTreeGraph:
    def __init__(self):
        self.root = GTNode("master")
        self.node_map = {"master": self.root}

    def addBranch(self, parentBranchName, childBranchName):
        p = self.node_map[parentBranchName]
        c = GTNode(childBranchName, p)
        self.node_map[childBranchName] = c
        p.branches.add(c)

    def removeBranch(self, childBranchName):
        c = self.node_map[childBranchName]
        p = c.parent
        p.branches.remove(c)
        self.node_map.pop(childBranchName, None)

    def show(self, subtreeName):
        root = self.node_map[subtreeName]
        self.__print(root, "")

    def __current(self):
        return subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"]).rstrip()
        

    def path(self, currname=None):
        if currname is None:
            currname = self.__current()
        current = self.node_map[currname]

        names_in_path = [current.name]
        node = current.parent
        while node is not None:
            names_in_path.insert(0, node.name)
            node = node.parent
        pathstr = " --> ".join(names_in_path)
        print pathstr

    def __print(self, root, spaces):
        additionalspaces = " "
        for i in range(len(root.name)):
            additionalspaces += " "
        for branch in root.branches:
            self.__print(branch, spaces + additionalspaces)
        print spaces + root.name
