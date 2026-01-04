#!/usr/bin/env bash
# veth-netx.sh

# set -x

VETH_NETX="veth_netx"
VETH_LINUX="veth_netx_peer"
LINUX_IP="10.10.0.1/24"

veth_up() {
    echo "[+] create veth pair"

    # cleanup if exists
    ip link del ${VETH_NETX} 2>/dev/null || true

    # create veth pair
    ip link add ${VETH_NETX} type veth peer name ${VETH_LINUX}

    # bring up interfaces
    ip link set ${VETH_NETX} up
    ip link set ${VETH_LINUX} up

    # assign IP to linux side (broker side)
    ip addr add ${LINUX_IP} dev ${VETH_LINUX}

    # disable tx offloading (pcap friendly)
    ethtool --offload ${VETH_LINUX} tx off 2>/dev/null || true

    echo "[+] veth ready:"
    ip addr show ${VETH_LINUX}
}

veth_down() {
    echo "[-] remove veth pair"
    ip link del ${VETH_NETX} 2>/dev/null || true
}

case "$1" in
    up)
        veth_up
        ;;
    down)
        veth_down
        ;;
    *)
        echo "usage: $0 {up|down}"
        exit 1
        ;;
esac
