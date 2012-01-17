if ENABLE_UTILITIES
AM_CFLAGS = -Wall $(all_includes) -I$(top_srcdir)/include

bin_PROGRAMS = osmo-arfcn osmo-auc-gen
noinst_PROGRAMS = osmo-sim-test

osmo_arfcn_SOURCES = osmo-arfcn.c
osmo_arfcn_LDADD = $(top_builddir)/src/libosmocore.la $(top_builddir)/src/gsm/libosmogsm.la

osmo_auc_gen_SOURCES = osmo-auc-gen.c
osmo_auc_gen_LDADD = $(top_builddir)/src/libosmocore.la $(top_builddir)/src/gsm/libosmogsm.la

osmo_sim_test_SOURCES = osmo-sim-test.c
osmo_sim_test_LDADD = $(top_builddir)/src/libosmocore.la $(top_builddir)/src/gsm/libosmogsm.la $(top_builddir)/src/sim/libosmosim.la -lpcsclite
osmo_sim_test_CFLAGS = -I/usr/include/PCSC
endif
