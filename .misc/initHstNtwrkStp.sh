#!/bin/bash
sudo tunctl -t gns3o
sudo ifconfig gns3o 10.200.200.1/30 up
sudo iptables -t nat -A POSTROUTING -o wlp2s0 -j MASQUERADE
sudo iptables -A FORWARD -i gns3o -j ACCEPT
sudo route add -net 10.200.200.2/31 gw 10.200.200.2 dev gns3o
sudo route add -net 192.168.2.0/24 gw 10.200.200.2 dev gns3o
sudo route add -net 172.1.100.0/24 gw 10.200.200.2 dev gns3o
