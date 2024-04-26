
all: dpc2sim-stream dpc2sim-ip-stride dpc2sim-next-line dpc2sim-ampm-lite dpc2sim-ghb-g-ac dpc2sim-skeleton

run: dpc2sim-ghb-g-ac
	zcat traces/mcf_trace2.dpc.gz | ./dpc2sim-ghb-ac

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
	$(CXX) -Wall -no-pie -o dpc2sim-ghb-ac example_prefetchers/ghb_g_ac_prefetcher.cc example_prefetchers/ghb.cc lib/dpc2sim.a

dpc2sim-ghb-g-dc:
	$(CXX) -Wall -no-pie -o dpc2sim-ghb-dc example_prefetchers/ghb_g_dc_prefetcher.cc example_prefetchers/ghb.cc lib/dpc2sim.a

dpc2sim-markov-one-degree:
	$(CXX) -Wall -no-pie -o dpc2sim-markov-one-degree example_prefetchers/one_degree_markov.cc lib/dpc2sim.a

dpc2sim-markov:
	$(CXX) -Wall -no-pie -o dpc2sim-markov example_prefetchers/markov.cc lib/dpc2sim.a

clean:
	rm -rf test_ghb_ac
	rm -rf test_ghb_dc
	rm -rf dpc2sim-ghb-ac
	rm -rf dpc2sim-ghb-dc
	rm -rf dpc2sim-markov
	rm -rf dpc2sim-markov-one-degree

test_ghb_ac:
	$(CXX) -Wall -o test_ghb_ac example_prefetchers/test_ghb_ac.cc example_prefetchers/ghb.cc
	./test_ghb_ac
	
test_ghb_dc:
	$(CXX) -Wall -o test_ghb_dc example_prefetchers/test_ghb_dc.cc example_prefetchers/ghb.cc
	./test_ghb_dc

.PHONY: all run clean
