
#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors!
. ~/.config/dwm/themes/onedark

# weathermodule () {
# 
# url="${WTTRURL:-wttr.in}"
# weatherreport="${XDG_CACHE_HOME:-$HOME/.cache}/weatherreport"
# 
# # Get a weather report from 'wttr.in' and save it locally.
# getforecast() { curl -sf "$url/Aberdeen" > "$weatherreport" || exit 1; }
# 
# # Forecast should be updated only once a day.
# checkforecast() {
	# [ -s "$weatherreport" ] && [ "$(stat -c %y "$weatherreport" 2>/dev/null |
		# cut -d' ' -f1)" = "$(date '+%Y-%m-%d')" ]
# }
# 
# getprecipchance() {
	# echo "$weatherdata" | sed '16q;d' |    # Extract line 16 from file
		# grep -wo "[0-9]*%" |           # Find a sequence of digits followed by '%'
		# sort -rn |                     # Sort in descending order
		# head -1q                       # Extract first line
# }
# 
# getdailyhighlow() {
	# echo "$weatherdata" | sed '13q;d' |      # Extract line 13 from file
		# grep -o "m\\([-+]\\)*[0-9]\\+" | # Find temperatures in the format "m<signed number>"
		# sed 's/[+m]//g' |                # Remove '+' and 'm'
		# sort -g |                        # Sort in ascending order
		# sed -e 1b -e '$!d'               # Extract the first and last lines
# }
# 
# readfile() { weatherdata="$(cat "$weatherreport")" ;}
# 
# showweather() {
	# readfile
	# printf "^c$blue^  %s ^c$white^ %s° ^c$orange^  %s°\n" "$(getprecipchance)" $(getdailyhighlow)
# }
# 
	# checkforecast || getforecast
	# 
	# showweather
# }

cpu() {

	cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
 	f=$(cat /sys/class/thermal/thermal_zone0/temp)
    t=$(echo $f | cut -b -2).$(echo $f | cut -b 5-)°C

	printf "^c$black^^b$green^ CPU "
	printf "^c$white^^b$grey^ $cpu_val "
	printf "^c$white^^b$grey^$t"
	
}

# disk() {
# 
	# printf "^c$pink^^b$black^ 󰋊 "
	# printf "$(df -h | awk 'NR==4{print $3,$5}')%"
# 
# }

pkg_updates() {

	updates=$(pacman -Qu | wc -l)
	
	if [ -z "$updates" ]; then
		printf "^b$black^^c$green^󰏔 "
	else
		printf "^b$black^^c$green^󰏔 $updates"
	fi
	
}

mem() {

  printf "^c$blue^^b$black^  "
  printf "^c$blue^$(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"

}

netstat() {
	
	update() {
	    sum=0
	    for arg; do
	        read -r i < "$arg"
	        sum=$(( sum + i ))
	    done
	    cache=${XDG_CACHE_HOME:-$HOME/.cache}/${1##*/}
	    [ -f "$cache" ] && read -r old < "$cache" || old=0
	    printf %d\\n "$sum" > "$cache"
	    printf %d\\n $(( sum - old ))
	}
	
	rx=$(update /sys/class/net/[ew]*/statistics/rx_bytes)
	tx=$(update /sys/class/net/[ew]*/statistics/tx_bytes)
	
	printf "^b$black^^c$white^ 󰬦 %4sB 󰬬 %4sB\\n" $(numfmt --to=iec $rx) $(numfmt --to=iec $tx)
}

wlan() {

if grep -xq 'up' /sys/class/net/w*/operstate 2>/dev/null ; then
	wifiicon="$(awk '/^\s*w/ { print "󰤥", int($3 * 100 / 70) "% " }' /proc/net/wireless)"
elif grep -xq 'down' /sys/class/net/w*/operstate 2>/dev/null ; then
	grep -xq '0x1003' /sys/class/net/w*/flags && wifiicon="󰤫 " || wifiicon="󰤮 "
fi

printf "%s%s%s\n" "^c$white^^b$black^ $wifiicon" "$(sed "s/down/󰈂/;s/up/󰈀/" /sys/class/net/e*/operstate 2>/dev/null)" "$(sed "s/.*//" /sys/class/net/tun*/operstate 2>/dev/null) "

}

wifi() {
	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
	up) printf "^c$black^^b$blue^ 󰤨 ^d^%s""^c$blue^Connected" ;;
	down) printf "^c$black^^b$blue^ 󰤭 ^d^%s ""^c$blue^Disconnected" ;;
	esac
}

clock() {
	printf "^c$black^^b$darkblue^ 󱑆 "
	printf "^c$black^^b$blue^ $(date '+%a %d %b %I:%M %p')"
}



volume() {
    # get the current audio level as a percentage
    vol=$(pulsemixer --get-volume | awk '{print $1}')
    mute=$(pulsemixer --get-mute | awk '{print $1}')

    if [ "$mute" = "1" ]; then
        icon="^c$red^"
        echo "$icon ""^c$white^$vol%"
    else
        case 1 in
            $((vol >= 70)) ) icon="^c$red^" ;;
            $((vol >= 30)) ) icon="^c$blue^󰕾" ;;
            $((vol >= 1)) ) icon="^c$blue^" ;;
            * ) echo 󰸈 && exit ;;
        esac
        echo "$icon ""$vol%"
    fi
}

while true; do

	[ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates) 

	# && weather=$(weathermodule)
	
 interval=$((interval + 1))

	sleep 1 && xsetroot -name "$updates $(volume) $(cpu) $(mem) $(netstat) $(wlan) $(clock) "
done

 
 


