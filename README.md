# scrimbloTimer
a simple speedrun timer for linux using X11 and SDL3<br>
<br>
currently splits haven't been implemented (I'm not 100% sure if I will implement them)<br>
<br>
the font used is [Terminus](https://terminus-font.sourceforge.net/)<br>
<br>
all file formats are not at all set in stone for now, so expect things to not be compatible with newer versions without manual editting<br>

## building
to build you should just be able to run `./build.sh` in this directory.<br>
<br>
it should download a version of SDL3 and compile it, however cmake is required to build SDL3, so you will need to have that installed.<br>
<br>

## usage
when launched it will open a prompt to open a file containing a previous time you want to compare against, you can just cancel if you don't want to compare against a previous time or just dont have any previous times saved.<br>
<br>
pressing 1 on the numpad will start and stop the timer depending on whether it is currently started.<br>
<br>
pressing 2 on the numpad will reset the timer, which will have to be done once the timer has finished.<br>
<br>
these hotkeys can be changed in the config file, described in the section below.<br>
<br>
once you exit it will prompt you to save your current PB to a file, you can safely cancel this prompt too if you don't want to save it.<br>
<br>

## configuration
to configure the hotkeys, you must have a file in your config folder (usually in `~/.config` unless you manually have changed it) named `scrimbloTimer.conf`. if it is not there, it will be automatically created once the timer has been launched.<br>
<br>
the config file generated will have some instructions on how to set up hotkeys, currently the only ones are to start/stop or reset the timer.<br>
