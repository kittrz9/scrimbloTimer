# scrimbloTimer
a simple speedrun timer for linux using X11 and SDL3<br>
<br>
currently the hotkeys are hard coded, and splits haven't been implemented (I'm not 100% sure if I will ever implement them)<br>
<br>
the font used is [Terminus](https://terminus-font.sourceforge.net/)

## building
to build you should just be able to run `./build.sh` in this directory.<br>
it should download a version of SDL3 and compile it, however cmake is required to build SDL3, so you will need to have that installed.<br>

## usage
when launched it will open a prompt to open a file containing a previous time you want to compare against, you can just cancel if you don't want to compare against a previous time or just dont have any previous times saved.<br>
pressing 1 on the numpad will start and stop the timer depending on whether it is currently started.<br>
pressing 2 on the numpad will reset the timer, which will have to be done once the timer has finished.<br>
currently remapping these keys requires you to modify the source code and change the X11 keysyms yourself. hopefully I'll implement a way to remap things in a config file eventually<br>
once you exit it will prompt you to save your current PB to a file, you can safely cancel this prompt too if you don't want to save it.<br>

