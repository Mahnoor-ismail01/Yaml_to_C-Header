import yaml

def yaml_to_header(yaml_file, header_file):
    # Load YAML file
    with open(yaml_file, 'r') as yf:
        data = yaml.safe_load(yf)

    # Open C header file for writing
    with open(header_file, 'w') as hf:
        hf.write("// Auto-generated from {}\n".format(yaml_file))
        hf.write("#ifndef CONFIG_H\n#define CONFIG_H\n\n")

        # Flatten and write as #define
        def write_defines(prefix, obj):
            if isinstance(obj, dict):
                for key, value in obj.items():
                    new_prefix = f"{prefix}_{key}".upper() if prefix else key.upper()
                    write_defines(new_prefix, value)
            elif isinstance(obj, list):
                for idx, value in enumerate(obj):
                    new_prefix = f"{prefix}_{idx}"
                    write_defines(new_prefix, value)
            else:
                hf.write(f"#define {prefix} {value}\n")

        write_defines("", data)

        hf.write("\n#endif // CONFIG_H\n")

if __name__ == "__main__":
    yaml_to_header("config.yaml", "config.h")
