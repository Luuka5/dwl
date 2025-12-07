
set +o errexit
set -o nounset
set +o pipefail

DEVICE="${DEVICE:-$(find /sys/class/backlight/ | head -1)}"

if [ -z "$DEVICE" ]; then
  echo "No brightness control available"
  exit 1
fi

STEP=5  # Change amount

get_brightness() {
    brightnessctl -d "$DEVICE" get 2>/dev/null || \
    cat "/sys/class/backlight/$DEVICE/brightness" 2>/dev/null
}

get_max_brightness() {
    brightnessctl -d "$DEVICE" max 2>/dev/null || \
    cat "/sys/class/backlight/$DEVICE/max_brightness" 2>/dev/null
}

# Calculate current percentage
CURRENT="$(get_brightness)"
MAX="$(get_max_brightness)"

if [ -z "$MAX" ]; then
  echo "Device has no max brightness. The device probably doesn't exist."
  exit 1
fi

CURRENT_PERCENT=$((CURRENT * 100 / MAX))

case "$1" in
    up)
        NEW_PERCENT=$((CURRENT_PERCENT + STEP))
        [ $NEW_PERCENT -gt 100 ] && NEW_PERCENT=100
        ;;
    down)
        NEW_PERCENT=$((CURRENT_PERCENT - STEP))
        [ $NEW_PERCENT -lt 1 ] && NEW_PERCENT=1
        ;;
    *)
        echo "Usage: $0 {up|down}"
        exit 1
        ;;
esac

# Set via systemd-logind
busctl call org.freedesktop.login1 \
    /org/freedesktop/login1/session/auto \
    org.freedesktop.login1.Session SetBrightness \
    ssu backlight "$DEVICE" "$NEW_PERCENT"

echo "Brightness: $NEW_PERCENT%"
