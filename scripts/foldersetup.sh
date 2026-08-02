#!/bin/bash
# Foldersetup script — creates all the required folders for latency-zero
# Run once after cloning: bash scripts/foldersetup.sh

set -e  # exit immediately if any command fails

echo "Setting up folders..."

# Get the directory where this script resides
dir_path=$(dirname "$(realpath "$0")")
echo "Script dir: $dir_path"

# Go one folder back and resolve the full path
root_path=$(realpath "$dir_path/..")
echo "Root path:  $root_path"

# Create required folders inside the root directory
mkdir -p "$root_path/build"
mkdir -p "$root_path/bin"

echo "Done. All folders setup."
