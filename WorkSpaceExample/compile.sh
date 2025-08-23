#!/bin/bash

set -e

cmake --build build --config Release --parallel $(nproc)