

if [ $# -lt 1 ];then
echo "usage: -bash [PID] "
exit 1
fi

process_id=$1
echo "process_id=$process_id"
sudo gdb -q --batch --ex "set height 0" -ex "thread apply all bt" -p $process_id
sleep 1

echo ==============================================
sudo strace -c -p $process_id


