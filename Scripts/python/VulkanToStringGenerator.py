import re
import os
import pyperclip

VULKAN_SDK = os.getenv('VULKAN_SDK')

if not VULKAN_SDK:
    raise EnvironmentError('VULKAN_SDK environment variable is not set.')

# Path to the Vulkan header file
VULKAN_HEADER_PATH = os.path.join(VULKAN_SDK, 'include', 'vulkan', 'vulkan_core.h')

# Regular expression to capture Vulkan enum values
VULKAN_ENUM_REGEX = r'VK_([A-Za-z0-9_]+)'

def extract_vk_enum_values(header_file_path, enum_prefix):
    """
    Extract Vulkan enum values based on the given prefix (e.g., VK_FORMAT_ or VK_RESULT_).
    """
    with open(header_file_path, 'r') as f:
        content = f.read()

    # Find all enum values that match the prefix (e.g., VK_FORMAT_)
    pattern = rf'{enum_prefix}([A-Za-z0-9_]+)'
    enum_values = re.findall(pattern, content)
    return enum_values

def generate_to_string_function(enum_prefix, enum_values, pascal_enum):
    """
    Generate a C++ ToString function for a Vulkan enum type.
    """
    # Start the function definition
    to_string_function = f'''
std::string ToString({pascal_enum} value) {{
    switch (value) {{
'''

    # Add a case for each enum value
    for value in enum_values:
        to_string_function += f'        case {enum_prefix}{value}: return "{convert_to_text(value.replace("_KHR", "").replace("_EXT", ""))}";\n'

    # Close the switch statement
    to_string_function += "        default: return \"Unknown " + pascal_enum + "\";\n    }\n}"
    return to_string_function

def save_to_file(pascal_enum, content):
    """
    Save the generated ToString function to a file.
    """
    # Create the directory if it doesn't exist
    output_dir = 'generated'
    os.makedirs(output_dir, exist_ok=True)

    # Define the file path
    file_path = os.path.join(output_dir, f'{pascal_enum}.hpp')

    # Write the content to the file
    with open(file_path, 'w') as f:
        f.write(content)

    print(f'File saved to: {file_path}')

def copy_to_clipboard(content):
    """
    Copy the generated content to the clipboard.
    """
    pyperclip.copy(content)
    print('Content copied to clipboard!')

def convert_to_pascal_case(enum_prefix):
    """
    Convert a Vulkan-style enum prefix (e.g., VK_FORMAT) to PascalCase (e.g., VkFormat).
    """
    # Convert to lowercase and split by underscores
    words = enum_prefix.lower().split('_')
    
    # Capitalize each word and join them
    return ''.join(word.capitalize() for word in words)

def convert_to_text(string):
    """
    Convert a Vulkan-style enum prefix (e.g., COLOR_SPACE) to text (e.g., Color Space).
    """

    words = string.lower().split('_')
    text = ' '.join(word.capitalize() for word in words)

    # capitalize certain phrases

    # List of patterns to match (r4, g4, b4, a4, ..., r32, g32, b32, a32)
    patterns = [f"{color}{i}" for color in ['r', 'g', 'b', 'a'] for i in range(64)]
    patterns.append("Astc")
    patterns.append("Srgb")
    patterns.append("Pvrtc")
    patterns.append("bpp")
    patterns.append("Amd")
    patterns.append("Dci")
    patterns.append("HdR")
    patterns.append("Hdr")
    patterns.append("Bt")
    patterns.append("St")
    patterns.append("Hlg")
    patterns.append("Fifo")
    patterns.append("Gpu")
    patterns.append("Cpu")

    replacements = {
        "Uint": "UInt",
        "Sint": "SInt",
        "Ufloat": "UFloat",
        "Sfloat": "SFloat",
        "Uscaled": "UScaled",
        "Sscaled": "SScaled",
        "Unorm": "UNorm",
        "Snorm": "SNorm",
        "Bc": "BC",
        "plane": "Plane",
        "pack": "Pack",
        " Img": " Image" ,
        "Adobergb": "AdobeRGB",
        "Dolbyvision": "DolbyVision"
    }

    for pattern in patterns:
        # Replace the pattern with its capitalized version
        text = re.sub(rf"{pattern}", pattern.upper(), text)

    for old, new in replacements.items():
        text = text.replace(old, new)

    return text

def main():
    # Example enum prefix (could be VK_FORMAT, etc.)
    enum_prefix = input('Enter Vulkan enum prefix (e.g., VK_FORMAT for VkFormat): ')

    pascal_enum = convert_to_pascal_case(enum_prefix)

    # Extract enum values for the given prefix
    enum_values = extract_vk_enum_values(VULKAN_HEADER_PATH, enum_prefix + "_")

    if not enum_values:
        print(f'No enum values found for prefix: {enum_prefix}')
        return

    # Generate the ToString function with the extracted values
    to_string_function = generate_to_string_function(enum_prefix + "_", enum_values, pascal_enum)

    # Save the content to a file
    save_to_file(pascal_enum, to_string_function)

    # Ask if the user wants to copy the output to clipboard
    copy = input('Do you want to copy the content to clipboard? (y/n): ').strip().lower()
    if copy == 'y':
        copy_to_clipboard(to_string_function)

if __name__ == '__main__':
    main()
