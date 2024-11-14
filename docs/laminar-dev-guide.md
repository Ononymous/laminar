# Laminar Developer Guide

This document contains guidelines and documentation of Laminar's abstractions, exposed for application developers.

**Table of Contents**
* [Laminar Components](#laminar-components)
* [Laminar APIs](#laminar-apis)
    * [Global State API](#global-state-api)
    * [Graph Construction API](#graph-construction-api)
    * [Execution API](#execution-api)
* [Creating Custom Operations](#creating-custom-operations)
* [Common Errors](#common-errors-in-laminar-application-development)

<!---
TODO:
- using the bashrc functions
- fast dev workflow
- debugging using logs
    - using printf statements
-->

## Laminar Components

Laminar is a dataflow programming language. It has the following constructs which are part of the dataflow representation:

### Hosts
Each host represents a CSPOT namespace. The hosts can exist on different machines. Each host has an ID, an IP address and the namespace path, and uniqueness is defined by the pair (IP, namespace_path).

NOTE: Host IDs HAVE to be numbered sequentially starting with 1 in the current version of laminar.

### Namespaces
Each _Laminar_ namespace is a logical block which contains a set of nodes. Note that this is different from the namespaces in CSPOT. Laminar namespaces exist to allow node id's to be reused. Namespace ids should be allotted sequentially starting from 1.

NOTE: Namespaces are not programmed to abstract the input and output edges to a set of nodes in the current version of laminar.

### Nodes
A node represents a logical operation or computation. Each node can have a variable number of inputs. A node can have a single result (output) but composite datatypes like strings, arrays, maps, etc. can be used to pass multiple values in an output. Node id's should start from 1 in every (Laminar) namespace. A node is uniquely identified by the pair (namespace_id, node_id). A node starts execution only once it receives all its inputs.

### Ports
Each node has a variable number of input sources (subscriptions) and each source is assigned a port number in the destination node. The port numbers start from 0 and increase sequentially.

### High-level view
A typical laminar program consists of setting up the laminar state, defining the hosts in the cluster, defining the nodes, which includes the operation performed in the node, and the location of the node i.e. the host machine and the namespace. Followed by connecting those nodes by subscriptions to relevant sources and sinks using the ports in the node. The final step is to enable the initial nodes to recieve data through operands and let the dataflow execution generate the result.


## Laminar APIs

### Global State API

#### laminar_init()

 Initializes the laminar state. It also reconstructs the state with the persistent logs if the application is restarting. It is used in the beginning of a laminar app.

#### laminar_setup()

Freezes the Laminar program's dataflow representation. It is called after the dataflow graph is defined (using the [graph construction API](#graph-construction-api)).

#### add_host(int host_id, const std::string& host_ip, const std::string& woof_path, enum RetryType retry_type = RETRY_EXPONENTIAL_BACKOFF)

Used to create a registry of all the hosts in the application execution cluster. It populates the registry with the information needed to interact with the host i.e host-id, host ip address and the host WooF path (CSPOT namespace). Developers can optionally specify the retry mechanism the host should follow for desination nodes of different machines in case of network failure. Host id's should start from 0 and increase sequentially.

Example:
```
add_host(1, "localhost", "/home/centos/myapp");
```

`RETRY_IMMEDIATE`: No wait time, retry immediately.

`RETRY_INTERVAL`: A constant delay of 10k ms between retries.

`RETRY_LINEAR_BACKOFF`: Linearly increasing retry delay, capped at 32k ms.

`RETRY_EXPONENTIAL_BACKOFF`: Exponentially increasing retry delay, capped at 32k ms. Use in cases where slow recovery is expected after network partition.

`RETRY_LOG_BACKOFF`: Logarithmically increasing retry delay, capped at 32k ms. Use in cases where quick recovery is expected after network partition.

#### set_host(int host_id)

Assigns the host id for the CSPOT namespace (host) in which an instance of user program is going to run. This is the only function which has a different argument in different hosts/machines in a laminar application. For example, if an application defines 2 hosts, it must be compiled and run with `set_host(1)` for the first host, and `set_host(2)` for the second host. Make sure the id provided in this function is the correct one, corresponding to the id's assigned with the `add_host(...)` calls.


### Graph Construction API 

#### add_node(int ns, int host_id, int id, DF_OPERATION operation)

Adds a node to the dataflow graph. Takes the laminar namespace, host id, node id, and the operation information to generate the node.

The 'operation' contains info of the type of the operation and the enum representing the operation. The argument can be provided in the format `{.category = <category>, .operation = <operation_name>}`. The categories and operation names can be found in [df_operations.h](../operation_system/include/operation_system/df_operations.h).

Example:
```
add_node(1, 1, 1, {.category = DF_ARITHMETIC, .operation = DF_ARITH_ADDITION});
```

#### add_operand(int ns, int host_id, int id)

Adds an operand to the dataflow graph. There is no operation attached to an operand. Operands are used to provide external inputs to nodes, and perform no computation. An operand functions as the source of data from the end devices or custom input parameters by the application developer for the dataflow graph. Values must be provided to the operand node by the user program using the [execution API](#execution-api).

Example:
```
add_operand(1, 1, 1);
```

#### subscribe(const std::string& dst_addr, const std::string& src_addr)

Creates an edge in the dataflow graph. It requires the source and destination information in the form of strings. The format for the information:

dst_addr : `"<namespace_id>:<node_id>:<port_number>"`

src_addr: `"<namespace_id>:<node_id>"`

Example:
```
subscribe("1:2:0", "1:1");
```

#### subscribe(int dst_ns, int dst_id, int dst_port, int src_ns, int src_id)

Creates an edge in the dataflow graph. It requires the destination and source information in the form of integer ids. This form is useful while declaring subscriptions programatically in loops, etc.

Example:
```
subscribe(1, 2, 0, 1, 1);
```

### Execution API

#### fire_operand(int ns, int id, const operand* op, bool trigger_handler = true)

This call fires the provided operand struct using the operand node identified by (ns, id). The operand struct (`const operand* op`) consists of the value and iteration number. The constructor for an operand struct is as follows:

`operand(struct ts_value* value = nullptr, unsigned long long itr = 1)`

The iteration number (itr) **MUST** start from 1 and **MUST** be sequentailly increasing, and has to be handled by the application developer. If the operand is a constant and not a value which should lead to computation, then trigger_handler can be set to false.

Example:
```
operand op(my_value, 1);
fire_operand(1, 2, &op);
```

#### get_result(int ns, int id, operand* res, unsigned long itr)

This call retrieves the output of a given version (itr) from a node into an operand.

NOTE: This call uses a loop to check if the output is ready. If the latency for the computation is emprically known, consider putting a `sleep()` for that duration before attempting to get the result.

If there is an error while reading the result, `get_result()` will return an integer less than 0.

Example:
```
operand result_op;
int err = get_result(1, 1, &result_op, 1);
```


## Creating Custom Operations

Developers can create custom operations for each node. Here are the steps to follow to create custom operations. 

- Define the enum for custom function in ```cspot-apps/dataflow_multiwoof/operation_system/df_operation.h``` in ```df_custom_ops```.

- Add the custom function in ```cspot-apps/dataflow_multiwoof/operation_system/df_custom_operation.cpp``` file.

- Map the custom function to the enum using the switch statement inside ```df_custom_operation``` function defined in ```cspot-apps/dataflow_multiwoof/operation_system/df_custom_operation.cpp```

- Head to ```cspot-apps/dataflow_multiwoof/operation_system/df_custom_operation.cpp``` for an example. 


## Common Errors in Laminar Application Development

- The state of the execution must be erased after the execution is completed, attempting to re-run will just make the system read values from state and not perform any computation. Use `rm -rf laminar*` to remove all laminar related woofs. 

- The three entities : the host ids, the node ids, the namespace ids start with 1 and have to sequential. 

- Port Ids can start with 0 but have to be sequential.


