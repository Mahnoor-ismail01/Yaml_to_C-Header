/* Auto-generated from out.yaml.cleaned */
#ifndef CLI_H_H
#define CLI_H_H

#define CLI_H_H ""
#define _SCHEMA "inst_schema.json#"
#define KIND "instruction"
#define NAME "andn"
#define LONG_NAME "AND with inverted operand"
#define DESCRIPTION "Performs the bitwise logical AND operation between `xs1` and the\nbitwise inversion of `xs2`.\n"
#define DEFINEDBY_ANYOF_0 "Zbb"
#define DEFINEDBY_ANYOF_1 "Zbkb"
#define ASSEMBLY "xd, xs1, xs2"
#define FORMAT__INHERITS_0 "inst_subtype/R/R-x.yaml#/data"
#define FORMAT_OPCODES_FUNCT7_DISPLAY_NAME "ANDN"
#define FORMAT_OPCODES_FUNCT7_VALUE "32"
#define FORMAT_OPCODES_FUNCT3_DISPLAY_NAME "ANDN"
#define FORMAT_OPCODES_FUNCT3_VALUE "7"
#define FORMAT_OPCODES_OPCODE__INHERITS "inst_opcode/OP.yaml#/data"
#define ACCESS_S "always"
#define ACCESS_U "always"
#define ACCESS_VS "always"
#define ACCESS_VU "always"
#define DATA_INDEPENDENT_TIMING "1"
#define OPERATION__ "if (implemented?(ExtensionName::B) && (CSR[misa].B == 1'b0)) {\n  raise (ExceptionCode::IllegalInstruction, mode(), $encoding);\n}\nX[xd] = X[xs2] & ~X[xs1];\n"
#define SAIL__ "{\n  let rs1_val = X(rs1);\n  let rs2_val = X(rs2);\n  let result : xlenbits = match op {\n    RISCV_ANDN => rs1_val & ~(rs2_val),\n    RISCV_ORN  => rs1_val | ~(rs2_val),\n    RISCV_XNOR => ~(rs1_val ^ rs2_val),\n    RISCV_MAX  => to_bits(sizeof(xlen), max(signed(rs1_val),   signed(rs2_val))),\n    RISCV_MAXU => to_bits(sizeof(xlen), max(unsigned(rs1_val), unsigned(rs2_val))),\n    RISCV_MIN  => to_bits(sizeof(xlen), min(signed(rs1_val),   signed(rs2_val))),\n    RISCV_MINU => to_bits(sizeof(xlen), min(unsigned(rs1_val), unsigned(rs2_val))),\n    RISCV_ROL  => if sizeof(xlen) == 32\n                  then rs1_val <<< rs2_val[4..0]\n                  else rs1_val <<< rs2_val[5..0],\n    RISCV_ROR  => if sizeof(xlen) == 32\n                  then rs1_val >>> rs2_val[4..0]\n                  else rs1_val >>> rs2_val[5..0]\n  };\n  X(rd) = result;\n  RETIRE_SUCCESS\n}\n"

#endif /* CLI_H_H */
