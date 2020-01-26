mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: all clean clock

all:
	cd $(mkfile_dir)/ac/ && $(MAKE)
	cd $(mkfile_dir)/dfs/ && $(MAKE)
	cd $(mkfile_dir)/ll/ && $(MAKE)
	cd $(mkfile_dir)/msort/ && $(MAKE)
	cd $(mkfile_dir)/strassen/ && $(MAKE)

clean: clean_results
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

clean_results:
	-rm -rf $(mkfile_dir)/rpt/

results: implement_clock
	@echo === Figure: Operating frequency for different input sizes for data structure ===
	@cat $(mkfile_dir)/rpt/implement_clock.txt

implement_clock:
	mkdir -p $(mkfile_dir)/rpt/
	echo kernel,size,clockperiod > $(mkfile_dir)/rpt/implement_clock.txt && \
		grep "achieved post-implementation" */rpt/implement_report* | \
		sort -V | \
		sed 's/^\([^0-9/]*\)[^0-9]*\([0-9]*\).txt:[^0-9]*\([0-9.]*\)$$/\1,\2,\3/' \
		>> $(mkfile_dir)/rpt/implement_clock.txt
