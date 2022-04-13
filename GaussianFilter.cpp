#include <cmath>
#ifndef NATIVE_SYSTEMC
#include "stratus_hls.h"
#endif

#include "GaussianFilter.h"

GaussianFilter::GaussianFilter( sc_module_name n ): sc_module( n )
{
#ifndef NATIVE_SYSTEMC
	HLS_FLATTEN_ARRAY(val);
#endif
	SC_THREAD( do_filter );
	sensitive << i_clk.pos();
	dont_initialize();
	reset_signal_is(i_rst, false);
        
#ifndef NATIVE_SYSTEMC
	i_rgb.clk_rst(i_clk, i_rst);
  o_result.clk_rst(i_clk, i_rst);
#endif
}

GaussianFilter::~GaussianFilter() {}

// gaussian mask
const int mask[MASK_X][MASK_Y] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

void GaussianFilter::do_filter() {
	{
#ifndef NATIVE_SYSTEMC
		HLS_DEFINE_PROTOCOL("main_reset");
		i_rgb.reset();
		o_result.reset();
#endif
		wait();
	}
	while (true) {
		sc_dt::sc_uint<24> rgb;
		#if defined(II)
			HLS_PIPELINE_LOOP( SOFT_STALL, II, "main_loop_pipeline" ); 
		#endif
		int tmp_r = 0;
    int tmp_b = 0;
    int tmp_g = 0;	
		for (unsigned int v = 0; v<MASK_Y; ++v) {
			//HLS_UNROLL_LOOP( ON, "CAL" );
			for (unsigned int u = 0; u<MASK_X; ++u) {
#ifndef NATIVE_SYSTEMC
				{
					HLS_DEFINE_PROTOCOL("input");
					rgb = i_rgb.get();
					wait();
				}
#else
				rgb = i_rgb.read();
#endif
				#ifdef UNROLL
				#   define CAL_LOOP  HLS_UNROLL_LOOP( ON, "CAL" )
				#endif
				#ifndef UNROLL
				#   define CAL_LOOP
				#endif
				CAL_LOOP;
				//HLS_UNROLL_LOOP( ON, "CAL" );
				HLS_CONSTRAIN_LATENCY(0, 3, "lat01");
			
				tmp_r += rgb.range(7, 0) * mask[u][v];
				tmp_g += rgb.range(15, 8) * mask[u][v];
				tmp_b += rgb.range(23, 16) * mask[u][v];
			}
		}
		tmp_r /= 16;
		tmp_g /= 16;
		tmp_b /= 16;
		rgb.range(7, 0) = tmp_r;
		rgb.range(15, 8) = tmp_g;
		rgb.range(23, 16) = tmp_b;

#ifndef NATIVE_SYSTEMC
		{
			HLS_DEFINE_PROTOCOL("output");
			o_result.put(rgb);
			wait();
		}
#else
		o_result.write(rgb);
#endif
	}
}
