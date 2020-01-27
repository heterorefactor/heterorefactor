mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all:
	cd $(mkfile_dir)/libraries && $(MAKE) all
	cd $(mkfile_dir)/refactoring && $(MAKE) all

clean:
	cd $(mkfile_dir)/libraries && $(MAKE) clean
	cd $(mkfile_dir)/refactoring && $(MAKE) clean
