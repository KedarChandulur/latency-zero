#!/bin/bash
# Bootstrap script — installs all heavy system dependencies for latency-zero
# Run once after cloning: bash scripts/bootstrap.sh

set -e  # exit immediately if any command fails

echo "Installing system dependencies..."

sudo apt update
sudo apt install -y \
    build-essential \
    gcc \
    g++ \
    clang \
    make \
    cmake \
    git \
#    libzmq3-dev \
#    libboost-all-dev \
    linux-tools-common \
    linux-tools-generic \
    linux-tools-$(uname -r)

echo "Done. All dependencies installed."
