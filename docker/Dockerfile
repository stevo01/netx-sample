# Dockerfile to build sample-net-app on Ubuntu 22.04.5 LTS
# Usage (from repo root):
#   docker build -t netx-sample:latest .
#   docker run --rm -it netx-sample:latest /src/build/sample-net-app

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install required packages (including i386 architecture packages)
RUN dpkg --add-architecture i386 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential cmake ninja-build git ca-certificates \
        libpcap-dev libpcap-dev:i386 gcc-multilib g++-multilib libc6-dev:i386 \
        pkg-config libcap2-bin \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

RUN mkdir -p /netx-sample
VOLUME ["/netx-sample"]

# Make build script executable if present
RUN if [ -f /src/build.sh ]; then chmod +x /src/build.sh || true; fi

# Default command: open an interactive shell so you can run the build script manually
CMD ["/bin/bash"]
