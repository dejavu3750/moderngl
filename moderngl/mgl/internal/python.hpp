#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

#if PY_VERSION_HEX >= 0x03070000
#define METHOD_V(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_FASTCALL, 0}
#define METHOD_N(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_NOARGS, 0}
#define MODULE_METHOD_V(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_FASTCALL, 0}
#define MODULE_METHOD_N(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_NOARGS, 0}
#else
#define METHOD_V(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth ## _slow, METH_VARARGS, 0}
#define METHOD_N(type, meth) { "" # meth, (PyCFunction)type ## _meth_ ## meth, METH_NOARGS, 0}
#define MODULE_METHOD_V(meth) { "" # meth, (PyCFunction)meth_ ## meth ## _slow, METH_VARARGS, 0}
#define MODULE_METHOD_N(meth) { "" # meth, (PyCFunction)meth_ ## meth, METH_NOARGS, 0}
#endif

#define BACKWARD_COMPATIBLE(meth) PyObject * meth ## _slow(PyObject * self, PyObject * args) { return meth(self, ((PyTupleObject *)args)->ob_item, ((PyVarObject *)args)->ob_size); }
#define SLOT(obj, type, offset) (*(type **)((char *)obj + offset))
#define NEW_REF(obj) (Py_INCREF(obj), obj)

inline PyObject * _new_object(PyTypeObject * type) {
    PyObject * res = 0;
    Py_INCREF(type);
    if (type->tp_flags & Py_TPFLAGS_HAVE_GC) {
        res = PyObject_GC_New(PyObject, type);
    } else {
        res = PyObject_New(PyObject, type);
    }
    // PyObject_GC_Track(wrapper);
    memset((char *)res + sizeof(PyObject), 0, type->tp_basicsize - sizeof(PyObject));
    return res;
}

#define new_object(type, typeobj) (type *)_new_object(typeobj)
