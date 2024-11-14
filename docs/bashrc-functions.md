# Laminar Helpers

Manually building, copying and executing files can be annoying. You can add the following to `~/.bashrc` to make the process faster:

NOTE: If you are using a different woof path (CSPOT namespace location) than `/home/centos/laminar`, make sure to change that in the functions below.

```sh
if ! [[ $LD_LIBRARY_PATH == *"/usr/local/lib"* ]]; then
        export  LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
fi

if ! [[ $PATH == *"devtoolset"* ]]; then
        export PATH="/opt/rh/devtoolset-10/root/usr/bin:$PATH"
fi

cspot() {
        curr_dir=$(pwd)
        cd /home/centos/laminar
        sudo /home/centos/laminar/woofc-namespace-platform
        cd $curr_dir
}

cspot_log() {
        curr_dir=$(pwd)
        cd /home/centos/laminar
        sudo /home/centos/laminar/woofc-namespace-platform >& /home/centos/laminar/namespace.log
        cd $curr_dir
}

tear() {
        sudo pkill woofc
        rm -f /home/centos/laminar/cspot-log*
}

build() {
        curr_dir=$(pwd)
        cd /home/centos/cspot-apps/build
        sudo cmake ..
        sudo make $1
        cd $curr_dir
}

sync_laminar() {
        curr_dir=$(pwd)
        cd /home/centos/laminar
        rm output_handler
        rm subscription_event_handler
        cp /home/centos/cspot-apps/build/bin/output_handler .
        cp /home/centos/cspot-apps/build/bin/subscription_event_handler .
        cd $curr_dir
}

load() {
        curr_dir=$(pwd)
        cd /home/centos/laminar
        cp /home/centos/cspot-apps/build/bin/$1 /home/centos/laminar
        p_name=$1
        shift
        sudo /home/centos/laminar/$p_name $@
        rm -f /home/centos/laminar/$p_name
        cd $curr_dir
}

load_log() {
        curr_dir=$(pwd)
        cd /home/centos/laminar
        cp /home/centos/cspot-apps/build/bin/$1 /home/centos/laminar
        p_name=$1
        shift
        sudo /home/centos/laminar/$p_name $@ >> $(p_name).txt
        rm -f /home/centos/laminar/$p_name
        cd $curr_dir
}

clean() {
        rm -rf /home/centos/laminar/lmr*
}
```

### cspot()
Will launch the CSPOT runtime.

Usage: `cspot`

### cspot_log()
Will launch the CSPOT runtime and write its output to `namespace.log` file.

Usage: `cspot_log`

### tear()
Interrupts and kills all CSPOT processes and removes the cspot-log file.

Usage: `tear`

### build()
Makes the requested Makefile rule. For complete cspot-apps build, no arguments are needed.

For building a specific test/benchmark, use the name of the rule in the CMakeLists.txt

Usage: `build <program_name>`

Example: `build simple_laminar_example`

### sync_laminar()
If you rebuilt cspot-apps, you need to replace the output_handler and subscription_event_handler with the new binaries. This command will do that sync.

Usage: `sync_laminar`

### load()
Will copy the binary of the test/benchmark from cspot-apps to the woof path, and execute the program with given arguments.

Usage: `load <program_name> [args]`

Example: `load riot_stats -m 20 -d 5`

### clean()
Will remove all woofs in the woof path. To be used after a laminar program has executed, for cleanup before the next execution.