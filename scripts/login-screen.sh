

# Run swaylock (blocks until unlocked)

dwl -s 'lock-and-kill-dwl' 

# Start user's actual session
exec dwl -s 'dwlb' 
