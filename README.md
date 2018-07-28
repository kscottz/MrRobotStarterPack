# MrRobotStarterPack
Mr. Robot Badge Firmware Starter Pack.
<p align="center">
  <img src="http://www.mrrobotbadge.tv/mask.png"></img>
</p>

[MR ROBOT](./img/mask.png)
# Where to Get a Badge
* We are only selling badges at the [Hackers on Planet Earth (HOPE) 2018](https://hope.net/) and [DEFCON 26](https://www.defcon.org/).
* Badges are sold via flash sales. Follow us on the [twitter](https://twitter.com/MrRobotBadge) or check the [Mr Robot Homepage](http://www.mrrobotbadge.tv/) for more info.
* If we have any badges left over they will go up on [Tindie](https://www.tindie.com/). Sometimes the badges end up on Ebay for 3-5x their retail price. If you buy two they'll pay for themselves.
* Check out what other people are doing with their badges by searching twitter for [#badgelife](https://twitter.com/search?q=%23badgelife&src=typd) or [#MrRobotBadge](https://twitter.com/hashtag/MrRobotBadge?src=hash)
* *Once the are gone. They are gone!* We're never doing another production run.
* After DefCon 26, after we've slept off our hang-overs we'll post the full hardware and software as open source hardware and software. Stay tuned. You should be able to clone the badge if you really want.


# Description of the Badge
# Development Environment Setup
## Arduino IDE Setup
* [Install a recent stock arduino IDE.](https://www.arduino.cc/en/Main/Software)
* Now we need to add ESP 8266 drivers to your IDE. To do this go Tools->Boards->BoardManager and a pop up should emerge.
  ![Board](./img/boards.png)
  In the pop up search for `ESP8266` and install the board.
  ![Set Board](./img/set_board.png)

* Once you have installed te hardware package for the ESP8266 we can now add the software dependencies to the arduino IDE. To do this hit Sketch->Include Library->Manage Libraries and a dialog should pop up.
  ![Add library](./img/addlib.png)
  * The first library we'll add is the Adafruit graphics library. We implement the Adafruit interface for rendering. In the dialog search for `Adafruit GFX` and install the library.
  ![GFX](./img/gfx.png)
  * The second library we need is [Brzo](https://github.com/pasko-zh/brzo_i2c). Brzo is a fast I2C (eye-squared-see) interface written in assembly for ESP8266. It is much better than the built-in one for Arduino. We use it because our frame buffer is 324 bytes and the Arduino I2C library only let's you move 32 bytes at a time. This lets us send images to display as one big chunk. To install it search for `Brzo` in the library dialog.
  ![brzo](./img/brzo.png)
* That should be it. You should now be able to build your own firmware.
## Image Tool Chain Setup
* The image toolchain is still in beta form. It was developed for linux systems and should work on OSX reasonably well. For Windows, reformat your hard drive and install linux.
* The image tool chain is a big hack, cobbled together using bash scripts, python, and the image magick command line utilitiy. What it does is looks through three directories `gifs`, `images`, and `scroll` and converts the images into byte arrays that can displayed on the badge. Once installed all you need to do is drop your images in the directory, run the script, and re-compile your firmware. The script will automagically read your images, process them, and dump them to header files with variables named to match the input image files.
* Assuming you have a system python intalled (preferably 3.x+) install the dependencies via pip. The deps are kinda heavy so good luck.
  ` sudo pip install -r './python/requirements.txt`
* If OpenCV gives you issue try installing it via apt using the following command. `sudo apt-get install python-opencv`. If you are using python 2.7 try using `python2.7-opencv`.
* The tool chain makes liberal use of [ImageMagick](https://www.imagemagick.org/script/index.php) which is a fantastic tool for image manipulation. To install it use the following command: `sudo apt-get install imagemagick`

## Installing
# Software Components
# Image Tool Chain Setup
# Badge Flashing Instructions
  * Where to get a programmer.
  * How to program a badge.