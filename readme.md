# Vectorial

## A library that brings Vector-Style appends to a list with rotating buffers

#### **Note:** This library uses a fixed-size circular buffer and does not allocate dynamic memory.

Vectorial is a library that brings append features to be able to append values to a list. This library creates a new struct Vector, which uses a list as a rotating buffer, which automatically can overwrite old entries if the list is full. The library can be very applicable to aspects such as data storage, PIDs, etc.

# Installation

## Install from Github

1. Press the green Download Code button, and download it as a zip.
2. In arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library
3. Select your .zip downloaded form github.

## Install from Arduino's Library Manager

This library is not in the Library Manager yet, although this description will be updated when it is.

# Documentation

## Creating a Vector

A vector can be simply created using the following block. The type is created using a template that specifies the data type and fixed buffer size.

In the below example, the vector has a size of 10 elements, and has a data type of int.

```cpp
Vector<int, 10> vector;
```

## Accessing an element of a Vector

An element of the vector can be accessed normally through a list accessor, for example getting indices 0 and 1 of the above vector and saving it to a variable we can use the following.

#### **Note: Index 0 always refers to the most recently appended element, with increasing indices moving toward older values.**

```cpp
int latest_value = vector[0];
int older_value = vector[1];
```

## Functions

| Sno. | Function name and args.                     | Usage                             | Functionality                                                                          |
| ---- | ------------------------------------------- | --------------------------------- | -------------------------------------------------------------------------------------- |
| 1    | push_back(element, appendType (optional) ) | vector.push_back(5, Cycle/Ignore) | Appends an element to the list                                                         |
| 2    | size()                                      | vector.size()                     | Gets the current number of elements in the vector<br />(useful to check if it is full) |
| 3    | reset()                                     | vector.reset()                    | Resets the vector to empty.                                                            |

### push_back()

The push_back() function takes 2 arguments:

1. An element (float, int, string, or another data type in the same type as the buffer)
2. An enum appendType value which can be either Cycle or Ignore.
   a. If the mode is Cycle, then on append, if the vector is full, oldest values are overwritten.
   b. If the mode is Ignore, then on append, if the vector is full, the write doesn't go through.

The latter is an optional argument that defaults to Cycle.

An example usage that appends 5 to the list regardless of whether the vector is full is as follows:

```cpp
vector.push_back(5);
// Or one could also use
vector.push_back(5, appendType::cycle);
// If trying to be more explicit
```

### size()

The Size() function takes no arguments but returns the number of elements in the vector.

For example:

```cpp
int size = vector.size()
```

### reset()

The reset function also takes no arguments but resets the vector with no return value.

For example:

```cpp
vector.reset()
```

# Examples

An example can be found in examples/Basic/Basic.ino or as Basic.ino in the top bar.

# Contribution guidelines

Although this is a relatively small project, we allow external contributions. However no PR is to be accepted without a corresponding issue that it addresses. To contribute, create a fork of the repository before pushing.

# Enjoy no longer having to append with index gymnastics again!
