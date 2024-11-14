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
#include "type_system/types/ts_primitive.h"

extern "C" {

int AnomalyIteration;
int AnomalyCycle;
void Anomaly2HostsSetUp() 
{
	struct timeval tm;
	double ts;
	double bench_start_time;
	double bench_end_time;
	int duty_cycle_count = 5;
	int i;
	int op_node; 
	int iteration;

	AnomalyIteration = 1;
	AnomalyCycle = 0;

	laminar_init();
	laminar_reset(); /* reset setup data structures */
	set_host(1);
//	add_host(1, "169.231.230.191", "/devices/feather-1/");
	add_host(1, "169.231.230.191", "/devices/linux@169.231.230.148/");
//	add_host(1, "169.231.230.191", "/devices/linux@pi3-10/");
	add_host(2, "169.231.230.191", "/cspot-device-namespace/");

	int ns = 1;

	// computes KS test on host 1
	// computes REGRESS test on host 2
	// computes CORR test on host 2
	// computes ANOMALY test on host 2
	add_node(ns, 2, 1, {DF_CUSTOM, KS_TEST});
	add_node(ns, 2, 2, {DF_CUSTOM, REGRESS_TEST});
	add_node(ns, 2, 3, {DF_CUSTOM, CORR_TEST});
	add_node(ns, 2, 4, {DF_CUSTOM, ANOMALY_TEST});

	// operands come from node 1 -- start with 5
	// use op_node variable to give the virtual node ids
	// note that virtual graph boundary nodes must begin
	// 1 higher than actual nodes
	op_node = 5;
	for(i=0; i < duty_cycle_count; i++) {
		add_operand(ns, 1, op_node); // value from graph boundary on node 1
	/*
	 * subscribe node 1 (KS) node 2 (REGRESS), and node 3 (CORR) on host 2 at port i to this operand coming from
	 * host 1 
	 */
		subscribe(ns, 1, i, ns, op_node); 
		subscribe(ns, 2, i, ns, op_node); 
		subscribe(ns, 3, i, ns, op_node); 
		op_node++;
	}

        // subscribe anomaly test node to KS (node 1) on port 0
        subscribe(ns,4,0,ns,1);
        // subscribe anomaly test node to REGRESS (node 2) on port 1
        subscribe(ns,4,1,ns,2);
        // subscribe anomaly test node to CORR (node 3) on port 2
        subscribe(ns,4,2,ns,3);
	laminar_setup();


	return;
}

double *Adata;
void Anomaly2HostsBody() 
{
	struct timeval tm;
	double ts;
	double bench_start_time;
	double bench_end_time;
	int duty_cycle_count = 5;
	int i;
	int op_node; 
	double dval;

	int ns = 1;

	if(get_curr_host_id() == 1) {
		if(AnomalyIteration == 1) {
			Adata = (double *)malloc(duty_cycle_count * sizeof(double));
			if(Adata == NULL) {
				exit(1);
			}
			for(i=0; i < duty_cycle_count; i++) {
				Adata[i] = rand();
			}
		}
		//gettimeofday(&tm,NULL);
		//bench_start_time = (double)tm.tv_sec + (tm.tv_usec / 1000000.0); // sec
		op_node = AnomalyCycle + 5;
		struct ts_value operands[duty_cycle_count];
#if 0
		for(i=0; i < duty_cycle_count; i++) {
			dval = Adata[i] * AnomalyIteration;
			set_double(&operands[i],dval); // simulated measurement
			operand opnd(&operands[i],AnomalyIteration); // iteration count
			printf("firing operand %d, iter: %d, value: %f\n",
						op_node, AnomalyIteration, dval);
			fire_operand(ns,op_node,&opnd); // fire the operand
			op_node++;
		}
#endif
		dval = Adata[AnomalyCycle] * AnomalyIteration;
		set_double(&operands[AnomalyCycle],dval); // simulated measurement
		operand opnd(&operands[AnomalyCycle],AnomalyIteration); // iteration count
		printf("firing operand %d, iter: %d, value: %f\n",
						op_node, AnomalyIteration, dval);
		fire_operand(ns,op_node,&opnd); // fire the operand
		AnomalyCycle++;
		if(AnomalyCycle == duty_cycle_count) {
			AnomalyCycle = 0;
			AnomalyIteration++;
		}

	}
       /*
	* host 2 waits for the ANOMALY decision
	*/
	if(get_curr_host_id() == 2) {
		operand result;
		int err = get_result(ns, 4, &result, AnomalyIteration);
		if (err < 0) {
			std::cout << "Failed to read the first result " << std::endl;
		} else {
			printf("Anomaly Test iteration %d: %d\n", AnomalyIteration, result.operand_value.value.ts_int);
		}
	}
    // std::cout << "Size: " << std::string(result_str).size() << std::endl;
    // std::cout << "Type: " << result.operand_value.type;
}
} // extern C
//int main() {
//	RegressAnomaly2Hosts();
//    return 0;
//}
