# Code Gen: From UDB to Implementations - Coding Challenge

![Python](https://img.shields.io/badge/Python-3.8%2B-blue?logo=python)  ![Status](https://img.shields.io/badge/Status-Completed-success)

##  Overview
This project is part of the RISC-V LFX Mentorship coding challenge.  
It provides a **bidirectional transformation tool** to convert RISC-V Unified Database (UDB) YAML instruction definitions into C header files and regenerate YAML from them, ensuring **lossless and deterministic** output.

---

##  Task Requirements
1. Read a YAML file from the [RISC-V Unified Database](https://github.com/riscv-software-src/riscv-unified-db).
2. Generate a C header (`.h`) file with equivalent data stored in `#define` macros.
3. Write a C program to read the generated header and output YAML.
4. Ensure that:
   - Regenerated YAML **matches exactly** the generated YAML (no extra fields, no comments, stable ordering).
   - Works for all valid YAML under `spec/std/isa/inst`.
   - All macro names are valid C identifiers.
   - Types (string, int, bool, list, dict) are preserved correctly.

---

## Dependencies & Setup
### GCC (any C compiler)
### **Python**
- Python 3.8
- PyYAML  
  Install with:
  ```bash
  pip install pyyaml

## Installation
1. Clone the repository:
   ```bash
   https://github.com/Mahnoor-ismail01/Yaml_to_C-Header.git
   cd Yaml_to_C-Header


3. Run Script
   ```
   python3 yamlToCHeader.py path/to/input.yaml output.h

   gcc HeaderToYaml.c -o headertoyaml
   ./headertoyaml output.h output.yaml
 4. Verify
```
python3 YamlToHeader.py output.yaml output1.h
./headertoyaml output1.h final.yaml
diff output.yaml final.yaml



