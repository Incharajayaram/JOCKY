#!/bin/bash
# Wrapper to run jocky.py inside the Docker container

if [ "$#" -eq 0 ]; then
    echo "Usage: ./jocky-docker.sh <input_file.c/cpp> [options]"
    exit 1
fi

docker run --rm -v "$(pwd)":/workspace jocky-env python3 jocky.py "$@"
