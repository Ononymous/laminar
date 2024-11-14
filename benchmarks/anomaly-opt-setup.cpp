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

extern int anomaly_hist_el_size();

void AnomalyOptSetUp() 
{
	struct timeval tm;
	double ts;
	double bench_start_time;
	double bench_end_time;
	int duty_cycle_count = 5;
	int i;
	int op_node; 
	int iteration;
	int err;
	int state[6];
	unsigned long seqno;

	int my_host = 2;

#ifndef ESP8266
#ifdef CSPOTDEVICE
	system("rm -f lmr* RE* KS* CO* ANOMALY-OPT.state");
#else
	system("sudo rm -f lmr* RE* KS* CO* ANOMALY-OPT.state");
#endif
#endif
	laminar_init();
	laminar_reset();
	seqno = WooFGetLatestSeqno("ANOMALY-OPT.state");
	if((seqno == -1) || (seqno == 0)) {
		err = WooFCreate("ANOMALY-OPT.state",sizeof(int),2);
		if(err < 0) {
			printf("anomaly-test-setup: could not create state woof\n");
			exit(1);
		}
	} else {
		printf("anomaly-test-setup: restarting, setup exiting\n");
		return;
	}

	set_host(my_host);
//	add_host(1, "169.231.230.148", "/cspot-device-namespace/");
	add_host(1, "169.231.230.191", "/devices/feather-1/");
//	add_host(1, "169.231.230.191", "/devices/linux@169.231.230.148/");
//	add_host(1, "169.231.230.148", "/cspot-device-namespace/");
//	add_host(1, "169.231.230.191", "/devices/linux@pi3-10/");
	add_host(2, "169.231.230.191", "/cspot-device-namespace/");

	// ns1 will be on the device
	// ns2 will be in the cloud
	int ns1 = 1;
	int ns2 = 2;
	int n1_id = 1;
	int n2_id = 1;

	// host IDs for KS, CORR, and REGRESS respectfully
	int khost = 2;
	int kid;
	int chost = 2;
	int cid;
	int rhost = 2;
	int rid;

	int aid;

	int ossrc_id;
	int ossnk_id;
	int snk_cnt = 0;

	// if there are components on host 2, create the gather woof for the SNK node on host 2
	if((khost == 2) || (rhost == 2) || (chost == 2)) {
		if(my_host == 2) {
			err = WooFCreate("ANOMALY-OPT-HIST.state",anomaly_hist_el_size(),10);
			if(err < 0) {
				printf("anomaly-opt-setup: could not create hist state woof\n");
				exit(1);
			}
		}
	}


	// two node computes KS test on node 2
	// two node computes REGRESS test on node 2
	// two node computes CORR test on node 2
	// two node computes ANOMALY test on node 2
	if(khost == 1) { // KS is on device
		add_node(ns1, khost, n1_id, {DF_CUSTOM, KS_TEST});
		kid = n1_id;
		n1_id++;
	} else {
		add_node(ns2, khost, n2_id, {DF_CUSTOM, KS_TEST});
		kid = n2_id;
		n2_id++;
		snk_cnt++;
	}
	if(rhost == 1) {
		add_node(ns1, rhost, n1_id, {DF_CUSTOM, REGRESS_TEST});
		rid = n1_id;
		n1_id++;
	} else {
		add_node(ns2, rhost, n2_id, {DF_CUSTOM, REGRESS_TEST});
		rid = n2_id;
		n2_id++;
		snk_cnt++;
	}
	if(chost == 1) {
		add_node(ns1, chost, n1_id, {DF_CUSTOM, CORR_TEST});
		cid = n1_id;
		n1_id++;
	} else {
		add_node(ns2, chost, n2_id, {DF_CUSTOM, CORR_TEST});
		cid = n2_id;
		n2_id++;
		snk_cnt++;
	}
	add_node(ns2, 2, n2_id, {DF_CUSTOM, ANOMALY_TEST}); // always on host 2
	aid = n2_id;
	n2_id++;

	if((khost == 2) || (rhost == 2) || (chost == 2)) {
		add_node(ns1,1,n1_id,{DF_CUSTOM, ANOMALY_OPT_SRC}); // proxy src
printf("src: %d added in ns1\n",n1_id);
		ossrc_id = n1_id;
		n1_id++;
		add_node(ns2,2,n2_id,{DF_CUSTOM, ANOMALY_OPT_SNK}); // proxy sink
printf("snk: %d added in ns2\n",n2_id);
		ossnk_id = n2_id;
		n2_id++;
	}


	// operands come from node 1 -- start with next id for host 1
	// use op_node variable to give the virtual node ids
	// note that virtual graph boundary nodes must begin
	// 1 higher than actual nodes
	int op_id=n1_id;
	int op2_id=n2_id;
	for(i=0; i < duty_cycle_count; i++) {
		add_operand(ns1, 1, n1_id); // value from graph boundary on host 1

		// if there is a test on host 2, set up the proxy
		// set it up only once
		if((khost == 2) || (rhost == 2) || (chost == 2)) {
			add_operand(ns2,2,n2_id); // create shadow operand in ns2
			subscribe(ns1, ossrc_id, i, ns1, n1_id); // src gathers opnds in ns1
printf("src: %d in ns1 suscribed to %d in ns1\n",ossrc_id,n1_id);
		}
	/*
	 * subscribe node 1 (KS) node 2 (REGRESS), and node 3 (CORR) on 
	 * host 2 at port i to this operand coming from
	 * host 1 
	 */
		// if test is on source host, subscribe directly otherwise subscribe to the
		// snk at the same port
		if(khost == 1) {
			subscribe(ns1, kid, i, ns1, n1_id); // subscribe ks to opns in ns1
		} else {
			subscribe(ns2, kid, i, ns2, n2_id); // subscribe ks to boundary in ns2 (fired by snk)
		}
		if(rhost == 1) {
			subscribe(ns1, rid, i, ns1, n1_id); 
		} else {
			subscribe(ns2, rid, i, ns2, n2_id); 
		}
		if(chost == 1) {
			subscribe(ns1, cid, i, ns1, n1_id); 
		} else {
			subscribe(ns2, cid, i, ns2, n2_id); 
		}
		n1_id++;
		if((khost == 2) || (rhost == 2) || (chost == 2)) {
			n2_id++;
		}
	}

	if((khost == 2) || (rhost == 2) || (chost == 2)) {
		subscribe(ns2, ossnk_id, 0, ns1, ossrc_id); // snk in ns2 subscribes to src in ns1
printf("snk: %d in ns2 suscribed to %d in ns1\n",ossnk_id,ossrc_id);
		add_operand(ns1, 1, n1_id); // one more for the iteration number
		subscribe(ns1, ossrc_id, i, ns1, n1_id); // subscribe to the iteration number as last operand value
printf("src: %d in ns1 suscribed to %d in ns1\n",ossrc_id,n1_id);
		n1_id++;
	}

	// subscribe anomaly test node to KS (node 1) on port 0
	if(khost == 1) {
		subscribe(ns2,aid,0,ns1,kid);
	} else {
		subscribe(ns2,aid,0,ns2,kid);
	}
	// subscribe anomaly test node to REGRESS (node 2) on port 1
	if(rhost == 1) {
		subscribe(ns2,aid,1,ns1,rid);
	} else {
		subscribe(ns2,aid,1,ns2,rid);
	}
	// subscribe anomaly test node to CORR (node 3) on port 2
	if(chost == 1) {
		subscribe(ns2,aid,2,ns1,cid);
	} else {
		subscribe(ns2,aid,2,ns2,cid);
	}

	laminar_setup();
	// save starting state for body
	if((seqno == -1) || (seqno == 0)) {
		state[0] = 1; // iteration number
		state[1] = 0; // cycle number
		state[2] = op_id; // starting operand id in ns1
		state[3] = op2_id; // starting operand id in ns2
	        state[4] = aid; // anomaly detector id in ns2	
		state[5] = 2; // host_id for OPT-SNK node
		seqno = WooFPut("ANOMALY-OPT.state",NULL,(void *)state);
		if(seqno == -1) {
			printf("anomaly-opt-setup: failed to put state\n");
			exit(1);
		}
	}

	return;
}

#ifdef CSPOTDEVICE
}// extern C
#endif

#ifndef CSPOTDEVICE
int main() {
	AnomalyOptSetUp();
	return 0;
}
#endif
