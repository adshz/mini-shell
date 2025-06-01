# Mini-Shell

A robust implementation of a Unix-like shell written in C, developed as a collaborative project. This shell demonstrates advanced system programming concepts and provides a feature-rich command-line interface.

## 🚀 Features

- **Interactive Command Line Interface**
  - Custom prompt with user and hostname display
  - Command history support using readline library
  - Advanced command parsing and execution

- **Built-in Commands**
  - `echo` - Display text and variables
  - `cd` - Change directory
  - `pwd` - Print working directory
  - `export` - Set environment variables
  - `unset` - Remove environment variables
  - `env` - Display environment variables
  - `exit` - Exit the shell

- **Advanced Shell Features**
  - Environment variable expansion
  - Signal handling (SIGINT, SIGQUIT)
  - Pipe implementation
  - Redirection support (`>`, `>>`, `<`, `<<`)
  - Command chaining with `&&` and `||`
  - Heredoc support (`<<`)

- **Robust Error Handling**
  - Comprehensive error messages
  - Graceful error recovery
  - Memory leak prevention

## 🏗️ Project Structure

The project is organized into several key components:

- `src/`
  - `builtins/` - Implementation of built-in shell commands
  - `core/` - Core shell functionality
  - `executor/` - Command execution logic
  - `expander/` - Environment variable expansion
  - `hashtable/` - Environment variable storage
  - `lexer/` - Command line tokenization
  - `parser/` - Command parsing and AST generation
  - `signals/` - Signal handling
  - `utils/` - Utility functions
  - `memory_collector/` - Memory management

- `inc/` - Header files
- `libft/` - Custom C library functions
- `test/` - Test suite

## 🛠️ Technical Challenges

This project presented numerous technical challenges that were successfully overcome:

1. **Complex Command Parsing**
   - Implementation of a robust lexer and parser
   - Handling of complex command chains and operators
   - Support for nested command structures

2. **Memory Management**
   - Custom memory tracking system
   - Prevention of memory leaks
   - Efficient resource cleanup

3. **Process Management**
   - Proper handling of child processes
   - Signal propagation
   - Process group management

4. **Environment Management**
   - Efficient environment variable storage using hashtables
   - Variable expansion and substitution
   - Proper environment inheritance

## 🏆 Project Success

This project represents a significant achievement in system programming, demonstrating:

- Advanced understanding of Unix/Linux system calls
- Complex software architecture and design
- Robust error handling and debugging
- Efficient memory management
- Clean and maintainable code structure

## 👥 Collaboration

This project was developed as a collaborative effort, requiring:
- Clear communication and coordination
- Code review and quality assurance
- Division of responsibilities
- Consistent coding standards

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Make
- Readline library

### Building
```bash
make
```

### Running
```bash
./minishell
```

## 📝 License

This project is part of the 42 school curriculum and follows its guidelines and standards.

## 🙏 Acknowledgments

Special thanks to my collaborator for their invaluable contributions to this project. The complexity and scope of this project made it a challenging but rewarding experience in system programming and software development.
