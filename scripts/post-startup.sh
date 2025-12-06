set -o errexit
set -o nounset
set -o pipefail

kanshi  &
lock &

run-status &

(
sleep 1
dwlb -show HDMI-A-1 &
) &

swayidle -w \
  timeout 1000 'lock' \
  timeout 1201 'systemctl suspend' \
  before-sleep 'lock' \
  after-resume 'kanshi' &

