#!/bin/sh
# Run a command in the Dockerfile's build environment, with this repository
# (disks/ and external/ included) mounted at /dw3; a shell without one:
#
#   tools/docker.sh make VERSION=eu generate
#
# It builds the image first (dw3_decomp, or $DW3_IMAGE), which is quick once
# Docker has it cached, and runs as the calling user, so that what the build
# writes stays the user's. VERSION is passed on when it is set.

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
IMAGE="${DW3_IMAGE:-dw3_decomp}"

# the image has no git: the submodules are checked out outside of it
if [ ! -f "$TOP/external/maspsx/maspsx.py" ]; then
	echo "external/ is empty: run git submodule update --init --recursive first" >&2
	exit 1
fi

docker build -q -t "$IMAGE" "$TOP" > /dev/null

[ $# -gt 0 ] || set -- bash
tty=
if [ -t 0 ] && [ -t 1 ]; then
	tty=-it
fi

exec docker run --rm $tty -v "$TOP:/dw3" -w /dw3 -u "$(id -u):$(id -g)" \
	-e VERSION "$IMAGE" "$@"
