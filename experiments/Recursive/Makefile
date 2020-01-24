mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all:
	cd $(mkfile_dir)/ac/ && $(MAKE)
	cd $(mkfile_dir)/dfs/ && $(MAKE)
	cd $(mkfile_dir)/ll/ && $(MAKE)
	cd $(mkfile_dir)/msort/ && $(MAKE)
	cd $(mkfile_dir)/strassen/ && $(MAKE)

%:
	cd $(mkfile_dir)/ac/ && $(MAKE) $@
	cd $(mkfile_dir)/dfs/ && $(MAKE) $@
	cd $(mkfile_dir)/ll/ && $(MAKE) $@
	cd $(mkfile_dir)/msort/ && $(MAKE) $@
	cd $(mkfile_dir)/strassen/ && $(MAKE) $@
