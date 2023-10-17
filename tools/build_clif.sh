#!/bin/bash
# Copyright 2020 Google LLC.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#

set -eux -o pipefail

echo ========== This script has been tested on Ubuntu18.04 and Ubuntu20.04.
echo ========== See https://github.com/google/clif for how to build on different Unix distributions.
echo ========== Run this script in root mode.

CLIF_UBUNTU_VERSION="${CLIF_UBUNTU_VERSION-20.04}"
ABSL_PIN="${ABSL_PIN-29bf8085f3bf17b84d30e34b3d7ff8248fda404e}"
PROTOBUF_VERSION=3.13.0
CLIF_PYTHON_VERSION="${CLIF_PYTHON_VERSION-3.8}"
# CLIF_PIN can be set to a specific commit hash on
# https://github.com/google/clif/commits/main.
# If not set, the default is to checkout the latest commit.
CLIF_PIN="${CLIF_PIN-9ec44bde4f7f40de342a1286f84f5b608633a2d7}"

APT_ARGS=(
"-y"
)


apt-get update  "${APT_ARGS[@]}"
apt-get install "${APT_ARGS[@]}" --no-install-recommends \
    autoconf \
    automake \
    cmake \
    curl \
    gpg-agent \
    g++ \
    libtool \
    make \
    pkg-config \
    software-properties-common \
    wget \
    unzip

# Configure LLVM 11 apt repository
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key |  apt-key add - && \
  add-apt-repository "deb http://apt.llvm.org/$(lsb_release -sc)/ llvm-toolchain-$(lsb_release -sc)-11 main"

# Install CLIF dependencies
apt-get update "${APT_ARGS[@]}"
apt-get install  "${APT_ARGS[@]}" \
    clang-11 \
    libclang-11-dev \
    libgoogle-glog-dev \
    libgtest-dev \
    libllvm11 \
    llvm-11 \
    llvm-11-dev \
    llvm-11-linker-tools \
    python3-dev \
    zlib1g-dev

# Uninstall an older version of libclang so that cmake uses the correct one.
apt-get remove "${APT_ARGS[@]}" libclang-common-9-dev

# Configure deadsnakes PPA with the more recent versions of python packaged for
# Ubuntu. See https://launchpad.net/~deadsnakes/+archive/ubuntu/ppa
apt-get update "${APT_ARGS[@]}" && \
  apt-get install "${APT_ARGS[@]}" \
    "python$CLIF_PYTHON_VERSION-dev" \
    "python$CLIF_PYTHON_VERSION-distutils"

# Install latest version of pip since the version on ubuntu could be outdated.
# Uninstall setuptools before installation of pip according to this suggestion:
# https://github.com/google/deepvariant/commit/5469fec47e57febba751c30f1eb587cbffea8d89#commitcomment-64556350
curl -Ss -o get-pip.py https://bootstrap.pypa.io/get-pip.py && \
    pip uninstall -y setuptools &&
    "python$CLIF_PYTHON_VERSION" get-pip.py --force-reinstall && \
    rm get-pip.py

# Compile and install absl-cpp from source
git clone https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
if [[ ! -z ${ABSL_PIN} ]]; then
  git checkout "${ABSL_PIN}"
fi
mkdir build && cd build
cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=true
make install
cd ../..
rm -rf abseil-cpp

# Compile and install protobuf from source
wget "https://github.com/protocolbuffers/protobuf/releases/download/v$PROTOBUF_VERSION/protobuf-cpp-$PROTOBUF_VERSION.tar.gz" && \
    tar -xf "protobuf-cpp-$PROTOBUF_VERSION.tar.gz" && \
    cd "protobuf-$PROTOBUF_VERSION" && \
    # Configure and install C++ libraries
    ./autogen.sh && \
    ./configure && \
    make -j"$(nproc)" && \
    make install && \
    ldconfig && \
    rm -rf "/protobuf-$PROTOBUF_VERSION" "/protobuf-cpp-$PROTOBUF_VERSION.tar.gz"

# Install googletest
cd /usr/src/googletest && \
    cmake . && \
    make install

# Install python runtime and test dependencies
"python$CLIF_PYTHON_VERSION" -m pip install \
    absl-py \
    parameterized \
    protobuf=="$PROTOBUF_VERSION" \
    pyparsing==2.2.0

DV_PLATFORM="ubuntu-${CLIF_UBUNTU_VERSION}"

ln -sf /usr/bin/python$CLIF_PYTHON_VERSION /usr/local/bin/python3

cd && rm -rf clif && git clone https://github.com/google/clif.git && cd clif

if [[ ! -z ${CLIF_PIN} ]]; then
  git checkout "${CLIF_PIN}"
fi

./INSTALL.sh
