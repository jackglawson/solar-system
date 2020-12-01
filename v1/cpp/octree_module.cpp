#include <Python.h>
#include "Octree.h"


PyObject* vectorToTuple_Float(const vector<double>& data) {
	PyObject* tuple = PyTuple_New(data.size());
	if (!tuple) throw logic_error("Unable to allocate memory for Python tuple");
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject* num = PyFloat_FromDouble((double)data[i]);
		if (!num) {
			Py_DECREF(tuple);
			throw logic_error("Unable to allocate memory for Python tuple");
		}
		PyTuple_SET_ITEM(tuple, i, num);
	}

	return tuple;
}


PyObject* vectorVectorToTuple_Float(const vector< vector< double > >& data) {
	PyObject* tuple = PyTuple_New(data.size());
	if (!tuple) throw logic_error("Unable to allocate memory for Python tuple");
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject* subTuple = NULL;
		try {
			subTuple = vectorToTuple_Float(data[i]);
		}
		catch (logic_error& e) {
			throw e;
		}
		if (!subTuple) {
			Py_DECREF(tuple);
			throw logic_error("Unable to allocate memory for Python tuple of tuples");
		}
		PyTuple_SET_ITEM(tuple, i, subTuple);
	}

	return tuple;
}


vector<double> tupleToVector_Float(PyObject* incoming) {
	vector<double> data;
	if (PyTuple_Check(incoming)) {
		for (Py_ssize_t i = 0; i < PyTuple_Size(incoming); i++) {
			PyObject* value = PyTuple_GetItem(incoming, i);
			data.push_back(PyFloat_AsDouble(value));
		}
	}
	else {
		if (PyList_Check(incoming)) {
			for (Py_ssize_t i = 0; i < PyList_Size(incoming); i++) {
				PyObject* value = PyList_GetItem(incoming, i);
				data.push_back(PyFloat_AsDouble(value));
			}
		}
		else {
			throw logic_error("Passed PyObject pointer was not a list or tuple!");
		}
	}
	return data;
}


vector<vector<double>> tupleTupleToVector_Float(PyObject* incoming) {
	vector<vector<double>> data;
	if (PyTuple_Check(incoming)) {
		for (Py_ssize_t i = 0; i < PyTuple_Size(incoming); i++) {
			vector<double> row;
			PyObject* py_row = PyTuple_GetItem(incoming, i);
			for (Py_ssize_t j = 0; j < PyTuple_Size(PyTuple_GetItem(incoming, 0)); j++) {
				PyObject* value = PyTuple_GetItem(py_row, j);
				row.push_back(PyFloat_AsDouble(value));
			}
			data.push_back(row);
		}
	}
	else {
		cout << "NOT IMPLEMENTED YET!";
	}
	return data;
}


PyObject* find_acc_cpp(PyObject* rs_tuple, PyObject* ms_tuple, PyObject* radii_tuple, PyObject* params_tuple) {
	// Calculates the accelerations of all particles
	// settings are stored in tuple (qt, G)

	vector<vector<double>> rs_vector = tupleTupleToVector_Float(rs_tuple);
	vector<double> ms_vector = tupleToVector_Float(ms_tuple);
	vector<double> radii_vector = tupleToVector_Float(radii_tuple);
	vector<double> params_vector = tupleToVector_Float(params_tuple);
	p::qt = params_vector[0];
	p::G = params_vector[1];

	Octree octree = Octree(rs_vector, ms_vector);
	vector<vector<double>> accelerations_vector;

	for (int n{ 0 }; n < ms_vector.size(); n++) {
		vector<double> a = octree.find_acc(rs_vector[n], ms_vector[n], radii_vector[n]);
		accelerations_vector.push_back(a);
	}

	PyObject* accelerations_tuple = vectorVectorToTuple_Float(accelerations_vector);
	return accelerations_tuple;
}


static PyObject* function_wrapper(PyObject* self, PyObject* args) {
	PyObject* rs;
	PyObject* ms;
	PyObject* radii;
	PyObject* p;
	if (!PyArg_ParseTuple(args, "OOOO", &rs, &ms, &radii, &p)) {
		// raises error if the arguments are not in the right format
		// otherwise, copies the argument to the local variable
		return NULL;
	}

	PyObject* result = find_acc_cpp(rs, ms, radii, p);
	return result;
}


static PyMethodDef mainMethods[] = {
 {"find_acc_cpp",function_wrapper,METH_VARARGS,"Find the gravitational acceleration on a set of particles, using an octree method."},
 {NULL,NULL,0,NULL}
};


static PyModuleDef octree_module = {
 PyModuleDef_HEAD_INIT,
 "octree_module","Gravitational acceleration calculation",
 -1,
 mainMethods
};


PyMODINIT_FUNC PyInit_octree_module(void) {
	return PyModule_Create(&octree_module);
}
