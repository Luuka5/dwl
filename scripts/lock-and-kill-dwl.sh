set -o errexit
set -o nounset
set -o pipefail

lock

find_ancestor() {
    local target_name="$1"
    local current_pid=$$
    local proc_name=""

    while [ "$current_pid" -ne 1 ]; do
        proc_name=$(ps -p "$current_pid" -o comm=)
        if [ "$proc_name" = "$target_name" ]; then
            echo "$current_pid"
            return 0
        fi
        current_pid=$(ps -p "$current_pid" -o ppid= | tr -d ' ')
    done
    return 1
}

#DWL_PID=$(find_ancestor "dwl")
#kill "$DWL_PID"
kill "$(pidof "dwl")"
