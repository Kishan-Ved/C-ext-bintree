//
//  bintree_module.h
//  BinTree
//
//  Created by dwd on 12/27/17.
//  Copyright © 2017 holdendou. All rights reserved.
//

#ifndef bintree_module_h
#define bintree_module_h

#include <stdio.h>
#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    PyObject* key;
    PyObject* data;
    PyObject* left;
    PyObject* right;
} BinTree;

static PyMemberDef BinTreeMembers[] = {
    {"key", T_OBJECT_EX, offsetof(BinTree, key), 0, "key of bintree node"},
    {"data", T_OBJECT_EX, offsetof(BinTree, data), 0, "data of a bintree node"},
    {"left", T_OBJECT_EX, offsetof(BinTree, left), 0, "left child"},
    {"right", T_OBJECT_EX, offsetof(BinTree, right), 0, "right child"},
    {NULL}
};

static PyObject* BinTreeListify(BinTree* self);
static PyObject* BinTreeInsert(BinTree* self, PyObject* args, PyObject* kwargs);

static PyMethodDef BinTreeMethods[] = {
    {
        "listify",
        (PyCFunction)BinTreeListify,
        METH_NOARGS,
        "inorder traversal of the binary tree"
    },
    {
        "insert",
        (PyCFunction)BinTreeInsert,
        METH_VARARGS | METH_KEYWORDS,
        "insert an element"
    },
    // {
    //     "find",
    //     (PyCFunction)BinTreeFind,
    //     METH_VARARGS,
    //     "find an element"
    // },
    {NULL}
};

#endif /* bintree_module_h */
