# Laminar -- Log-based Dataflow

Laminar is a simple API for implementing dataflow programs that use append-only logs and triggered computations as their only underlying storage abstraction.  At present, Laminar uses [CSPOT](https://github.com/MAYHEM-Lab/cspot) to implement this functionality.

#### Installing Laminar

First, download and install CSPOT.  Ubuntu 20.04 is the currently preferred distribution.  CSPOT includes an installation script for this distro.  Note that the installation script installs Docker and requires sudo permissions to install the CSPOT libraries and to run its self-tests using Docker.

Next, issues the following commands in the top level Laminar directory:

	mkdir build
	cd build
	cmake ..
	make

The build will create a number of Laminar test programs and benchmarks.  These programs typically must run in a CSPOT namespace with CSPOT active (e.g. with woofc-namespace-platform running for the namespace).
