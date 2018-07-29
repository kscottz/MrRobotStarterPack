import os
import cv2
import subprocess
from PIL import Image
import numpy as np
import shutil
import argparse
import glob

def get_stats(fname):
    frame = Image.open(fname)
    nframes = 0
    w = frame.width
    h = frame.height
    while frame:
        nframes += 1
        try:
            frame.seek( nframes )
        except EOFError:
            break

    return nframes, w, h


def make_animation(input_file, object_name, temp_dir="./tmp/", temp_file="tmp.png", resize=True):
    output_file = "strip.png"
    nframes, f_w, f_h = get_stats(input_file)
    filesToStrip  = ["montage","-mode","concatenate",temp_dir+"*.png","-tile",
                     "{0}x1".format(nframes), output_file]
    gif2files = ["convert", "-coalesce", input_file, temp_dir+temp_file]
    shrink = ["convert",output_file,"-geometry","x18", output_file]
    os.mkdir(temp_dir)
    #print " ".join(gif2files)
    subprocess.call(gif2files)
    #print " ".join(filesToStrip)
    subprocess.call(filesToStrip)
    if( resize ):
        subprocess.call(shrink)
    shutil.rmtree(temp_dir)
    #object_name = input_file.split(".")[0].upper()
    img = cv2.imread(output_file)
    h = img.shape[0]
    w = img.shape[1]
    out = "static uint8_t {0}[{1}] PROGMEM= {{\n".format(object_name,w*h)
    #print img.shape
    for y in range(0, h):
        for x in range(0, w):
            temp = "\t0x{:02X}, ".format(int(np.mean(img[y, x, :])))
            out += temp
        out += '\n'
    out = out[:-3]
    out += "};\n"
    object_str = "Animation {0}_animation({1},{2},{3},{4},{5},{6},false);\n".format(
        object_name,w/nframes,h,object_name,nframes,w,h)
    object_name = object_name + "_animation"
    out += object_str
    os.remove(output_file)
    return out, object_name

def make_animation_list(name,obj_names):
    out = ""
    out += "static uint16_t total_animations = {0};\n".format(len(obj_names))
    out += "static Animation* {0}[{1}] = {{\n".format(name,len(obj_names))
    for obj_name in obj_names:
        out += "\t&{0},\n".format(obj_name)
    out = out[:-2]
    out += "};\n\n"
    return out

def main(input_dir,output_file):
    print( "Ripping from {0} to {1}".format(input_dir,output_file))
    get_these = input_dir + "/*.gif"
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
            payload, obj_name = make_animation(os.path.abspath(f),name)
            with open(output_file,"a") as file:
                file.write(payload)
                file.write("\n\n")
            obj_names.append(obj_name)
            print("Added {0} as {1}".format(f,obj_name))
        except Exception as e:
            print(e)
            continue
    with open(output_file,"a") as file:
        file.write(make_animation_list("ANIMATION_LIST",obj_names))


    print("Done. Please include {0} in your code.".format(output_file))

if __name__ == "__main__":
    # to run:  python gif2code.py --input ./gifs --output animations.h
    parser = argparse.ArgumentParser(description="gif to code")
    parser.add_argument("--input", type=str, default="./gifs/",
                        help="The input directory from which to pull gifs")
    parser.add_argument("--output", type=str, default="animation.h",
                        help="The output header file.")
    args = parser.parse_args()
    main(args.input,args.output)
