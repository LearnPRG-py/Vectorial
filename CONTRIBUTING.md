# Contributing to Vectorial

Thank you for your interest in contributing! This guide will help you set up your development environment and understand our contribution process.

## Development Workflow

We've provided Python helper scripts to make building, testing, and formatting easy.

### Prerequisites

- **Python 3.8+** (for the helper scripts)
- **Git** (for version control)

### 1. Install Dependencies

Run the installer script to get platform-specific build tools and code formatters:

```bash
python3 src/install_deps.py
```

This installs:
- **CMake** (build system)
- **GCC/MinGW** (C++ compiler)
- **black** (Python code formatter)

On some platforms you may need to enter your password for `sudo` (Linux) or confirmbrew installations (macOS).

### 2. Build and Test

```bash
python3 src/compile.py
```

What this does:
- Configures CMake in `src/build/`
- Builds the test executable
- Runs the test suite

All tests should pass. If they fail, the script will exit with an error code.

**Note:** Generated build files are in `src/build/` and should **not** be committed.

### 3. Format Your Code

Before committing, format your code to match the project style:

```bash
python3 src/format.py
```

This runs:
- `clang-format` on all `.cpp` and `.h` files in `src/`
- `black` on all `.py` files in `src/`

### Quick Setup Checklist

```bash
# 1. Fork and clone the repository
git clone https://github.com/your-username/Vectorial.git
cd Vectorial

# 2. Install dependencies
python3 src/install_deps.py

# 3. Make your changes...

# 4. Build and test
python3 src/compile.py

# 5. Format everything
python3 src/format.py

# 6. Commit and push
git add .
git commit -m "Your message"
git push origin your-branch
```

## Writing Tests

Tests are located in `src/testing/Test.h`. We use a lightweight custom test framework with the following helpers:

### Assertion Helpers

```cpp
// Returns true if equal, false otherwise. Prints PASS/FAIL.
template <typename T>
bool ExpectEQ(T expected, T actual);

// Crashes immediately if not equal (use sparingly).
template <typename T>
void AssertEQ(T expected, T actual);
```

### Test Structure

- `SimpleVectorTest()` - Tests for `Vector<T, N>`
- `TimedVectorTest()` - Tests for `TimedVector<T, N>`

Add new tests as separate functions. Use `ExpectEQ` to check conditions and accumulate failures. Example:

```cpp
bool MyNewFeatureTest() {
    bool all_pass = true;
    std::cout << "Running MyNewFeatureTest...\n";

    Vector<int, 5> buf;
    buf.push_back(42);

    // Use ExpectEQ to check values
    all_pass = ExpectEQ(42, buf[0]) && all_pass;

    // For safe access:
    auto result = buf.get_value(0);
    all_pass = ExpectEQ(true, result.success) && all_pass;
    all_pass = ExpectEQ(42, result.value) && all_pass;

    return all_pass;
}
```

Then add it to `TestRunner()`:

```cpp
void TestRunner() {
    bool success = SimpleVectorTest()
                && TimedVectorTest()
                && MyNewFeatureTest();
    assert(success); // Will crash if any test failed
}
```

### Guidelines for Tests

- **Coverage:** Test all public API functions and edge cases (full buffer, empty buffer, cycling, ignore mode, etc.)
- **Isolation:** Each test should be independent; use fresh objects.
- **Clarity:** Print descriptive messages before each test block.
- **Failures:** Use `ExpectEQ` (which prints both expected and actual) to make failures easy to debug.

## Code Style

- **C++:** Follow the `.clang-format` rules (run via `format.py`). Project uses C++11 standard.
- **Python:** Follow `black` defaults (run via `format.py`).
- **Arduino-specific:** No dynamic memory allocation; keep functions small and efficient.

## Pull Request Process

1. **Create an issue first** – Describe the bug or feature before starting work. This avoids duplicated effort.
2. **Fork the repository** – Work on your own fork.
3. **Create a branch** – Use a descriptive branch name (e.g., `fix-vector-overflow` or `add-timestamp-api`).
4. **Make your changes** – Follow the coding and testing guidelines.
5. **Run tests** – Ensure all tests pass (`python3 src/compile.py`).
6. **Format code** – Run `python3 src/format.py`.
7. **Commit and push** – Write clear commit messages.
8. **Open a PR** – Reference the issue you're addressing.

We'll review your PR, provide feedback, and merge when ready.

## Areas for Contribution

- More comprehensive tests (coverage for corner cases)
- Additional data structures (e.g., `RingBuffer` with different APIs)
- Performance benchmarks
- Documentation improvements
- Arduino Library Manager submission
- Example projects
- CI/CD setup (GitHub Actions)

## Questions?

Open an issue or reach out to the maintainer.

---

Thanks for helping make Vectorial better! 🎉
