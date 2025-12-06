
swaylock \
  --image "/usr/share/wallpapers/srcery-locked.png" \
  --scaling center \
  --color 1c1b19 \
  --ring-color 00000000 \
  --ring-ver-color 519f50 \
  --ring-wrong-color ef2f27 \
  --ring-clear-color 2c78bf \
  --key-hl-color 98bc37 \
  --bs-hl-color f75341 \
  --inside-color 1c1b1900 \
  --inside-ver-color 519f5000 \
  --inside-wrong-color ef2f2700 \
  --inside-clear-color 2c78bf00 \
  --text-color 00000000 \
  --text-ver-color 00000000 \
  --text-wrong-color 00000000 \
  --text-clear-color 00000000 \
  --line-color 00000000 \
  --line-ver-color 00000000 \
  --line-wrong-color 00000000 \
  --line-clear-color 00000000 \
  --separator-color 00000000

find_ancestor() {
    local target_name="$1"
    local current_pid=$$

    while [ "$current_pid" -ne 1 ]; do
        local proc_name=$(ps -p "$current_pid" -o comm=)
        if [ "$proc_name" = "$target_name" ]; then
            echo "$current_pid"
            return 0
        fi
        current_pid=$(ps -p "$current_pid" -o ppid= | tr -d ' ')
    done
    return 1
}

DWL_PID=$(find_ancestor "dwl")

kill "$DWL_PID"
