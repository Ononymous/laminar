Install Git
`sudo yum install git`

## Set up SSH keys for Git
`cd ~/.ssh`
`ssh-keygen -t ed25519 -C "tylerekaireb@gmail.com"`
(Add public key to GitHub)

## Set up CSPOT
`git clone git@github.com:MAYHEM-Lab/cspot.git`
`cd cspot`
`git checkout rich2.0`
`sudo ./install-centos7.sh`

## Set up cspot-apps
`cd ~`
`git clone git@github.com:MAYHEM-Lab/cspot-apps.git`
`cd cspot-apps`
`sudo ./install_cspot_apps.sh`

## Test cspot-apps
`sudo ./test_apps.sh -c ~/cspot`

## Test Laminar
`cd ~/cspot-apps`
`mkdir tmp && cd tmp`
`cp ~/cspot/build/bin/woofc*`
`cp ../build/bin/output_handler`
`cp ../build/bin/subscription_event_handler`
`cp ../build/bin/tests`
In one terminal (start CSPOT platform): `sudo ./woofc-namespace-platform`
In another terminal (run tests): `sudo ./tests`
To kill woofc platform: `sudo pkill woofc`
