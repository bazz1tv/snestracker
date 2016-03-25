export ST_DIR="$HOME/Code/SNES-Tracker"
export STD_DIR="$ST_DIR/PC_Tracker/APU.SDL.OLD.DONT.TOUCH/gme_vspc/std"
alias sti='cd "$ST_DIR" ; \
xfce4-terminal --tab --working-directory="$STD_DIR" ; \
xfce4-terminal --tab --working-directory="$HOME/.local/share/Black_Hole_Studios/SNES_Tracker" ; \
xfce4-terminal --tab --working-directory="$HOME/.wine/dosdevices/c:/users/bazz/Application Data/Black_Hole_Studios/SNES_Tracker"'

function mkcppmodule() {
	touch "`dirname $1`/`basename $1`."{cpp,h}
}