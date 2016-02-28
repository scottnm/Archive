import argparse
import gtreegraph
import os.path
import pickle
import sys

def init(argv):
    # gtree = pickle.load(".gtree") if os.path.exists(".gtree") else gtreegraph()
    print "init: ", argv

def add(argv):
    print "add: ", argv

def rm(argv):
    print "rm: ", argv

def show(argv):
    print "show: ", argv

def path(argv):
    print "path: ", argv

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
    show_parser.add_argument('subtree', default='master')
    show_parser.set_defaults(func=show)

    path_parser = subparsers.add_parser('path')
    path_parser.add_argument('branch', nargs='?', default=None)
    path_parser.set_defaults(func=path)

    args = parser.parse_args()
    args.func(vars(args))
