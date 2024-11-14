#ifdef ESP8266
#define CSPOTDEVICE
#endif
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

#if defined(CSPOTDEVICE) && defined(ESP8266)
#include "df_interface.h"
#include "ts_type.h"
//#include "ts_array.h"
//#include "ts_matrix.h"
#include "ts_primitive.h"
//#include "ts_string.h"
#else
#include "../df_interface.h"
#include "type_system/ts_type.h"
#include "type_system/types/ts_array.h"
#include "type_system/types/ts_matrix.h"
#include "type_system/types/ts_primitive.h"
#include "type_system/types/ts_string.h"
#endif

#ifdef CSPOTDEVICE
extern "C" {
#endif

#define SAMPLESIZE 1
#define INTERVAL 5

void LatencyTestBody() 
{
	struct timeval tm;
	double ts;
	int iteration = 0;
	int err;
	int ns = 1;

#ifndef CSPOTDEVICE
	laminar_init();
#endif

	while(iteration < SAMPLESIZE)
	{
		if(get_curr_host_id() == 1) {
			struct ts_value op1;
			set_double(&op1,100.0); // boundary value 100 on host 1
			operand opnd(&op1,iteration); // iteration count
			printf("firing operand 3\n"); // node 3 is graph boundary host 1
			fire_operand(ns,3,&opnd); // fire the operand

			// node 1 is add on host 1 which will fire when add
			// and graph boundary node on host 2 have fired
			operand result;
			int err = get_result(ns, 1, &result, iteration);
			if (err < 0) {
				std::cout << "Failed to read first result " << std::endl;
			} else {
				printf("Latency Test iteration %d: %f\n", iteration, result.operand_value.value.ts_double);
			}
			iteration++;

#ifdef CSPOTDEVICE
			break;
#else
			if(iteration < SAMPLESIZE) {
				sleep(INTERVAL);
			}
#endif
		}
	       /*
		* host 2 
		*/
		if(get_curr_host_id() == 2) {
			struct ts_value op2;
			set_double(&op2,200.0); // boundary value 200 on host 2
			operand opnd1(&op2,iteration); // iteration count
			printf("firing operand 4\n"); // node 4 is graph boundary host 2
			fire_operand(ns,2,&opnd1); // fire the operand
			iteration++;
		}
	}

    // std::cout << "Size: " << std::string(result_str).size() << std::endl;
    // std::cout << "Type: " << result.operand_value.type;
}

#ifdef CSPOTDEVICE
} // extern C
#endif

#ifndef CSPOTDEVICE
int main() {
	Anomaly2HostsBody();
    return 0;
}
#endif
