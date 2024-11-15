#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef ESP8266
typedef struct woof_stc WOOF;

int WooFInit();
void WooFExit();
#endif

int WooFCreate(const char* name, unsigned long element_size, unsigned long history_size);

unsigned long WooFPut(const char* wf_name, const char* wf_handler, const void* element);
int WooFGet(const char* wf_name, void* element, unsigned long seq_no);

unsigned long WooFGetLatestSeqno(const char* wf_name);
unsigned long WooFGetLatestSeqnoWithCause(const char* wf_name,
                                          unsigned long cause_host,
                                          unsigned long long cause_seq_no,
                                          const char* cause_woof_name,
                                          unsigned long cause_woof_latest_seq_no);
int WooFInvalid(unsigned long seq_no);

#ifndef ESP8266
unsigned long WooFGetNameID();


int WooFValidURI(const char* str);

int WooFLocalIP(char* ip_str, int len);
#endif

#define WOOFNAMESIZE (256)
unsigned long WooFGetElSize(WOOF* wf, const char* wf_name);

const char* WoofGetFileName(WOOF* wf);
#define WooFGetFileName WoofGetFileName


#if defined(__cplusplus)
}
#endif
