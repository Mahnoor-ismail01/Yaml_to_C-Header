#ifndef OUTPUT_H
#define OUTPUT_H

// Instruction Data
#define INST_KIND "instruction"
#define INST_NAME "andn"
#define INST_LONG_NAME "AND with inverted operand"
#define INST_DESCRIPTION "Performs the bitwise logical AND operation between `xs1` and the\nbitwise inversion of `xs2`.\ndefinedBy:\nanyOf: [Zbb, Zbkb]\nassembly: xd, xs1, xs2\nformat:\n\$inherits:\n  - inst_subtype/R/R-x.yaml#/data\nopcodes:\n  funct7:\n  display_name: ANDN\n  value: 0b0100000\n  funct3:\n  display_name: ANDN\n  value: 0b111\n  opcode: \{ \$inherits: inst_opcode/OP.yaml#/data \}\naccess:\ns: always\nu: always\nvs: always\nvu: always\ndata_independent_timing: true\n"
#define INST_OPERATION__ "if (implemented?(ExtensionName::B) && (CSR[misa].B == 1'b0)) \{\n  raise (ExceptionCode::IllegalInstruction, mode(), \$encoding);\n\}\n\nX[xd] = X[xs2] & ~X[xs1];\n\n# SPDX-SnippetBegin\n# SPDX-FileCopyrightText: 2017-2025 Contributors to the RISCV Sail Model <https://github.com/riscv/sail-riscv/blob/master/LICENCE>\n# SPDX-License-Identifier: BSD-2-Clause\n"
#define INST_SAIL__ "\{\n  let rs1_val = X(rs1);\n  let rs2_val = X(rs2);\n  let result : xlenbits = match op \{\n  RISCV_ANDN => rs1_val & ~(rs2_val),\n  RISCV_ORN  => rs1_val | ~(rs2_val),\n  RISCV_XNOR => ~(rs1_val ^ rs2_val),\n  RISCV_MAX  => to_bits(sizeof(xlen), max(signed(rs1_val),   signed(rs2_val))),\n  RISCV_MAXU => to_bits(sizeof(xlen), max(unsigned(rs1_val), unsigned(rs2_val))),\n  RISCV_MIN  => to_bits(sizeof(xlen), min(signed(rs1_val),   signed(rs2_val))),\n  RISCV_MINU => to_bits(sizeof(xlen), min(unsigned(rs1_val), unsigned(rs2_val))),\n  RISCV_ROL  => if sizeof(xlen) == 32\n  then rs1_val <<< rs2_val[4..0]\n  else rs1_val <<< rs2_val[5..0],\n  RISCV_ROR  => if sizeof(xlen) == 32\n  then rs1_val >>> rs2_val[4..0]\n  else rs1_val >>> rs2_val[5..0]\n  \};\n  X(rd) = result;\n  RETIRE_SUCCESS\n\}\n\n# SPDX-SnippetEnd\n"

#endif
