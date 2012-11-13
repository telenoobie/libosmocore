/*
 * Test the SMC statemachine code, specially the error conditions
 *
 * (C) 2012 by Holger Hans Peter Freyther
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <osmocom/core/logging.h>
#include <osmocom/core/talloc.h>
#include <osmocom/core/msgb.h>

#include <osmocom/gsm/gsm0411_smc.h>

/*
 * Comment the below to if you do not want it to crash but just print
 * a warning.
 */
#define CRASH

struct transaction {
	struct gsm411_smc_inst smc_inst;
	int est_req;
	int deleted;	
};

static struct transaction *trans;
static int g_exit;

static int mm_send(struct gsm411_smc_inst *inst, int msg_type,
		struct msgb *msg, int cp_msg_type)
{
	if (trans->deleted)
		printf("ERROR: Sending on deleted instance.\n");

	/* respond with an establishment */
	switch (msg_type) {
	case GSM411_MMSMS_EST_REQ:
		trans->est_req = 1;
		break;
	case GSM411_MMSMS_REL_REQ:
		g_exit = 1;
		break;
	}

	msgb_free(msg);
	return 0;
}

static int mn_received(struct gsm411_smc_inst *inst, int msg_type,
			struct msgb *msg)
{
	switch (msg_type) {
	case GSM411_MNSMS_ERROR_IND:
		trans->deleted = 1;
		gsm411_smc_clear(&trans->smc_inst);
#ifdef CRASH
		talloc_free(trans);
		trans = NULL;
#endif	
		break;
	default:
		printf("Unexpected msg %d deleted %d\n",
			msg_type, trans->deleted);
		break;
	}
	return 0;
}

static void test_smc(void)
{
	struct msgb *msg;
	trans = talloc_zero(NULL, struct transaction);

	gsm411_smc_init(&trans->smc_inst, 1,
			mn_received, mm_send);

	/* reduce the timer to crash early */
	trans->smc_inst.cp_tc1 = 1;

	/* send the message.. */
	msg = msgb_alloc_headroom(1024, 124, "dummy");
	gsm411_smc_send(&trans->smc_inst, GSM411_MNSMS_EST_REQ, msg);

	/* and confirm it from the other side.. */
	if (!trans->est_req)
		printf("ERROR: The establishment should have been requested.\n");
	gsm411_smc_recv(&trans->smc_inst, GSM411_MMSMS_EST_CNF, NULL, 0);


	while (!g_exit)
		osmo_select_main(0);

	talloc_free(trans);
}

static const struct log_info log_info = {
	.num_cat = 0,
};

int main(int argc, char **argv)
{
	log_init(&log_info, NULL);
	test_smc();
}
