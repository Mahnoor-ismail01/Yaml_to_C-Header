/* Auto-generated from out3.yaml.cleaned */
#ifndef ADD1_H
#define ADD1_H

#define _SCHEMA "inst_schema.json#"
#define KIND "instruction"
#define NAME "add"
#define LONG_NAME "Integer add"
#define DESCRIPTION "Add the value in xs1 to xs2, and store the result in xd.\nAny overflow is thrown away.\n"
#define DEFINEDBY "I"
#define ASSEMBLY "xd, xs1, xs2"
#define ENCODING_MATCH "0000000----------000-----0110011"
#define ENCODING_VARIABLES_0_NAME "xs2"
#define ENCODING_VARIABLES_0_LOCATION "24-20"
#define ENCODING_VARIABLES_1_NAME "xs1"
#define ENCODING_VARIABLES_1_LOCATION "19-15"
#define ENCODING_VARIABLES_2_NAME "xd"
#define ENCODING_VARIABLES_2_LOCATION "11-7"
#define ACCESS_S "always"
#define ACCESS_U "always"
#define ACCESS_VS "always"
#define ACCESS_VU "always"
#define DATA_INDEPENDENT_TIMING "1"
#define OPERATION__ "X[xd] = X[xs1] + X[xs2];"
#define SAIL__ "{\n  let xs1_val = X(xs1);\n  let xs2_val = X(xs2);\n  let result : xlenbits = match op {\n    RISCV_ADD  => xs1_val + xs2_val,\n    RISCV_SLT  => zero_extend(bool_to_bits(xs1_val <_s xs2_val)),\n    RISCV_SLTU => zero_extend(bool_to_bits(xs1_val <_u xs2_val)),\n    RISCV_AND  => xs1_val & xs2_val,\n    RISCV_OR   => xs1_val | xs2_val,\n    RISCV_XOR  => xs1_val ^ xs2_val,\n    RISCV_SLL  => if   sizeof(xlen) == 32\n                  then xs1_val << (xs2_val[4..0])\n                  else xs1_val << (xs2_val[5..0]),\n    RISCV_SRL  => if   sizeof(xlen) == 32\n                  then xs1_val >> (xs2_val[4..0])\n                  else xs1_val >> (xs2_val[5..0]),\n    RISCV_SUB  => xs1_val - xs2_val,\n    RISCV_SRA  => if   sizeof(xlen) == 32\n                  then shift_right_arith32(xs1_val, xs2_val[4..0])\n                  else shift_right_arith64(xs1_val, xs2_val[5..0])\n  };\n  X(xd) = result;\n  RETIRE_SUCCESS\n}\n"

#endif /* ADD1_H */
