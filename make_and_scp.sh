#!/bin/bash
echo "### Making executables ###"
make

echo "### Copying executables into slaves ###"
while read -u 3 node; do
	if [ "$node" != "192.168.1.0" ]; then
		echo "$node:"
		ssh pi@$node 'mkdir -p /home/pi/eratosthenes/bin'
		scp -r bin pi@$node:/home/pi/eratosthenes
	fi
done 3<machines
