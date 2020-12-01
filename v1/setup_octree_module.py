from distutils.core import setup, Extension
function_module = Extension('octree_module',sources = ['cpp/octree_module.cpp', 'cpp/functions.cpp', 'cpp/Octree.cpp', 'cpp/OctreeNode.cpp', 'cpp/params.cpp'])
setup(name = 'OctreeExtension',version='1.0',description = 'This is a package for the find_acc_cpp function',ext_modules = [function_module])

# do setup_list.py install
