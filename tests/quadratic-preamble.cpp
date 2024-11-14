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

void QuadraticPreamble() 
{

	const struct df_operation multiplication = {DF_ARITHMETIC, DF_ARITH_MULTIPLICATION};
	const struct df_operation addition = {DF_ARITHMETIC, DF_ARITH_ADDITION};
	const struct df_operation subtraction = {DF_ARITHMETIC, DF_ARITH_SUBTRACTION};
	const struct df_operation division = {DF_ARITHMETIC, DF_ARITH_DIVISION};
	const struct df_operation sqrt = {DF_ARITHMETIC, DF_ARITH_SQRT};


#ifndef ESP8266
	system("sudo rm -f lmr* RE* KS* CO* ANOMALY.state");
#endif
	laminar_init();
	laminar_reset();

	set_host(1);
//	add_host(1, "169.231.230.148", "/cspot-device-namespace/");
//	add_host(1, "169.231.230.191", "/devices/feather-1/");
//	add_host(1, "169.231.230.191", "/devices/linux@169.231.230.148/");
//	add_host(1, "169.231.230.148", "/cspot-device-namespace/");
//	add_host(1, "169.231.230.191", "/devices/linux@pi3-10/");
	add_host(1, "169.231.230.191", "/cspot-device-namespace/");

	int graph1 = 1;
	int graph2 = 2;

	add_node(graph1, 1, 1, multiplication); // b^2
	add_node(graph1, 1, 2, multiplication); // 4 * a * c
	add_node(graph1, 1, 3, subtraction); // b^2 - 4ac
	add_node(graph1, 1, 4, sqrt); // sqrt(b^2-4ac)

	add_node(graph2, 1, 1, multiplication); // -1 * b
	add_node(graph2, 1, 2, multiplication); // 2 * a
	add_node(graph2, 1, 3, addition); // -b + sqrt(b^2-4ac)
	add_node(graph2, 1, 4, subtraction); // -b - sqrt(b^2-4ac)
	add_node(graph2, 1, 5, division); // (-b + sqrt(b^2-4ac))/2a
	add_node(graph2, 1, 6, division); // (-b - sqrt(b^2-4ac))/2a

	add_operand(graph1,1,5); // a
	add_operand(graph1,1,6); // b
	add_operand(graph1,1,7); // c
	add_operand(graph1,1,8); // 4
	add_operand(graph2,1,7); // -1
	add_operand(graph2,1,8); // 2

	subscribe(graph1,1,0,graph1,6);  // mult(port0) = b
	subscribe(graph1,1,1,graph1,6);  // mult(port1) = b
	subscribe(graph1,2,0,graph1,5);  // mult(port0) = a
	subscribe(graph1,2,1,graph1,7);  // mult(port1) = c
	subscribe(graph1,2,2,graph1,8);  // mult(port2) = 4
	subscribe(graph1,3,0,graph1,1);  // sub(port0) = b^2
	subscribe(graph1,3,1,graph1,2);  // sub(port0) = 4ac
	subscribe(graph1,4,0,graph1,3);  // sqrt(port0) = (b^2 - 4ac)

	subscribe(graph2,1,0,graph1,6); // mult(port0) = b
	subscribe(graph2,1,1,graph2,7); // mult(port1) = -1
	subscribe(graph2,2,0,graph1,5); // mult(port0) = a
	subscribe(graph2,2,1,graph2,8); // mult(port1) = 2
	subscribe(graph2,3,0,graph2,1); // add(port0) = -b
	subscribe(graph2,3,1,graph1,4); // add(port1) = sqrt(b^2 - 4ac)
	subscribe(graph2,5,0,graph2,3); // div(port0) = -b + (sqrt(b^2 - 4ac))
	subscribe(graph2,5,1,graph2,2); // div(port1) = 2a
	subscribe(graph2,4,0,graph2,1); // sub(port0) = -b
	subscribe(graph2,4,1,graph1,4); // sub(port1) = sqrt(b^2 - 4ac)
	subscribe(graph2,6,0,graph2,4); // div(port0) = -b - (sqrt(b^2 - 4ac))
	subscribe(graph2,6,1,graph2,2); // div(port1) = 2a



	laminar_setup();
	std::cout << graphviz_representation();

	return;
}

#ifdef CSPOTDEVICE
}// extern C
#endif

#ifndef CSPOTDEVICE
int main() {
	QuadraticPreamble();
	return 0;
}
#endif
