mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all clean clock

all:
	cd $(mkfile_dir)/ac/ && $(MAKE)
	cd $(mkfile_dir)/dfs/ && $(MAKE)
	cd $(mkfile_dir)/ll/ && $(MAKE)
	cd $(mkfile_dir)/msort/ && $(MAKE)
	cd $(mkfile_dir)/strassen/ && $(MAKE)

clean: clean_report
	cd $(mkfile_dir)/ac/ && $(MAKE) clean
	cd $(mkfile_dir)/dfs/ && $(MAKE) clean
	cd $(mkfile_dir)/ll/ && $(MAKE) clean
	cd $(mkfile_dir)/msort/ && $(MAKE) clean
	cd $(mkfile_dir)/strassen/ && $(MAKE) clean

%:
	cd $(mkfile_dir)/ac/ && $(MAKE) $@
	cd $(mkfile_dir)/dfs/ && $(MAKE) $@
	cd $(mkfile_dir)/ll/ && $(MAKE) $@
	cd $(mkfile_dir)/msort/ && $(MAKE) $@
	cd $(mkfile_dir)/strassen/ && $(MAKE) $@

clean_report:
	-rm -rf $(mkfile_dir)/rpt/

report: implement_clock resource_after_impl
	@echo === Figure: Operating frequency for different input sizes for data structure ===
	@cat $(mkfile_dir)/rpt/implement_clock.txt
	@echo === Table: Resource utilization for HeteroRefactor ===
	@bash -c "join -j2 -o1.1,1.2,1.3,1.4,1.5,2.3 -t, \
		<(join -j2 -o1.1,1.2,1.3,1.4,2.3 -t, \
		<(join -j2 -o1.1,1.2,1.3,2.3 -t, \
		rpt/implement_lut.txt rpt/implement_ff.txt) \
		rpt/implement_bram.txt) rpt/implement_dsp.txt"

implement_clock:
	mkdir -p $(mkfile_dir)/rpt/
	cd $(mkfile_dir) && \
		echo kernel,size,clockperiod > $(mkfile_dir)/rpt/implement_clock.txt && \
		grep "achieved post-implementation" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_clock.txt

resource_after_impl:
	mkdir -p $(mkfile_dir)/rpt/
	cd $(mkfile_dir) && \
		echo kernel,size,lut > $(mkfile_dir)/rpt/implement_lut.txt && \
		grep "LUT" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_lut.txt
	cd $(mkfile_dir) && \
		echo kernel,size,ff > $(mkfile_dir)/rpt/implement_ff.txt && \
		grep "FF" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_ff.txt
	cd $(mkfile_dir) && \
		echo kernel,size,dsp > $(mkfile_dir)/rpt/implement_dsp.txt && \
		grep "DSP" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_dsp.txt
	cd $(mkfile_dir) && \
		echo kernel,size,bram > $(mkfile_dir)/rpt/implement_bram.txt && \
		grep "BRAM" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_bram.txt
