import argparse
import gtreeerrors as GTreeError
import gtreegraph as GTreeGraph
import os.path
import pickle
import sys

def init(argv):
    print "init: ", argv
    if os.path.exists(".gtree"):
        raise GTreeError.InitError
    writeGtree(GTreeGraph.GTreeGraph())

def add(argv):
    print "add: ", argv
    gtree = loadGtree()
    gtree.addBranch(argv["parent"], argv["child"])
    writeGtree(gtree)

def rm(argv):
    print "rm: ", argv
    gtree = loadGtree()
    gtree.removeBranch(argv["parent"], argv["child"])
    writeGtree(gtree)

def show(argv):
    print "show: ", argv
    gtree = loadGtree()
    print gtree.node_map.keys()

def path(argv):
    print "path: ", argv

def loadGtree():
    try:
        with open(".gtree", "r") as gtfile:
            return pickle.load(gtfile)
    except IOError:
        raise GTreeError.GTreeNotFoundError

def writeGtree(gtree):
    with open(".gtree", "w") as gtfile:
       pickle.dump(gtree, gtfile)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()

    init_parser = subparsers.add_parser('init')
    init_parser.set_defaults(func=init)

    badd_parser = subparsers.add_parser('add')
    badd_parser.add_argument('parent')
    badd_parser.add_argument('child')
    badd_parser.set_defaults(func=add)

    brm_parser = subparsers.add_parser('rm')
    brm_parser.add_argument('parent')
    brm_parser.add_argument('child')
    brm_parser.set_defaults(func=rm)

    show_parser = subparsers.add_parser('show')
    show_parser.add_argument('-subtree', default='master')
    show_parser.set_defaults(func=show)

    path_parser = subparsers.add_parser('path')
    path_parser.add_argument('-branch', default=None)
    path_parser.set_defaults(func=path)

    try:
        args = parser.parse_args()
        args.func(vars(args))
    except GTreeError.InitError:
        print "Cannot initialize a new GitTree. Another GitTree already exists. Use 'rm .gtree' to delete it."
    except GTreeError.GTreeNotFoundError:
        print "Operation failed: Gtree file did not exist. Ensure that a Gtree file exists by using 'init'"
