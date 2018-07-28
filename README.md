# MrRobotStarterPack
Mr. Robot Badge Firmware Starter Pack.
# Where to Get a Badge
* We are only selling badges at the [Hackers on Planet Earth (HOPE) 2018](https://hope.net/) and [DEFCON 26](https://www.defcon.org/).
* Badges are sold via flash sales. Follow us on the [twitter](https://twitter.com/MrRobotBadge) or check the [Mr Robot Homepage](http://www.mrrobotbadge.tv/) for more info.
* If we have any badges left over they will go up on [Tindie](https://www.tindie.com/). Sometimes the badges end up on Ebay for 3-5x their retail price. If you buy two they'll pay for themselves.
* Check out what other people are doing with their badges by searching twitter for [#badgelife](https://twitter.com/search?q=%23badgelife&src=typd) or [#MrRobotBadge](https://twitter.com/hashtag/MrRobotBadge?src=hash)
* *Once the are gone. They are gone!* We're never doing another production run.
* After DefCon 26, after we've slept off our hang-overs we'll post the full hardware and software as open source hardware and software. Stay tuned. You should be able to clone the badge if you really want.


# Description of the Badge
# Arduino IDE Setup Instructions
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
# Software Components
# Image Tool Chain Setup
# Badge Flashing Instructions
  * Where to get a programmer.
  * How to program a badge.