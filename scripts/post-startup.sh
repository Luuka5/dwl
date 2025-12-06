set -o errexit
set -o nounset
set -o pipefail

kanshi  &
run-status &

(
sleep 1
dwlb -show HDMI-A-1 &
) &

swayidle -w \
  timeout 2000 'locksuspend' \
  after-resume 'kanshi' &

lock &
