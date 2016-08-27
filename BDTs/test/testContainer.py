#!/usr/bin/env python

# unit tests for the container class
# see https://docs.python.org/2/library/unittest.html

import unittest

# notes on tests:
# contains: type, name -> boolean or raises exception
#     check if a container already has an indicated tree, variable, spectator, cut, weight or MVA method
# display:  type -> writes or raises exception
#     print out all currently contained objects, by category, in the terminal or to a file
#     if the container is empty, prints out that it is empty
#     if instructed to write to file but no filename is present, write to a default file name unless it exists; if it exists, do not overwrite
# delete:   type, name -> deletes or raises exception
#     if a container has an indicated data member, delete it
# add:      type, name -> adds or raises exception
#     should add a tree or MVA method in the container
#     should add a variable, spectator, cut, or weight if they correspond to an object in a tree
# the above should work with the following cases and their combinations:
#     all
#     trees
#     variables
#     spectators
#     cuts
#     MVA methods
#     weights
# is_ready             -> boolean
#     checks if a container is ready to be configured for training --- i.e., if it has a minimal set of necessary objects
#     that could be at least one tree, at least one variable, and at least one MVA method.

# based on the above, need the following test suits:
# contains
# display
# delete
# add
# is_ready

# base test cases to be inherited

class EmptyTestCase(unittest.TestCase):
    'Base Test Case on TMVAContainer("TestDefaultTMVAContainer")'

    def setUp(self):
        self.container = TMVAContainer("TestEmptyTMVAContainer")

    def tearDown(self):
        self.container.dispose()
        self.container = None

class DefaultTestCase(unittest.TestCase):
    'Base Test Case on TMVAContainer("TestDefaultTMVAContainer")'

   # **************************************

    present_type = "Present Type"
    present_obj  = "Present Object"
    absent_obj   = "Absent Object"

    absent_type  = "Absent Type"

   # **************************************

    def setUp(self):
        self.container = TMVAContainer("TestDefaultTMVAContainer")

    def tearDown(self):
        self.container.dispose()
        self.container = None


# Test Suite for Contains

class ContainsEmptyTestCase(EmptyTestCase):
    'Tests TMVAContainer.contains on TMVAContainer("TestEmptyTMVAContainer").'

    # tests if empty container raises exception
    def test_empty(self):
        self.assertRaises(InputError):
            self.container.contains("Any Type", "Any Object")
        self.assertRaises(InputError):
            self.container.contains("Any Object")

class ContainsDefaultTestCase(DefaultTestCase):
    'Tests TMVAContainer.contains on TMVAContainer("TestDefaultTMVAContainer").'

    # tests if present_obj is in container
    def test_present(self):
        self.assertTrue(self.container.contains(present_type, present_obj),
            "expected default test container %s to have %s of type %s,  but it is absent"  % (self.container.name(), present_obj, present_type))
        self.assertTrue(self.container.contains(present_obj),
            "expected default test container %s to have %s of any type, but it is absent"  % (self.container.name(), present_obj))
        with self.assertRaises(InputError):
            self.container.contains(absent_type, present_obj)

    # tests if absent_obj is not in container
    def test_absent(self):
        self.assertFalse(self.container.contains(present_type, absent_obj),
            "expected default test container %s to lack %s of type %s,  but it is present" % (self.container.name(),  absent_obj, present_type))
        self.assertFalse(self.container.contains(absent_obj),
            "expected default test container %s to lack %s of any type, but it is present" % (self.container.name(),  absent_obj))
        with self.assertRaises(InputError):
            self.container.contains(absent_type, absent_obj)

def suite_contains():
    pass

# Test Suite for Display

class DisplayEmptyTestCase(EmptyTestCase):
    pass

class DisplayDefaultTestCase(DefaultTestCase):
    pass

def suite_display():
    pass

# Test Suite for Delete

class DeleteEmptyTestCase(EmptyTestCase):
    pass

class DeleteDefaultTestCase(DefaultTestCase):
    pass

def suite_delete():
    pass

# Test Suite for Add

class AddEmptyTestCase(EmptyTestCase):
    pass

class AddDefaultTestCase(DefaultTestCase):
    pass

def suite_add():
    pass
# Test Suite for is_ready

class IsReadyEmptyTestCase(EmptyTestCase):
    pass

class IsReadyDefaultTestCase(DefaultTestCase):
    pass

def suite_is_ready():
    pass
