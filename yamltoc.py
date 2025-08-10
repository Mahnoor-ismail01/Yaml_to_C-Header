import yaml
import os
import sys
import re

def preprocess_yaml_content(content):
    """Preprocess YAML content to normalize indentation and fix formatting issues."""
    lines = content.splitlines()
    processed_lines = []
    in_literal_block = False
    indent_level = 0

    for line in lines:
        stripped_line = line.rstrip()  # Remove trailing whitespace
        if not stripped_line and not in_literal_block:
            continue  # Skip empty lines outside literal blocks

        # Detect start of literal block
        if stripped_line.endswith("|"):
            in_literal_block = True
            indent_level = len(line) - len(line.lstrip()) + 2  # Base indent + 2 spaces
            processed_lines.append(stripped_line)
            continue

        if in_literal_block:
            # Handle literal block content
            if stripped_line.strip():  # Non-empty line
                current_indent = len(line) - len(line.lstrip())
                indent_diff = indent_level - current_indent
                if indent_diff > 0:
                    processed_lines.append(" " * indent_level + stripped_line.lstrip())
                elif indent_diff < 0:
                    processed_lines.append(" " * indent_level + stripped_line[-indent_diff:])
                else:
                    processed_lines.append(stripped_line)
            else:
                processed_lines.append(" " * indent_level)  # Indent empty lines
        else:
            processed_lines.append(stripped_line)

        # Detect end of literal block
        if in_literal_block and stripped_line.strip() and len(line) - len(line.lstrip()) < indent_level - 2:
            in_literal_block = False

    # Ensure single trailing newline
    result = "\n".join(processed_lines).rstrip() + "\n"
    print(f"Debug: Preprocessed YAML lines:\n{result}")
    return result

def load_yaml_file(yaml_path):
    try:
        print(f"Debug: Attempting to open {yaml_path}")
        with open(yaml_path, 'r') as file:
            raw_content = file.read()
            print(f"Debug: Raw YAML content:\n{raw_content}\n")
            processed_content = preprocess_yaml_content(raw_content)
            data = yaml.safe_load(processed_content)
            if not data:
                print(f"Error: YAML file {yaml_path} is empty or invalid.")
                sys.exit(1)
            # Validate expected fields
            expected_fields = {'kind', 'name', 'long_name', 'description', 'definedBy', 'assembly', 'format', 'access', 'data_independent_timing', 'operation()', 'sail()'}
            actual_fields = set(data.keys())
            missing_fields = expected_fields - actual_fields
            if missing_fields:
                print(f"Warning: Missing expected fields in YAML: {missing_fields}")
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
        return str(s)  # Convert non-strings to strings
    s = s.replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r').replace('$', '\\$').replace('{', '\\{').replace('}', '\\}')
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
                        elif isinstance(subvalue, dict):
                            for subsubkey, subsubvalue in subvalue.items():
                                sanitized_subsubkey = re.sub(r'[^a-zA-Z0-9_]', '_', subsubkey).upper()
                                if isinstance(subsubvalue, (str, int, bool)):
                                    if isinstance(subsubvalue, str):
                                        f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey} "{escape_c_string(subsubvalue)}"\n')
                                    elif isinstance(subsubvalue, bool):
                                        f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey} {int(subsubvalue)}\n')
                                    else:
                                        f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey} {subsubvalue}\n')
                                    print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey}")
                                elif isinstance(subsubvalue, dict):
                                    for subsubsubkey, subsubsubvalue in subsubvalue.items():
                                        sanitized_subsubsubkey = re.sub(r'[^a-zA-Z0-9_]', '_', subsubsubkey).upper()
                                        if isinstance(subsubsubvalue, str):
                                            f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey}_{sanitized_subsubsubkey} "{escape_c_string(subsubsubvalue)}"\n')
                                        else:
                                            f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey}_{sanitized_subsubsubkey} {subsubsubvalue}\n')
                                        print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{sanitized_subkey}_{sanitized_subsubkey}_{sanitized_subsubsubkey}")
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
                                else:
                                    f.write(f'#define {prefix}_{sanitized_key}_{sanitized_subkey}_{i} "{escape_c_string(str(item))}"\n')
                                    print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{sanitized_subkey}_{i}")
                elif isinstance(value, list):
                    for i, item in enumerate(value):
                        if isinstance(item, str):
                            f.write(f'#define {prefix}_{sanitized_key}_{i} "{escape_c_string(item)}"\n')
                            print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{i}")
                        elif isinstance(item, dict):
                            for item_key, item_value in item.items():
                                sanitized_item_key = re.sub(r'[^a-zA-Z0-9_]', '_', item_key).upper()
                                if isinstance(item_value, str):
                                    f.write(f'#define {prefix}_{sanitized_key}_{i}_{sanitized_item_key} "{escape_c_string(item_value)}"\n')
                                else:
                                    f.write(f'#define {prefix}_{sanitized_key}_{i}_{sanitized_item_key} {item_value}\n')
                                print(f"Debug: Wrote macro {prefix}_{sanitized_key}_{i}_{sanitized_item_key}")
            f.write("\n#endif\n")
        print(f"Generated C header file: {output_path}")
    except Exception as e:
        print(f"Error writing C header file {output_path}: {e}")
        sys.exit(1)

def main():
    yaml_file = "outp.yaml"  # Use the generated YAML file
    header_path = "output.h"

    if not os.path.exists(yaml_file):
        print(f"YAML file {yaml_file} does not exist.")
        sys.exit(1)

    yaml_data = load_yaml_file(yaml_file)
    generate_c_header(yaml_data, header_path)

if __name__ == "__main__":
    main()