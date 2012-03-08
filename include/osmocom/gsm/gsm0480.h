#ifndef gsm0480_h
#define gsm0480_h

#include <osmocom/core/msgb.h>
#include <osmocom/gsm/protocol/gsm_04_08.h>
#include <osmocom/gsm/protocol/gsm_04_80.h>

#define MAX_LEN_USSD_STRING	31

struct ss_request {
	uint8_t opcode;
	uint8_t ss_code;
	uint8_t ussd_text[MAX_LEN_USSD_STRING + 1];
	uint8_t transaction_id;
	uint8_t invoke_id;
};

int gsm0480_decode_ss_request(const struct gsm48_hdr *hdr, uint16_t len,
				struct ss_request *request);

struct msgb *gsm0480_create_ussd_resp(uint8_t invoke_id, uint8_t trans_id, const char *text);
struct msgb *gsm0480_create_unstructuredSS_Notify(int alertPattern, const char *text);
struct msgb *gsm0480_create_notifySS(const char *text);

int gsm0480_wrap_invoke(struct msgb *msg, int op, int link_id);
int gsm0480_wrap_facility(struct msgb *msg);

inline unsigned char *msgb_wrap_with_TL(struct msgb *msgb, uint8_t tag);
inline unsigned char *msgb_push_TLV1(struct msgb *msgb, uint8_t tag, uint8_t value);

#endif
