from PIL import Image
import sys

def export_image_flat(input_path, var_name):
    img = Image.open(input_path).convert("RGBA")
    width, height = img.size
    pixels = list(img.get_flattened_data())
    output_path = f"fb_{input_path}.h"

    with open(output_path, "w") as f:
        f.write(f"int {var_name}_w = {width};\n")
        f.write(f"int {var_name}_h = {height};\n")
        f.write(f"unsigned char {var_name}_f[{width * height * 3}] = {{\n")

        for (r, g, b, a) in pixels:
            f.write(f"{r}, {g}, {b}, ")

        f.write("\n};\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python img2fb_h.py <input> <varname>")
        sys.exit(1)
    export_image_flat(sys.argv[1], sys.argv[2])