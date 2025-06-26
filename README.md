# Decaf Compiler

A full hybrid SSA-style compiler for the Decaf programming language, built entirely in C++. It compiles high-level Decaf code to optimized x86-64 assembly through multiple intermediate representations and optimization passes. The compiler includes a complete front-end, mid-end, and back-end pipeline.

## Features

- **Custom AST (DecafAST)**  
  Lexical and syntactic analysis powered by a maintainable Abstract Syntax Tree design.

- **Semantic Analysis**  
  Type checking, scope management, and symbol resolution using the Visitor pattern.

- **Intermediate Representations**  
  - High-Level IR: `DecafAST`  
  - Low-Level Linear IR: Simple, instruction-based IR suitable for direct code generation.

- **Control Flow Graph (CFG)**  
  - Converts Linear IR into basic blocks for analysis.

- **Optimization Pipeline** (iterative and modular)
  - Dead Code Elimination
  - Constant Folding
  - Copy Propagation
  - Common Subexpression Elimination

- **Register Allocation**  
  Based on graph coloring of interference graphs with spill cost heuristics and special handling of caller-saved registers for additional performance gain.

- **Assembly Code Generation**  
  Generates efficient x86-64 assembly with proper calling conventions, stack alignment, and runtime error checks.

## Performance

- Achieves **200% runtime speedup** over Clang and GCC compiled with `-O0` for x86-64 code.

## Usage

```bash
# Compile a Decaf source file to assembly
./run.sh -t assembly path/to/input.decaf -o out.s

# Assemble and link the code
gcc -O0 -no-pie out.s -o out

# Run the binary
./out
