python img2scroll.py --input ./scroll --output scrolls.h
python img2bitmap.py --input ./images/ --output bitmaps.h
python gif2code.py --input ./gifs --output animations.h
mv *.h ../MrRobotStarterPack/
