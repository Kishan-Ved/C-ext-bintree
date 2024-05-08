//
//  bintree_module.c
//  BinTree
//
//  Created by dwd on 12/27/17.
//  Copyright © 2017 holdendou. All rights reserved.
//

#include "bintree_module.h"

/*
 * constructors/destructors
*/
static void BinTreeDealloc(BinTree* self) {
    if ((PyObject*)self == Py_None) { // base case
        Py_DECREF(Py_None);
        return;
    }
    
    // post-order traversal to dealloc descentents
    BinTreeDealloc((BinTree*)self->left);
    BinTreeDealloc((BinTree*)self->right);
    
    Py_XDECREF(self->key);
    Py_XDECREF(self->data);
    Py_TYPE(self)->tp_free((PyObject*)self);
    
}

static PyObject* BinTreeAlloc(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    BinTree * self = (BinTree*)type->tp_alloc(type, 0);
    
    // set all three as None
    if (self) {
        Py_INCREF(Py_None);
        Py_INCREF(Py_None);
        Py_INCREF(Py_None);
        Py_INCREF(Py_None);
        self->key = Py_None;
        self->data = Py_None;
        self->left = Py_None;
        self->right = Py_None;
    }
    
    return (PyObject*)self;
}

static int BinTreeInit(BinTree* self, PyObject *args, PyObject *kwds) {
    const char * kwlist[] = {"key","data", NULL};
    PyObject* key = NULL;
    PyObject* data = NULL;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &key, &data)) {
        return -1;
    }

    if (key) {
        Py_INCREF(key);        
        PyObject* tmp = self->key;
        self->key = key;
        Py_XDECREF(tmp);
    }

    if (data) {
        Py_INCREF(data);        
        PyObject* tmp = self->data;
        self->data = data;
        Py_XDECREF(tmp);
    }
    
    return 0;
    
}

static PyTypeObject BinTreeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "bintree.BinTree", // name
    sizeof(BinTree), // size
    0, // itemsize
    (destructor)BinTreeDealloc,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    "BinTree type object",
    0, 0, 0, 0, 0, 0,
    BinTreeMethods,
    BinTreeMembers,
    0,0,0,0,0,0,
    (initproc)BinTreeInit,
    0,
    BinTreeAlloc
};

/**
 * methods
*/
static int inorder(BinTree* self, PyObject* list) {
    if (self == Py_None) {
        return 0;
    }

    int sts = inorder(self->left, list);
    if (sts < 0) {
        return -1;
    }
    sts = PyList_Append(list, self->data);
    if (sts < 0) {
        return -1;
    }
    sts = inorder(self->right, list);
    if (sts < 0) {
        return -1;
    }
    return sts;
}
static PyObject* BinTreeListify(BinTree* self) {
    PyObject* inorder_list = PyList_New(0);
    if (inorder_list) {
        int sts = inorder(self, inorder_list);
        if (sts < 0) {
            PyErr_SetString(PyExc_AttributeError, "inorder");
            return NULL;
        }
    }
    return inorder_list;
}


static PyObject* BinTreeInsert(BinTree* self, PyObject* args, PyObject* kwargs) {
    static char* keywords[] = {"key", "data", "comparator", NULL};
    PyObject *data = NULL, *key = NULL, *comparator = NULL;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OO|O", keywords, &key, &data, &comparator)) {
        return NULL;
    }

    if (self == Py_None) {
        BinTree * b = BinTreeAlloc(&BinTreeType, NULL, NULL);
        PyObject* argument = Py_BuildValue("(OO)", key, data);
        BinTreeInit(b, argument, NULL);
        Py_DECREF(argument);
        return b;
    }    
    
    if (comparator == NULL) {
        PyObject* tmp;
        if (self->key >= key) { // curr key is larger; insert left
            tmp = self->left;
            self->left = BinTreeInsert(self->left, args, kwargs);
            Py_DECREF(tmp);
        } else if(self->key == key) {
            self->data = data;
        } else {
            tmp = self->right;
            self->right = BinTreeInsert(self->right, args, kwargs);
            Py_DECREF(tmp);
        }
        
        Py_INCREF(self);
        return self;
    } else {
        // Check if the provided comparator is callable
        if (!PyCallable_Check(comparator)) {
            PyErr_SetString(PyExc_ValueError, "comparator should be callable");
            return NULL;
        }

        PyObject* arguments = Py_BuildValue("OO", self->key, key);
        PyObject* comp = PyObject_CallObject(comparator, arguments);
        Py_DECREF(arguments);
        
        if (!PyLong_Check(comp)) {
            PyErr_SetString(PyExc_TypeError, "bad return type from comparator");
            return NULL;
        }
        
        long long comp_result = PyLong_AsLongLong(comp);
        Py_DECREF(comp);
        
        PyObject* tmp;
        if (comp_result == 1) { // curr key is larger; insert left
            tmp = self->left;
            self->left = BinTreeInsert(self->left, args, kwargs);
            Py_DECREF(tmp);
        } else if(comp_result == 0) {
            self->data = data;
        } else {
            tmp = self->right;
            self->right = BinTreeInsert(self->right, args, kwargs);
            Py_DECREF(tmp);
        }
        
        Py_INCREF(self);
        return self;
    }
}

// The below insert() works completely without keywords

// static PyObject* BinTreeInsert(BinTree* self, PyObject* args) {
//     PyObject *key, *elem, *comparator = NULL;
//     if (!PyArg_ParseTuple(args, "OOO", &key, &elem, &comparator)) {
//         return NULL;
//     }
    
//     if (!PyCallable_Check(comparator)) {
//         PyErr_SetString(PyExc_ValueError, "comparator should be callable");
//         return NULL;
//     }
//     if (self == Py_None) {
//         BinTree * b = BinTreeAlloc(&BinTreeType, NULL, NULL);
//         PyObject* argument = Py_BuildValue("(OO)", key, elem);
//         BinTreeInit(b, argument, NULL);
//         Py_DECREF(argument);
//         return b;
//     }
//     PyObject* arguments = Py_BuildValue("OO", self->key, elem);
//     PyObject* comp = PyObject_CallObject(comparator, arguments);
//     Py_DECREF(arguments);
//     if (!PyLong_Check(comp)) {
//         PyErr_SetString(PyExc_TypeError, "bad return type from comparator");
//         return NULL;
//     }
//     long long comp_result = PyLong_AsLongLong(comp);
//     Py_DECREF(comp);
//     PyObject* tmp;
//     if (comp_result == 1) { // curr elem is larger; insert left
//         tmp = self->left;
//         self->left = BinTreeInsert(self->left, args);
//         Py_DECREF(tmp);
//     } else {
//         tmp = self->right;
//         self->right = BinTreeInsert(self->right, args);
//         Py_DECREF(tmp);
//     }
//     Py_INCREF(self);
//     return self;
// }


static PyModuleDef bintreemodule = {
    PyModuleDef_HEAD_INIT,
    "bintree",
    "c extension of bintree",
    -1,
    NULL, NULL, NULL, NULL, NULL
};


PyMODINIT_FUNC PyInit_bintree(void) {
    if (PyType_Ready(&BinTreeType) < 0) {
        return NULL;
    }
    PyObject* m = PyModule_Create(&bintreemodule);
    if (!m) {
        return NULL;
    }
    Py_INCREF(&BinTreeType);
    PyModule_AddObject(m, "BinTree", (PyObject*)&BinTreeType);
    return m;
}







