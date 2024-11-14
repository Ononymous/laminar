# Laminar Internals

This document details the internal working of Laminar.

 **Table of Contents**
 * [Overview of CSPOT](#overview-of-cspot)
 * [Laminar Handlers](#laminar-handlers)
 * [Type System](#type-system)
 * [Operation System](#operation-system)
 * [Logger System](#logger-system)

## Overview of CSPOT

Laminar is essentially an abstraction over CSPOT. Laminar is supposed to be "event-driven". This section gives an overview of flow of control in CSPOT for an "event".

In the CSPOT context, an "event" is an append to a WooF. The CSPOT WooFPut() call takes the name of the WooF, and the name of the handler to execute **after** the append.

All the files referred here are in [cspot/src/](https://github.com/MAYHEM-Lab/cspot/tree/master/src).

### Step 1: Append to WooF & log the event
- `woofc.cpp` implements the CSPOT API.
- The `WooFPut(woof_name, handler, element)` appends the `element` to the woof `woof_name` and returns the sequence number allotted to the element.
- The handler is the name of the handler binary present in the CSPOT namespace.
- Internally, `WooFPut()` calls `WooFAppendWithCause()` which does majority of the work.
    - The `memcpy(ptr, element, wfs->element_size)` call appends the element.
    - The `ls = LogEvent(Name_log, ev)` call adds an event to the CSPOT namespace event log.

### Step 2: Pick up an event
- The main container process is written in `woofc-container.cpp`. This process is responsible for creating forks that execute the handlers.
    - `constexpr auto WOOF_CONTAINER_FORKERS` sets the maximum number of threads the container will run. For larger machines, this number be increased from 8 to 2 * number_of_cores to fully utilize the machine's computing power.
- The `WooFForker()` function picks up events from the event log and puts them in the pipe for execution.
- The event and required metadata is bundled in a string and written as a message to a pipe `err = write(ta->parenttochild[1],payload,mcurr)`.

### Step 3: Handler execution
- The process implemented in `woofc-forker-helper.cpp` executes the handler.
- The call `err = posix_spawn(&pid,fargv[0],NULL,NULL,fargv,menv)` fires the handler binary.
- Note that the extrenal handler function is not called directly. The handler binary is actually created by combining `woofc-shepherd.cpp` and the custom handler code. The `woofc-shepherd.cpp` file has the `main()` for the binary.
    - The custom handler function is recognized as an extern function during linking. The function is called almost at the end of the file with `err = handler(wf, seq_no, (void*)farg)`.


## Laminar Handlers

Laminar uses 2 handlers : `output_handler` and `subscription_event_handler`.

### Output Handler
- Data in laminar flows the the output WooF (`OUT_WF_TYPE`) of each node and operand.
- A put on the `OUT_WF_TYPE` woof fires the `output_handler`.
- The output handler goes over the subscriber map and puts a `subevent` in the subscription event woof of each of the subscribers.
- For operands, the `fire_operand()` call writes to the output woof.
- For nodes, the subscription event handler writes to the output woof.

### Subscription Event Handler
- "Events" in laminar are recorded in the subscription event WooF (`SUBN_EVNT_WF_TYPE`) of each node.
- A put on the `SUBN_EVNT_WF_TYPE` woof fires the `subscription_event_handler`.
- Suppose node A subscribes to node B, when node B has a new output, it becomes an "event" for node A.
- The handler tries to fetch inputs from all subscriptions, and exits if all inputs are not available.
- If all inputs are found, the handler goes through a write-first sync mechanism and then executes the operation for the node.
- Once the operation is performed, the handler writes the result to the output woof of the node, thereby asking CSPOT to fire the output handler.


## Type System

The `type_system` deals with various dataypes and their operations. The main job is dealing with conversion between datatypes and `ts_value` struct.

The header `cspot-apps/dataflow_multiwoof/type_system/include/type_system/type_system/ts_types.h` details the types and structs.

### Strings, Arrays and other Composites
Currently, strings, arrays, and other composite types are stored using a 'storage system'. Since the lengths might be variable, the idea is to store the serialized data in a separate WooF, and have a struct containing the reference to the data along with metadata as the value in `ts_value`.

This mechanism works for a single CSPOT namespace, but does not work accross namespaces (and thereby hosts). This means that if node A is in namespace 1 and node B is in namespace 2, B cannot read the output of A if it is a composite type.

Have a look at `write_value()` in `ts_type.c`, and `ts_string.c` for a better understanding.

### Primitive Types
The workaround for the above problem is fixed-size primitive types. The primitive types have a prefix `ts_prim_`.

For example, the `ts_prim_string` has a fixed size of `TS_PRIM_STRING_SIZE`.

Primitive datatypes can be read across CSPOT namespaces and thus work accross hosts.

To add a prim datatype, first add it to `ts_types` and `ts_values` in `ts_types.h`. Then add the conversion functions in `ts_primitive.c`. Also add a case in the required functions in `ts_types.c`. Follow any existing primitive type to make sure all changes are in place.


## Operation System

The operation_system essentially maps an operation enumeration to the operation's code.

`operation_system/include/operation_system/df_operations.h` contains the enumerations for the operations.

The first layer of mapping is by `df_operation_categories` and is placed in `df_operation.c`. The second layer of mapping is in the c or cpp file for the particular operation category, for example, `df_linear_algebra.cpp`.

Instructions to add a new custom operation are in the [laminar dev guide](laminar-dev-guide.md#creating-custom-laminar-operations).


## Logger System

The logger system should be used extensively for debugging and adding runtime logs.

`df_logger.h` has the enumeration for log levels.

By default, the log level is set to `DF_WARN` in `df_logger_settings.h`. Changing it to `DF_DEBUG` will give details runtime debug logs.

While using `DF_DEBUG`, avoid using `std::cout` in the user programs. Use `printf()` or a logger function like `log_debug()` or `log_warn()` instead.

The logs are extremely helpful in catching bad runtime behavior and getting points of failure.

The logs emitted through the logger system are of the form:
```
2024-05-20 08:59:32 [debug] [DF-S-15734] [namespace:1][node_id:1] has current state: [iteration: 2]
```
Notice the segment `[DF-S-15734]`. This can be used as a search key for all logs pertaining to that particular execution of a handler. For highly concurrent graphs, this comes in very handy when log outputs are interleaved and it is required to isolate logs for a particular iteration of a node.