
# Start a minimal Sway session for swaylock
dwl &
DWL_PID=$!

# Wait for Sway to start
sleep 1

# Run swaylock (blocks until unlocked)
lock

# After unlock, kill the greeter session
kill $DWL_PID

# Start user's actual session
exec dwl -s 'dwlb' 
