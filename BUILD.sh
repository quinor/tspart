#!/bin/bash

# build commands
cmake -B build . --install-prefix $(pwd)/tspart
cmake --build build -j4

# create tarball commands
cmake --install build --component tspart_release
tar czf tspart.tar.gz tspart
