mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all:
	cd $(mkfile_dir)/heterorefactor && $(MAKE) all
	cd $(mkfile_dir)/experiments/Recursive && $(MAKE) all

clean:
	cd $(mkfile_dir)/heterorefactor && $(MAKE) clean
	cd $(mkfile_dir)/experiments/Recursive && $(MAKE) clean
