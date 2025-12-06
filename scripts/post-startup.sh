set -o errexit
set -o nounset
set -o pipefail

dwlb -show HDMI-A-1 &

run-status &
kanshi  &

swayidle -w \
  timeout 1000 'lock' \
  timeout 1201 'systemctl suspend' \
  before-sleep 'lock' \
  after-resume 'kanshi' &

