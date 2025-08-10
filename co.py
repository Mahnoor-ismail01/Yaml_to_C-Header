#!/usr/bin/env python3
"""
yaml_to_header.py

Usage:
  python3 yaml_to_header.py input.yaml output.h

Produces:
 - input.yaml.cleaned  (cleaned YAML, deterministic order, no comments)
 - output.h            (C header with #define macros; all values C-escaped)
"""
import sys
import re
import json
from collections import OrderedDict
import yaml

# Preserve mapping order when loading YAML
def ordered_yaml_load(stream):
    class OrderedLoader(yaml.SafeLoader):
        pass
    def construct_mapping(loader, node):
        loader.flatten_mapping(node)
        return OrderedDict(loader.construct_pairs(node))
    OrderedLoader.add_constructor(
        yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG,
        construct_mapping)
    return yaml.load(stream, OrderedLoader)

def sanitize_macro_name(name):
    s = re.sub(r'[^0-9A-Za-z_]', '_', name)
    if re.match(r'^[0-9]', s):
        s = '_' + s
    return s.upper()

def flatten(obj, parent=None):
    """
    Flatten nested dicts/lists to (macro_name, value) pairs.
    """
    items = []
    if isinstance(obj, dict):
        for k, v in obj.items():
            pname = f"{parent}_{k}" if parent else k
            items.extend(flatten(v, pname))
    elif isinstance(obj, list):
        for idx, v in enumerate(obj):
            pname = f"{parent}_{idx}"
            items.extend(flatten(v, pname))
    else:
        macro = sanitize_macro_name(parent)
        items.append((macro, obj))
    return items

def c_escape_value(v):
    if isinstance(v, bool):
        return "1" if v else "0"
    if v is None:
        return None
    if isinstance(v, (int, float)):
        return str(v)
    if isinstance(v, (list, dict, OrderedDict)):
        j = json.dumps(v, separators=(',', ':'), ensure_ascii=False)
        j = j.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')
        return f"\"{j}\""
    s = str(v)
    if s == "":
        return None
    s = s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r')
    return f"\"{s}\""

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 yaml_to_header.py input.yaml output.h")
        sys.exit(1)

    in_yaml = sys.argv[1]
    out_h = sys.argv[2]

    with open(in_yaml, 'r', encoding='utf-8') as f:
        data = ordered_yaml_load(f)

    cleaned_path = in_yaml + ".cleaned"
    with open(cleaned_path, 'w', encoding='utf-8') as f:
        yaml.dump(data, f, sort_keys=False, allow_unicode=True)

    pairs = flatten(data)
    header_guard = re.sub(r'[^0-9A-Za-z]', '_', out_h).upper()

    with open(out_h, 'w', encoding='utf-8', newline='\n') as f:
        f.write(f"/* Auto-generated from {in_yaml}.cleaned */\n")
        f.write(f"#ifndef {header_guard}\n#define {header_guard}\n\n")
        for macro, val in pairs:
            # Skip macros that match the header guard or have no usable value
            if macro == header_guard or val is None or val == "":
                continue
            lit = c_escape_value(val)
            if lit is not None:
                f.write(f"#define {macro} {lit}\n")
        f.write(f"\n#endif /* {header_guard} */\n")

    print(f"Wrote cleaned YAML -> {cleaned_path}")
    print(f"Wrote header -> {out_h}")

if __name__ == "__main__":
    main()

