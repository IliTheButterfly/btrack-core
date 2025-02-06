import re
import sys

def parse_metadata(file_content):
    """Extract metadata entries from a C++ file."""
    metadata_pattern = re.compile(r'BEGIN_METADATA\(\s*(\w+),\s*"([^"]+)",\s*"([^"]*)",\s*([\w:<>]+),\s*([\w:<>]+)\s*\)', re.MULTILINE)
    results = []

    for match in metadata_pattern.finditer(file_content):
        class_name, friendly_name, description, node_class, meta_node_class = match.groups()
        results.append((class_name, friendly_name, description, node_class, meta_node_class))
    
    return results

def generate_registration_code(metadata_entries):
    """Generate C++ code for UI registration."""
    code = ["#include \"UIRegistry.h\""]
    code.append("namespace UI {")
    code.append("void registerNodes() {")

    for class_name, friendly_name, description, node_class, meta_node_class in metadata_entries:
        code.append(f'    UIRegistry::registerNode<{node_class}>(\"{friendly_name}\", \"{description}\");')
        code.append(f'    UIRegistry::registerMetaNode<{meta_node_class}>(\"{friendly_name}\", \"{description}\");')

    code.append("}")
    code.append("} // namespace UI")

    return "\n".join(code)

def main():
    if len(sys.argv) < 3:
        print("Usage: python parse_nodes.py <input_file> <output_file>")
        return

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    with open(input_file, "r", encoding="utf-8") as f:
        content = f.read()

    metadata_entries = parse_metadata(content)
    generated_code = generate_registration_code(metadata_entries)

    with open(output_file, "w", encoding="utf-8") as f:
        f.write(generated_code)

if __name__ == "__main__":
    main()
