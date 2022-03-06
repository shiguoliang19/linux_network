#!/bin/sh

clean(){

    echo "clean tun"

    echo "============================================================"

    ip tuntap del tun10 mode tun
    ip tuntap del tun20 mode tun
    killall -9 tun

    echo "============================================================"

}

runClient(){

    echo "run client"

    echo "============================================================"

    ip tuntap add tun10 mode tun
    ip link set tun10 up
    ip addr add 10.0.0.1/24 dev tun10

    # route add default gw 10.0.0.1 dev tun10
    # route add 10.0.0.1 gw 192.168.0.115 dev ens33

    ./build/tun -c tun10 127.0.0.1 8080

    echo "============================================================"

}

runServer(){
    echo "run client"

    echo "============================================================"

    ip tuntap add tun20 mode tun
    ip link set tun20 up
    ip addr add 10.0.0.2/24 dev tun20

    route add default gw 192.168.0.2 dev tun20
    iptables -t nat -A POSTROUTING -s 10.0.0.0/24 -o -j MASQUERADE

    ./build/tun -s tun10 0.0.0.0 8080

    echo "============================================================"

}

if [ $1 == "-c" ]; then

    clean

    runClient

fi

if [ $1 == "-s" ]; then

    clean

    runServer

fi

if [ $1 == "-d" ]; then

    clean

fi
