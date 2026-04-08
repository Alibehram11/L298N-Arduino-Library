# Contributing to Arduino Car Library

First off, thank you for considering contributing to Arduino Car Library! It's people like you that make this library such a great tool.

## Code of Conduct

Be respectful, inclusive, and professional in all interactions.

## How Can I Contribute?

### 🐛 Reporting Bugs

Before creating bug reports, please check the issue list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible:

* **Use a clear and descriptive title**
* **Describe the exact steps which reproduce the problem**
* **Provide specific examples to demonstrate the steps**
* **Describe the behavior you observed after following the steps**
* **Explain which behavior you expected to see instead and why**
* **Include screenshots/photos if possible**
* **Include your Arduino board type and motor setup**
* **Include code snippets in markdown code blocks**

### 🎯 Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. Create an issue and provide:

* **Use a clear and descriptive title**
* **Provide a step-by-step description of the suggested enhancement**
* **Provide specific examples to demonstrate the steps**
* **Describe the current behavior and the new behavior**
* **Explain why this enhancement would be useful**

### 🔧 Pull Requests

* Fill in the required template
* Follow the C++ style guide (see below)
* Include appropriate test cases
* Document new functionality
* Update documentation and README as needed
* End all files with a newline

## Development Setup

### Prerequisites

- Arduino IDE installed
- Git version control
- CMake 3.10 or higher (for build system)
- A text editor or IDE (VSCode recommended)

### Local Development

1. **Fork the repository**
   ```bash
   # On GitHub, click "Fork" button
   ```

2. **Clone your fork**
   ```bash
   git clone https://github.com/YOUR-USERNAME/Arduino-Car-Library.git
   cd Arduino-Car-Library
   ```

3. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or for bug fixes:
   git checkout -b fix/issue-description
   ```

4. **Make your changes and test them**
   - Edit files in `src/` and `include/` directories
   - Test with Arduino IDE
   - Verify examples still work

5. **Commit with clear messages**
   ```bash
   git add your_files
   git commit -m "Add clear description of changes"
   ```

6. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Create Pull Request on GitHub**
   - Provide detailed description of changes
   - Reference any related issues
   - Include test results

## Style Guide

### C++ Code Style

We follow these conventions to keep code consistent:

#### Naming Conventions

```cpp
// Classes: PascalCase
class MotorController { };

// Methods: camelCase
void moveForward(int speed) { }

// Variables: snake_case for private members, camelCase for parameters
int _motor_speed;
void setSpeed(int desiredSpeed) { }

// Constants: UPPER_SNAKE_CASE
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
```

#### Indentation & Formatting

```cpp
// Use 4 spaces for indentation (NOT tabs)
// Max line length: 100 characters
// Opening braces on same line for functions

void Motor::forward(int speed)
{
    // 4-space indentation
    if (speed > 255) {
        speed = 255;
    }
    
    // Blank line between logical groups
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
}
```

#### Documentation Comments

Every public method must include detailed documentation:

```cpp
/**
 * Brief description (one line)
 * 
 * Detailed description explaining the method's purpose,
 * parameters, and return value.
 * 
 * @param speed     - Motor speed (0-255)
 * @param duration  - How long to maintain this speed (milliseconds)
 * 
 * @return true if successful, false if error occurred
 * 
 * Example:
 *   motor.forward(200);
 */
bool setMotorSpeed(int speed, int duration);
```

#### Comments in Code

```cpp
// Use // for inline comments
int value = getData();  // Store the result

// Use entire lines for block comments
//
// Complex algorithm explanation goes here
// Keep comments up to date with code
//

// TODO, FIXME, HACK, NOTE markers for special attention
// TODO: Implement encoder feedback for accurate distance
// FIXME: This breaks on Arduino Nano - test and fix
// HACK: Temporary workaround until library update
// NOTE: This value is calibrated for specific motor model
```

### Header File Organization

```cpp
#ifndef CLASS_NAME_H
#define CLASS_NAME_H

#include <Arduino.h>  // System includes first
#include <SomeLib.h>

// Then custom includes
#include "motor.h"

// Class/struct definitions
class MyClass {
public:
    // Public interface first
    MyClass();
    void publicMethod();

private:
    // Private implementation
    int _memberVariable;
    void _privateHelper();
};

#endif // CLASS_NAME_H
```

### Implementation File Organization

```cpp
// File: myclass.cpp

#include "myclass.h"

// Constructor
MyClass::MyClass()
{
    // Initialization
}

// Public methods
void MyClass::publicMethod()
{
    // Implementation
}

// Private helper methods
void MyClass::_privateHelper()
{
    // Implementation
}
```

## Testing

### Requirements for Contributions

- [ ] Code compiles without errors/warnings
- [ ] All existing examples still work
- [ ] New features include example code
- [ ] Documentation updated where applicable
- [ ] Comments added for complex logic
- [ ] Tested on actual  Arduino board (not just IDE simulation)

### Testing Your Changes

1. **Compile Check**
   ```bash
   # In Arduino IDE: Sketch → Verify/Compile
   ```

2. **Test on Hardware**
   - Upload to Arduino Uno (if possible)
   - Test relevant examples
   - Verify motors respond correctly

3. **Run Examples**
   ```cpp
   // BasicCarControl.ino
   // AdvancedNavigation.ino
   // Add new example if needed
   ```

## Documentation

### README Updates

If you add new features, update [README.md](/README.md):

- Add method to API Reference section
- Add usage example
- Update feature list if applicable

### Example Code Documentation

New examples should follow this template:

```cpp
/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * EXAMPLE NAME: ExampleName.ino
 * Arduino Car Library - Description
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * OVERVIEW: What does this example do?
 * 
 * PREREQUISITE HARDWARE: What components needed?
 * 
 * PIN CONFIGURATION: Exact pin wiring required
 * 
 * WHAT THIS SKETCH DOES: Detailed behavior description
 * 
 * OUTPUT: What serial output or behavior to expect
 * 
 * TROUBLESHOOTING: Common issues and solutions
 */
```

## Commit Message Guidelines

Write clear, descriptive commit messages:

```bash
# Good
git commit -m "Add motor encoder support for distance tracking"

# Bad
git commit -m "Fix stuff"
git commit -m "Update"
```

### Format

```
<type>: <subject>

<body>

<footer>
```

**Type:**
- `feat`: A new feature
- `fix`: A bug fix
- `docs`: Documentation changes
- `refactor`: Code refactoring without new features
- `perf`: Performance improvements
- `test`: Adding/updating tests

**Example:**
```bash
git commit -m "feat: Add PWM frequency adjustment for DC motors

- Implement setFrequency() method
- Add frequency parameter validation
- Update Motor class documentation
- Add example sketch showing frequency tuning

Resolves #42"
```

## Additional Notes

### Git Workflow

1. Always create a fresh branch for each feature/fix
2. Keep commits focused and atomic
3. Don't commit commented-out code or debug prints
4. Rebase before submitting PR to keep history clean

### For Maintainers

- Review all submissions for code quality
- Ensure backward compatibility
- Update version number if needed
- Test on multiple Arduino boards
- Merge only after approval

## Questions?

- Read existing documentation
- Check GitHub Issues for similar topics
- Ask in GitHub Discussions
- Open a new Issue asking for clarification

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for making Arduino Car Library better! 🚀**