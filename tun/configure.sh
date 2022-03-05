#!/bin/sh

# client
ip tuntap add tun10 mode tun10
ip link tun10 set up
ip addr add 10.0.0.1/24 dev tun10

route add default gw 192.168.0.1 dev tun10

# server
ip tuntap add tun20 mode tun
ip link tun20 set up
ip addr add 10.0.0.2/24 dev tun78

route add default gw 192.168.0.2 dev tun20
iptables -t nat -A POSTROUTING -s 10.0.0.0/24 -o -j MASQUERADE