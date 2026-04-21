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

Full documentation is available in **[documentation.md](**documentation.md**)**.

# Examples

Example sketches are provided:

- **Basic Vector**: `examples/Basic/Basic.ino`
- **TimedVector**: `examples/TimedVector/TimedVector.ino`

# Contribution guidelines

Although this is a relatively small project, we allow external contributions. However no PR is to be accepted without a corresponding issue that it addresses. To contribute, create a fork of the repository before pushing.

# Enjoy no longer having to append with index gymnastics again!
