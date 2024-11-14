#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"

void KSAnomaly1Hosts() 
{
	struct timeval tm;
	double ts;
	double bench_start_time;
	double bench_end_time;
	int duty_cycle_count = 5;
	int i;
	int op_node; 
	int iteration;

	system("sudo rm -f lmr*");
	laminar_init();
	laminar_reset(); /* reset setup data structures */
	set_host(1);
	add_host(1, "169.231.230.191", "/cspot-device-namespace/");

	int ns = 1;

	// single node computes KS test
	add_node(ns, 1, 1, {DF_CUSTOM, KS_TEST});

	// operands come from node 1 -- start with 5
	// use op_node variable to give the virtual node ids
	// note that virtual graph boundary nodes must begin
	// 1 higher than actual nodes
	op_node = 2;
	for(i=0; i < duty_cycle_count; i++) {
		add_operand(ns, 1, op_node); // value from graph boundary on node 1
		/*
		 * subscribe node 1 on host1 at port i to this operand coming from
		 * host 1 
		 */
		subscribe(ns, 1, i, ns, op_node); 
		op_node++;
	}

	laminar_setup();
	for(iteration = 1; iteration <= 5; iteration++) {
		op_node = 2;
		struct ts_value operands[duty_cycle_count];
		for(i=0; i < duty_cycle_count; i++) {
			set_double(&operands[i],rand()); // simulated measurement
			operand opnd(&operands[i],iteration); // iteration count
			printf("firing operand %d\n",op_node);
			fire_operand(ns,op_node,&opnd); // fire the operand
			op_node++;
		}
		

		operand result;
		int err = get_result(ns, 1, &result, iteration);
		if (err < 0) {
			std::cout << "Failed to read the result " << std::endl;
		} else {
			printf("KS: %d\n",result.operand_value.value.ts_int);
		}
	}

    // std::cout << "Size: " << std::string(result_str).size() << std::endl;
    // std::cout << "Type: " << result.operand_value.type;
}

void KSAnomaly2Hosts() 
{
	struct timeval tm;
	double ts;
	double bench_start_time;
	double bench_end_time;
	int duty_cycle_count = 5;
	int i;
	int op_node; 
	int iteration;

	system("sudo rm -f lmr*");
	laminar_init();
	laminar_reset(); /* reset setup data structures */
	set_host(2);
	add_host(1, "169.231.230.191", "/devices/feather-1/");
	add_host(2, "169.231.230.191", "/cspot-device-namespace/");

	int ns = 1;

	// single node computes KS test on node 2
	add_node(ns, 2, 1, {DF_CUSTOM, KS_TEST});

	// operands come from node 1 -- start with 5
	// use op_node variable to give the virtual node ids
	// note that virtual graph boundary nodes must begin
	// 1 higher than actual nodes
	op_node = 2;
	for(i=0; i < duty_cycle_count; i++) {
		add_operand(ns, 1, op_node); // value from graph boundary on node 1
	/*
	 * subscribe node 1 on host2 at port i to this operand coming from
	 * host 1 
	 */
		subscribe(ns, 1, i, ns, op_node); 
		op_node++;
	}

	laminar_setup();

	for(iteration = 1; iteration <= 5; iteration++) 
	{
		if(get_curr_host_id() == 1) {
			//gettimeofday(&tm,NULL);
			//bench_start_time = (double)tm.tv_sec + (tm.tv_usec / 1000000.0); // sec
			op_node = 2;
			struct ts_value operands[duty_cycle_count];
			for(i=0; i < duty_cycle_count; i++) {
				set_double(&operands[i],rand()); // simulated measurement
				operand opnd(&operands[i],iteration); // iteration count
				printf("firing operand %d\n",op_node);
				fire_operand(ns,op_node,&opnd); // fire the operand
				op_node++;
				}

		}
	       /*
		* host 2 waits for the KS score
		*/
		if(get_curr_host_id() == 2) {
			operand result;
			int err = get_result(ns, 1, &result, iteration);
			if (err < 0) {
				std::cout << "Failed to read the result " << std::endl;
			} else {
				printf("KS: %d\n",result.operand_value.value.ts_int);
			}
			
		}
	}
    // std::cout << "Size: " << std::string(result_str).size() << std::endl;
    // std::cout << "Type: " << result.operand_value.type;
}

int main() {
//    KSAnomaly1Hosts();
	KSAnomaly2Hosts();
    return 0;
}
