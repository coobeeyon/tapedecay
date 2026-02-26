/*
 * tapedecay.c — LADSPA plugin simulating cassette tape decay
 *
 * Moves each sample toward zero by a fixed amount, then rescales.
 * Quiet details are eaten first; louder parts survive.
 */

#include <stdlib.h>
#include <string.h>
#include "ladspa.h"

#define TD_DECAY    0
#define TD_INPUT_L  1
#define TD_INPUT_R  2
#define TD_OUTPUT_L 3
#define TD_OUTPUT_R 4
#define TD_NPORTS   5

typedef struct {
	LADSPA_Data *ports[TD_NPORTS];
} TapeDecay;

static LADSPA_Handle td_instantiate(const LADSPA_Descriptor *desc,
                                    unsigned long rate)
{
	(void)desc;
	(void)rate;
	TapeDecay *td = calloc(1, sizeof(*td));
	return (LADSPA_Handle)td;
}

static void td_connect_port(LADSPA_Handle instance, unsigned long port,
                            LADSPA_Data *data)
{
	TapeDecay *td = (TapeDecay *)instance;
	if (port < TD_NPORTS)
		td->ports[port] = data;
}

static void td_run(LADSPA_Handle instance, unsigned long n)
{
	TapeDecay *td = (TapeDecay *)instance;

	const LADSPA_Data decay = *td->ports[TD_DECAY];
	const LADSPA_Data *in_l = td->ports[TD_INPUT_L];
	const LADSPA_Data *in_r = td->ports[TD_INPUT_R];
	LADSPA_Data *out_l = td->ports[TD_OUTPUT_L];
	LADSPA_Data *out_r = td->ports[TD_OUTPUT_R];

	const LADSPA_Data scale = 1.0f / (1.0f - decay);

	for (unsigned long i = 0; i < n; i++) {
		LADSPA_Data sl = in_l[i];
		if (sl > 0.0f) {
			sl -= decay;
			if (sl < 0.0f) sl = 0.0f;
		} else if (sl < 0.0f) {
			sl += decay;
			if (sl > 0.0f) sl = 0.0f;
		}
		out_l[i] = sl * scale;

		LADSPA_Data sr = in_r[i];
		if (sr > 0.0f) {
			sr -= decay;
			if (sr < 0.0f) sr = 0.0f;
		} else if (sr < 0.0f) {
			sr += decay;
			if (sr > 0.0f) sr = 0.0f;
		}
		out_r[i] = sr * scale;
	}
}

static void td_cleanup(LADSPA_Handle instance)
{
	free(instance);
}

static const char *port_names[TD_NPORTS] = {
	"Decay", "Input L", "Input R", "Output L", "Output R"
};

static const LADSPA_PortDescriptor port_descriptors[TD_NPORTS] = {
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL,
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO,
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO,
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO,
};

static const LADSPA_PortRangeHint port_hints[TD_NPORTS] = {
	{ LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE
	  | LADSPA_HINT_DEFAULT_LOW,
	  0.0f, 0.99f },
	{ 0, 0.0f, 0.0f },
	{ 0, 0.0f, 0.0f },
	{ 0, 0.0f, 0.0f },
	{ 0, 0.0f, 0.0f },
};

static const LADSPA_Descriptor descriptor = {
	.UniqueID        = 9301,
	.Label           = "tapedecay",
	.Properties      = LADSPA_PROPERTY_HARD_RT_CAPABLE,
	.Name            = "Tape Decay",
	.Maker           = "coobeeyon",
	.Copyright       = "ISC",
	.PortCount       = TD_NPORTS,
	.PortDescriptors = port_descriptors,
	.PortNames       = port_names,
	.PortRangeHints  = port_hints,
	.instantiate     = td_instantiate,
	.connect_port    = td_connect_port,
	.run             = td_run,
	.cleanup         = td_cleanup,
};

const LADSPA_Descriptor *ladspa_descriptor(unsigned long index)
{
	return index == 0 ? &descriptor : NULL;
}
