mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
instrumentation_profile := /tmp/hetero-profile

all: $(mkfile_dir)/invariant

$(mkfile_dir)/invariant: \
		$(mkfile_dir)/bin/instrument.exe \
		$(mkfile_dir)/testdata/data_generator.sh
	mkdir -p $(mkfile_dir)/tmp
	mkdir -p $(mkfile_dir)/invariant
	for size in 1024 2048 4096 8192 16384 32768 ; do \
		$(mkfile_dir)/testdata/data_generator.sh $$((size-26)) |  \
			$(mkfile_dir)/bin/instrument.exe > \
			$(mkfile_dir)/tmp/output_$${size}.txt ; \
		awk -f $(mkfile_dir)/../../heterorefactor/instrumentation/recursive/generate_invariant.awk \
			$(instrumentation_profile) \
			> $(mkfile_dir)/invariant/invariant_$${size}.ivr ; \
	done

$(mkfile_dir)/bin/instrument.exe: \
		$(mkfile_dir)/refactored/kernel_instrument.cpp \
		$(mkfile_dir)/src/testbed.cpp
	mkdir -p $(mkfile_dir)/bin
	g++ -o $(mkfile_dir)/bin/instrument.exe \
		$(mkfile_dir)/refactored/kernel_instrument.cpp \
		$(mkfile_dir)/src/testbed.cpp

$(mkfile_dir)/refactored/kernel_instrument.cpp: $(mkfile_dir)/src/kernel.cpp
	$(mkfile_dir)/../../heterorefactor/refactoring/build/heterorefactor \
		-instrument $(instrumentation_profile) \
		-u $(mkfile_dir)/refactored/kernel_instrument.cpp \
		$(mkfile_dir)/src/kernel.cpp

clean:
	-rm -f $(mkfile_dir)/refactored/kernel_instrument.cpp
	-rm -f $(mkfile_dir)/refactored/kernel_refactored.cpp
	-rm -rf $(mkfile_dir)/invariant
	-rm -rf $(mkfile_dir)/bin
	-rm -rf $(mkfile_dir)/tmp
