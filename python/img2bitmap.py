import os
import cv2
import subprocess
import numpy as np
import shutil
import argparse
import glob


def make_bitmap(input_file, object_name, temp_dir="./tmp/", temp_file="tmp.png", resize=True):
    img = cv2.imread(input_file);
    h,w,d = img.shape
    if h > 18 or w > 18:
        output_file = "strip.png"
        shrink = ["convert",input_file,"-geometry","18x18", output_file]
        print shrink
        subprocess.call(shrink)
    else:
        output_file = input_file

    img = cv2.imread(output_file)
    h = img.shape[0]
    w = img.shape[1]
    out = "static uint8_t {0}[{1}] PROGMEM = {{\n".format(object_name,w*h)
    for y in range(0, h):
        for x in range(0, w):
            temp = "\t0x{:02X}, ".format(int(np.mean(img[y, x, :])))
            out += temp
        out += '\n'
    out = out[:-3]
    out += "};\n"
    object_str = "Bitmap {0}_bitmap({1},{2},{3},false);\n".format(
        object_name,w,h,object_name)
    object_name = object_name + "_bitmap"
    out += object_str
    return out, object_name


def make_bitmap_list(name,obj_names):
    out = ""
    out += "static uint16_t total_bitmaps = {0};\n".format(len(obj_names))
    out += "static Bitmap* {0}[{1}] = {{\n".format(name,len(obj_names))
    for obj_name in obj_names:
        out += "\t&{0},\n".format(obj_name)
    out = out[:-2]
    out += "};\n\n"
    return out

def main(input_dir,output_file):
    print( "Ripping from {0} to {1}".format(input_dir,output_file))
    get_these = input_dir + "/*.png"
    print get_these
    files = glob.glob(get_these)
    files = sorted(files)
    print(files)

    with open(output_file,"w") as file:
        file.write("#include \"RobotGFX.h\" \n")

    obj_names = []
    for f in files:
        try:
            name = f.split(".")[-2]
            name = name.split("/")[-1].upper()
            payload, obj_name = make_bitmap(os.path.abspath(f),name)
            with open(output_file,"a") as file:
                file.write(payload)
                file.write("\n\n")
            obj_names.append(obj_name)
            print("Added {0} as {1}".format(f,obj_name))
        except Exception as e:
                print(e)
                continue
    with open(output_file,"a") as file:
        file.write(make_bitmap_list("BITMAP_LIST",obj_names))


    print("Done. Please include {0} in your code.".format(output_file))

if __name__ == "__main__":
    # to run:  python img2bitmap.py --input ./images/ --output bitmaps.h
    parser = argparse.ArgumentParser(description="gif to code")
    parser.add_argument("--input", type=str, default="./images/",
                        help="The input directory from which to pull pngs")
    parser.add_argument("--output", type=str, default="bitmaps.h",
                        help="The output header file.")
    args = parser.parse_args()
    main(args.input,args.output)
