mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all: $(mkfile_dir)/build/ refresh

$(mkfile_dir)/build/:
	mkdir $(mkfile_dir)/build && cd $(mkfile_dir)/build && cmake ..

refresh: $(mkfile_dir)/build/
	cd $(mkfile_dir)/build && $(MAKE)

clean:
	-rm -rf $(mkfile_dir)/build
