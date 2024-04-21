
all: dpc2sim-stream dpc2sim-ip-stride dpc2sim-next-line dpc2sim-ampm-lite dpc2sim-ghb-g-ac dpc2sim-skeleton

run: dpc2sim-ghb-g-ac
	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-ghb -simulation_instructions 100 -warmup_instructions 0

dpc2sim-stream:
	$(CXX) -Wall -no-pie -o dpc2sim-stream example_prefetchers/stream_prefetcher.cc lib/dpc2sim.a

dpc2sim-ip-stride:
	$(CXX) -Wall -no-pie -o dpc2sim-stride example_prefetchers/ip_stride_prefetcher.cc lib/dpc2sim.a

dpc2sim-next-line:
	$(CXX) -Wall -no-pie -o dpc2sim-next example_prefetchers/next_line_prefetcher.cc lib/dpc2sim.a

dpc2sim-ampm-lite:
	$(CXX) -Wall -no-pie -o dpc2sim-ampm example_prefetchers/ampm_lite_prefetcher.cc lib/dpc2sim.a

dpc2sim-skeleton:
	$(CXX) -Wall -no-pie -o dpc2sim-skeleton example_prefetchers/skeleton.cc lib/dpc2sim.a

dpc2sim-ghb-g-ac:
	$(CXX) -Wall -no-pie -o dpc2sim-ghb example_prefetchers/ghb_g_ac_prefetcher.cc example_prefetchers/ghb.cc lib/dpc2sim.a

clean:
	rm -rf dpc2sim-ghb

test:
	$(CXX) -Wall -o test_ghb example_prefetchers/test_ghb.cc example_prefetchers/ghb.cc

.PHONY: all run clean
