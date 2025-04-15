#!/bin/bash

# List of required packages
REQUIRED_PKGS="antlr4 libantlr4-runtime-dev"

# Function to check if a package is installed
check_installed() {
    dpkg -l | grep -q "^ii  $1 "
}

# # Ensure the script runs with sudo privileges
# if [[ $EUID -ne 0 ]]; then
#     echo "This script must be run as root. Restarting with sudo..."
#     exec sudo bash "$0" "$@"
# fi

# Update package lists
echo "Updating package lists..."
apt-get update

echo "Checking and installing missing dependencies..."
for pkg in $REQUIRED_PKGS; do
    if ! check_installed "$pkg"; then
        echo "Installing $pkg..."
        apt-get install -y "$pkg"
    else
        echo "$pkg is already installed."
    fi
done

echo "All dependencies are installed. Proceeding with build..."

: ${CMAKE_DIR:=cmake-build}

# cmake -D CMAKE_BUILD_TYPE=Debug -S . -B $CMAKE_DIR
cmake -D CMAKE_BUILD_TYPE=Release -S . -B $CMAKE_DIR
cmake --build $CMAKE_DIR