set -o errexit
set -o nounset
set -o pipefail

(
sleep 1
dwlb -show HDMI-A-1
) &

run-status &

kanshi > /tmp/kanshi.log 2>&1 &

systemctl --user import-environment WAYLAND_DISPLAY XDG_CURRENT_DESKTOP
systemctl --user start graphical-session.target

swayidle -w \
  timeout 1000 'lock' \
  timeout 1201 'systemctl suspend' \
  before-sleep 'lock' \
  after-resume 'kanshi' &

