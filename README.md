# OptiMap: Self-Optimizing Compiler for pPIM ISA  
**With Speculative Strategy Execution and AI-Guided Memory Mapping**

## Overview

**OptiMap** is a self-learning, AI-augmented compiler built for custom **24-bit Processor-in-Memory (pPIM) Instruction Set Architectures (ISAs)**. Unlike traditional compilers that follow static optimization rules, OptiMap employs **speculative strategy generation** and **AI-guided cost prediction** to select the most optimal code path for each workload.

It also incorporates **hardware-aware memory mapping**, making it highly suitable for edge AI accelerators and memory-centric compute systems where efficient row/bank alignment is critical. The system is designed to evolve over time using historical logs and machine learning, turning it into a long-term optimization tool.

---

## Project Highlights

- **Custom pPIM ISA Compiler**: Converts LLVM IR to 24-bit ISA for in-memory compute architectures.
- **Speculative Strategy Execution**: Explores multiple loop and memory layout variants for each matrix operation.
- **AI-Guided Cost Modeling**: Uses MLP-based prediction for selecting optimal execution plans.
- **Memory Mapping Optimization**: Translates abstract matrix access into hardware-aligned (Bank, Row, Offset) formats.
- **Self-Learning System**: Continuously improves performance through feedback and real-time updates.

---

## 📂 Repository Structure

```
pPIMCompiler/
 ├── data/                     # JSON memory store (compiler_memory.json, nss_memory.json)
 ├── include/                  # Header files (IR, strategy, NSS, cost model, etc.)
 ├── logs/                     # Debug logs (ISA traces)
 ├── src/                      # Source code for each module
 ├── .gitignore
 ├── CMakeLists.txt            # CMake configuration
 ├── matrix_mul.ll             # LLVM IR input file
 └── README.md
```

---

## ⚙️ Compilation Algorithm

The compilation process follows an adaptive pipeline:

### 🔹 1. Input and Preprocessing
- Compile C++ matrix code into LLVM IR
- Parse matrix operations and extract shape dimensions

### 🔹 2. Intermediate Representation (IR)
- Convert parsed data into pPIM-specific IR instructions
- Use IR as an abstraction layer between LLVM IR and ISA codegen

### 🔹 3. Strategy Generation
- Generate all valid permutations of loop orders, layouts, and tile sizes
- Filter semantically invalid strategies

### 🔹 4. Cost Estimation and Selection
- Lookup known strategies in `nss_memory.json`
- Use heuristic estimator for unseen strategies
- Predict cost via MLP model if history is unavailable
- Select the most efficient strategy

### 🔹 5. Memory Mapping
- Map logical matrix access to `(Bank, Row, Offset)`
- Ensure compatibility with row-parallel pPIM execution

### 🔹 6. Final Code Emission
- Encode IR into 24-bit binary ISA instructions
- Log instruction type, hex encoding, and physical address

### 🔹 7. Feedback and Learning
- Log real execution cost
- Update strategy history and AI model

### 🔹 8. Output
- Emit 24-bit instruction stream
- Save/update memory and logs

---

## Use Cases

- Hardware-software co-design for AI accelerators
- Simulation and testing of pPIM or memory-centric architectures
- Compiler innovation and AI-based optimization research
- Integration into embedded AI toolchains

---

## License

This project is licensed under the **MIT License**.

---

## Acknowledgements

This project uses the open-source [nlohmann/json](https://github.com/nlohmann/json) C++ library for JSON parsing and management.  
We thank **Niels Lohmann** for developing and maintaining this powerful and intuitive JSON library, which is integrated into OptiMap’s persistent memory subsystem.



---

## 🛠 How to Use

### Step 1: Install Dependencies (macOS)

```bash
xcode-select --install   # Install Xcode Command Line Tools
brew install cmake       # CMake build system
brew install git         # Version control
brew install llvm        # For LLVM-based IR parsing
```

### Step 2: Build and Run

From the project root directory:

```bash
cd build
cmake ..
make
./pPIMCompiler
```

This will compile the project and execute the main pipeline, including LLVM IR parsing, strategy selection, and 24-bit ISA code generation.

