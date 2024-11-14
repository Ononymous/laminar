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

void QuadraticBody() 
{
	// (x + 1)(x - 2)
	double a = 1;
	double b = -1;
	double c = -2;
	int graph1 = 1;
	int graph2 = 2;
	int err;


#ifndef CSPOTDEVICE
	laminar_init();
#endif

	if(get_curr_host_id() == 1) { // all on host 1 for now

		struct ts_value op_4; // 4 is a constant
		set_double(&op_4,4.0); 
		operand opnd4(&op_4,1); // iteration 1 only
		fire_operand(graph1,8,&opnd4); // fire the 4 operand

		struct ts_value op_m1; // -1 is a constant
		set_double(&op_m1,-1.0); 
		operand opndm1(&op_m1,1); // iteration 1 only
		fire_operand(graph2,7,&opndm1); // fire the -1 operand

		struct ts_value op_2; // 2 is a constant
		set_double(&op_2,2.0); 
		operand opnd2(&op_2,1); // iteration 1 only
		fire_operand(graph2,8,&opnd2); // fire the 2 operand

		struct ts_value a_op;
		set_double(&a_op,a); 
		operand opnda(&a_op,1); // iteration 1 only
		printf("firing operand a: %f\n",a);
		fire_operand(graph1,5,&opnda); // fire the a operand

		struct ts_value b_op;
		set_double(&b_op,b); 
		operand opndb(&b_op,1); // iteration 1 only
		printf("firing operand b: %f\n",b);
		fire_operand(graph1,6,&opndb); // fire the a operand

		struct ts_value c_op;
		set_double(&c_op,c); 
		operand opndc(&c_op,1); // iteration 1 only
		printf("firing operand c: %f\n",c);
		fire_operand(graph1,7,&opndc); // fire the a operand

		operand rootpos;
		operand rootneg;
		err = get_result(graph2, 5, &rootpos, 1);
		if (err < 0) {
			std::cout << "Failed to read positive root " << std::endl;
		} else {
			printf("positive root: %f\n", rootpos.operand_value.value.ts_double);
		}
		err = get_result(graph2, 6, &rootneg, 1);
		if (err < 0) {
			std::cout << "Failed to read negative root " << std::endl;
		} else {
			printf("negative root: %f\n", rootneg.operand_value.value.ts_double);
		}
	}
}

#ifdef CSPOTDEVICE
} // extern C
#endif

#ifndef CSPOTDEVICE
int main() {
	QuadraticBody();
    return 0;
}
#endif
