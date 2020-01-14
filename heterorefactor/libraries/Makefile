mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all: $(mkfile_dir)/template-hls-float $(mkfile_dir)/rose-compiler/dist

$(mkfile_dir)/template-hls-float:
	git clone https://github.com/template-hls/template-hls-float.git $(mkfile_dir)/template-hls-float
	cd $(mkfile_dir)/template-hls-float && git checkout -b working 36c18856803e4e6df8c824b08bbd0ea136baf126
	cd $(mkfile_dir)/template-hls-float && git apply $(mkfile_dir)/template-hls-float.patch

$(mkfile_dir)/rose-compiler/dist:
	cd $(mkfile_dir)/rose-compiler && $(MAKE) all
