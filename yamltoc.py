import yaml
import os
import sys
import re

def load_yaml_file(yaml_path):
    try:
        print(f"Debug: Attempting to open {yaml_path}")
        with open(yaml_path, 'r') as file:
            data = yaml.safe_load(file)
            if not data:
                print(f"Error: YAML file {yaml_path} is empty or invalid.")
                sys.exit(1)
            print(f"Debug: Loaded YAML data: {data}")
            return data
    except yaml.YAMLError as e:
        print(f"YAML parsing error in {yaml_path}: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading YAML file {yaml_path}: {e}")
        sys.exit(1)

def escape_c_string(s):
    if not isinstance(s, str):
        return s
    s = s.replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r').replace('$', '\\$')
    print(f"Debug: Escaped string: {s}")
    return s

def generate_c_header(yaml_data, output_path):
    try:
        print(f"Debug: Writing to {output_path}")
        with open(output_path, 'w') as f:
            header_name = os.path.basename(output_path).replace('.', '_').upper()
            f.write(f"#ifndef {header_name}\n")
            f.write(f"#define {header_name}\n\n")
            f.write("// Instruction Data\n")

            prefix = "INST"
            data_to_process = yaml_data
            if isinstance(yaml_data, dict) and 'instruction' in yaml_data and isinstance(yaml_data['instruction'], dict):
                print("Debug: Detected nested 'instruction' key, using INST_INSTRUCTION_ prefix")
                prefix = "INST_INSTRUCTION"
                data_to_process = yaml_data['instruction']
            else:
                print("Debug: Using INST_ prefix for top-level keys")

            for key, value in data_to_process.items():
                if key == '$schema':
                    print(f"Debug: Skipping key: {key}")
                    continue
                sanitized_key = re.sub(r'[^a-zA-Z0-9_]', '_', key).upper()
                if isinstance(value, (str, int, bool)):
                    if isinstance(value, str):
                        f.write(f'#define {prefix}_{sanitized_key} "{escape_c_string(value)}"\n')
                    elif isinstance(value, bool):
                        f.write(f'#define {prefix}_{sanitized_key} {int(value)}\n')
                    else:
                        f.write(f'#define {prefix}_{sanitized_key} {value}\n')
                    print(f"Debug: Wrote macro {prefix}_{sanitized_key}")
                elif isinstance(value, dict):
                    for subkey, subvalue in value.items():
                        sanitized_subkey = re.sub(r'[^a-zA-Z0-9_]', '_', subkey).upper()
                        if isinstance(subvalue, (str, int, bool)):
                            if isinstance(subvalue, str):
                                f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey} "{escape_c_string(subvalue)}"\n')
                            elif isinstance(subvalue, bool):
                                f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey} {int(subvalue)}\n')
                            else:
                                f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey} {subvalue}\n')
                            print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{sanitized_subkey}")
                        elif isinstance(subvalue, list):
                            for i, item in enumerate(subvalue):
                                if isinstance(item, dict):
                                    for item_key, item_value in item.items():
                                        sanitized_item_key = re.sub(r'[^a-zA-Z0-9_]', '_', item_key).upper()
                                        if isinstance(item_value, str):
                                            f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{i}_{sanitized_item_key} "{escape_c_string(item_value)}"\n')
                                        else:
                                            f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{i}_{sanitized_item_key} {item_value}\n')
                                        print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{sanitized_subkey}_{i}_{sanitized_item_key}")
            f.write("\n#endif\n")
        print(f"Generated C header file: {output_path}")
    except Exception as e:
        print(f"Error writing C header file {output_path}: {e}")
        sys.exit(1)

def main():
    yaml_file = "outputnew.yaml"
    header_path = "output.h"

    if not os.path.exists(yaml_file):
        print(f"YAML file {yaml_file} does not exist.")
        sys.exit(1)

    yaml_data = load_yaml_file(yaml_file)
    generate_c_header(yaml_data, header_path)

if __name__ == "__main__":
    main()