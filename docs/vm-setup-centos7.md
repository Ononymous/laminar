# Laminar VM Setup

**Table of Contents**
* [Provision VM](#vm-instance)
* [Setup VM and CSPOT](#setup-vm-and-cspot)
* [Setup Laminar](#setup-laminar)
* [CSPOT Namespaces](#create-a-cspot-namespace)
* [Re-building Laminar](#future-laminar-builds)
* [Running a Laminar Program](#running-a-laminar-program)

NOTE: While doing the setup for the first time, it is best to go through the entire document serially.

## VM Instance

1. Provision a VMin the `race` az with `centos-7` image on Eucalyptus (image id: `ami-8c8192f1011f1a61a`).
2. SSH into the VM using the centos user `ssh -i <key_name>.pem centos@169.231.230.xyz`.
3. Install vim: `sudo yum -y install vim`

## Setup VM and CSPOT

1. In the home directory, create a file setup.sh: `touch ~/setup.sh`
2. Open the file with vim and copy + paste the following script in the file:

```sh
#!/bin/bash

yum -y update

yum -y install centos-release-scl-rh
yum --enablerepo=centos-sclo-rh-testing -y install devtoolset-10
source scl_source enable devtoolset-10
echo "source scl_source enable devtoolset-10" >> ~/.bashrc
echo 'if ! [[ $LD_LIBRARY_PATH == *"/usr/local/lib"* ]]; then export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"; fi' >> ~/.bashrc
echo 'if ! [[ $PATH == *"devtoolset"* ]]; then export PATH="/opt/rh/devtoolset-10/root/usr/bin:$PATH"; fi' >> ~/.bashrc
source ~/.bashrc

yum -y install python3 python3-pip npm git libffi-devel
yum -y install openssl-devel openssl-static
yum -y install glibc-static epel-release
yum -y install boost boost-thread boost-devel
yum -y install zlib-devel zlib
yum -y install wget

cd /home/centos
wget https://cmake.org/files/v3.19/cmake-3.19.1.tar.gz
tar zxvf cmake-3.*
rm cmake-3.19.1.tar.gz
cd cmake-3.*
./bootstrap --prefix=/usr/local
make -j$(nproc)
make install
cmake --version

cd /home/centos

git clone -q https://github.com/MAYHEM-Lab/cspot.git
cd cspot
./install-centos7.sh
```

3. Give execute permissions `sudo chmod 777 ~/setup.sh`
4. Run the script `sudo ~/setup.sh`, and go out for a long long walk.

## Setup Laminar

1. `cd ~ && git clone https://github.com/MAYHEM-Lab/cspot-apps.git` NOTE: Use github ssh if you have that configured.
2. `cd cspot-apps`
3. `sudo ./install_cspot_apps.sh`

## Create a CSPOT Namespace

A CSPOT namespace will contain the CSPOT runtime binaries, woofs and handlers for woofs. For this setup, we create the namespace in `/home/centos/laminar/`. This location might be referred to as the **woof path** in documents and code to differentiate it from the 'namespace' in Laminar.

1. `cd ~ && mkdir laminar && cd laminar` Note: create in any location with any name, be sure to update the add_host() in programs.
2. `cp ~/cspot/build/bin/woofc-* .`
3. `cp ~/cspot-apps/build/bin/output_handler .`
4. `cp ~/cspot-apps/build/bin/subscription_event_handler .`

NOTE: For build + run helper commands, have a look at [bashrc functions](bashrc-functions.md).

## Future Laminar Builds

- If any file apart from the ones in `tests` and `benchmarks` directories are changed, you will need to do a full cspot-apps build using `sudo ./install_cspot_apps.sh`.
- If only user programs (tests, benchmarks, etc) are changed, partial builds (of only the modified file) can be done to speed up development. See the `build()` function in the `bashrc-functions.md` to see how.

## Running a Laminar Program

1. Make sure the woof_path for the host in your program is pointing to the correct location. Here, it is: `/home/centos/laminar`.
2. Re-build cspot-apps if you made a change.
3. `cd <woof-path-directory>`
4. Copy the program you wish to run into the CSPOT namespace `cp ~/cspot-apps/build/bin/simple_laminar_example .`.
5. In one terminal, run CSPOT: `sudo ./woofc-namespace-platform`
6. In another terminal, run the laminar program: `sudo ./simple_laminar_example`
7. Once the laminar program has finished execution, run `sudo pkill woofc` to terminate CSPOT.
8. Cleanup with `sudo rm -f lmr*` before running another laminar program.

NOTE: If you wish to use a single terminal, CSPOT can be run in the background. If doing so, it is a good idea to redirect the output os CSPOT into a log file: `sudo ./woofc-namespace-platform >& namespace.log &`.