#!/bin/bash
set -e -o pipefail

sudo apt update && sudo apt install wget g++ gdb make ninja-build rsync zip software-properties-common lsb-release -y
sudo apt clean all
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo apt update && sudo apt install kitware-archive-keyring -y && sudo rm /etc/apt/trusted.gpg.d/kitware.gpg && sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4
sudo apt update && sudo apt install cmake -y

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt update && sudo apt install libassimp-dev g++-11 -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 --slave /usr/bin/g++ g++ /usr/bin/g++-11
sudo add-apt-repository -y ppa:oibaf/graphics-drivers
sudo apt update && sudo apt upgrade -y
sudo apt install libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-tools -y

# Install additional Vulkan packages:
wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
sudo apt update
set +e
set +o pipefail
sudo apt install vulkan-sdk dpkg-dev libvulkan1-dbgsym vulkan-tools-dbgsym -y --fix-missing
set -e
set -o pipefail

# Install glfw and glm and opengl glu glew dependency (glfw is needed for the vulkan project, glu is needed for the OpenGL project):
sudo apt install libglfw3 libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev -y

# However, we need a newer version of libglew from Ubuntu 22, even if we are on Ubuntu 20. This does not make a difference if we are
# already on Ubuntu 22:
sudo add-apt-repository -s "deb http://archive.ubuntu.com/ubuntu/ jammy universe" -y
sudo apt install libglew-dev -y
# Remove jammy again from apt repos to ensure that other packages are not uncontrolled updated.
sudo add-apt-repository --remove "deb http://archive.ubuntu.com/ubuntu/ jammy universe" -y

wget -qO ./glm.tar.xz http://archive.ubuntu.com/ubuntu/pool/main/g/glm/glm_0.9.9.8+ds.orig.tar.xz
sudo tar -xf glm.tar.xz
sudo mv glm-0.9.9.8/glm/ /usr/include/glm
rm -rf glm*

echo ""
echo "Now running \"vulkaninfo\" to see if vulkan has been installed successfully:"
vulkaninfo