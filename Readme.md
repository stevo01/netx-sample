# netx sample application

The project demonstrates how to build a ThreadX and NetX application and run it on Linux. libpcap (pcap) is used for network communication.

## Install required packages

(tested with wsl2 / Ubuntu 22.04.5 LTS )

```
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libpcap-dev:i386
sudo apt-get install gcc-multilib g++-multilib libc6-dev:i386
```

## clone and build the sample application
```
git clone https://github.com/stevo01/netx-sample.git
git submodule update --init --recursive
```

## build application
```
rm -fr build
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/linux.cmake
cmake --build ./build
```

## Set capabilities
Note: the application needs to be run as the root user (or you can grant capabilities) because it requires
the capabilities cap_net_raw and cap_net_admin for network access.

Optional: you can set the required capabilities with the following commands:
```
sudo setcap cap_net_raw,cap_net_admin+eip ./build/sample-net-app 
```

## Build with Docker
A `Dockerfile` is provided in the `docker/` directory to provide the build environment (it does NOT build the app during image creation).

From the repository root, build the image:
```
docker build -f docker/Dockerfile -t netx-sample:latest .
```

Run the container and execute the project's build script `build.sh` (recommended: mount the repository into `/netx-sample` so build artifacts end up on the host):
```
# Build inside a container and write build output into host's ./build directory
docker run --rm -it -v "$(pwd):/netx-sample" --network host --cap-add NET_RAW --cap-add NET_ADMIN netx-sample:latest bash -lc "cd /netx-sample && chmod +x ./build.sh || true && ./build.sh"
```

Or open an interactive shell inside the container and run commands manually:
```
docker run --rm -it -v "$(pwd):/src" --network host --cap-add NET_RAW --cap-add NET_ADMIN netx-sample:latest
# then inside the container:
# cd /src && ./build.sh
```

Notes:
- `--network host` and adding capabilities are often required for pcap/raw socket access — avoid using these flags in untrusted environments.
- If you prefer not to run the build as root, adjust `build.sh` or the container user accordingly.

## debug with vscode

### helper script

create the script ~/gdbasroot.sh
and add the following content:

```
#!/bin/bash
exec sudo /usr/bin/gdb "$@"
```

make the script executable:
```
chmod +x  ~/gdbasroot.sh
```

### allow user to start gdb by command sudo without password 

Open the sudoers file by command 
'sudo visudo -f /etc/sudoers.d/gdb'
and add the following content

```
stevo ALL=(root) NOPASSWD: /usr/bin/gdb
```

## setup mosquitto message broker

### create config file

open file "/etc/mosquitto/mosquitto.conf" and add following content:

```
pid_file /run/mosquitto/mosquitto.pid
persistence true
persistence_location /var/lib/mosquitto/
log_dest file /var/log/mosquitto/mosquitto.log
include_dir /etc/mosquitto/conf.d
listener 1883 10.10.0.1
allow_anonymous true
```

### start message broker

```
mosquitto -c /etc/mosquitto/mosquitto.conf -v
```

### test mqtt message transfer

#### subscribe message

```
mosquitto_sub -h 10.10.0.1 -v -t /#
```

#### publish messages

```
mosquitto_pub -h 10.10.0.1 -t /device -m test
```
