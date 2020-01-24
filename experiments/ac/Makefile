mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
instrumentation_profile := /tmp/hetero-profile

all: invariant runtime

invariant: $(mkfile_dir)/invariant
runtime: $(mkfile_dir)/invariant $(mkfile_dir)/rpt/original_run_time_1024.txt
compiletime: $(mkfile_dir)/bin/original.exe $(mkfile_dir)/bin/instrument.exe

$(mkfile_dir)/invariant: \
		$(mkfile_dir)/bin/instrument.exe \
		$(mkfile_dir)/testdata/data_generator.sh
	mkdir -p $(mkfile_dir)/tmp
	mkdir -p $(mkfile_dir)/rpt
	mkdir -p $(mkfile_dir)/invariant
	for size in 1024 2048 4096 8192 16384 32768 ; do \
		$(mkfile_dir)/testdata/data_generator.sh $$((size-26)) |  \
			time -o $(mkfile_dir)/rpt/instrument_run_time_$${size}.txt \
			$(mkfile_dir)/bin/instrument.exe > \
			$(mkfile_dir)/tmp/instrument_output_$${size}.txt ; \
		awk -f $(mkfile_dir)/../../heterorefactor/instrumentation/recursive/generate_invariant.awk \
			$(instrumentation_profile) \
			> $(mkfile_dir)/invariant/invariant_$${size}.ivr ; \
	done

$(mkfile_dir)/rpt/original_run_time_1024.txt: \
		$(mkfile_dir)/bin/original.exe \
		$(mkfile_dir)/testdata/data_generator.sh
	mkdir -p $(mkfile_dir)/tmp
	mkdir -p $(mkfile_dir)/rpt
	mkdir -p $(mkfile_dir)/invariant
	for size in 1024 2048 4096 8192 16384 32768 ; do \
		$(mkfile_dir)/testdata/data_generator.sh $$((size-26)) |  \
			time -o $(mkfile_dir)/rpt/original_run_time_$${size}.txt \
			$(mkfile_dir)/bin/original.exe > \
			$(mkfile_dir)/tmp/original_output_$${size}.txt ; \
	done

$(mkfile_dir)/bin/original.exe: \
		$(mkfile_dir)/src/kernel.cpp \
		$(mkfile_dir)/src/testbed.cpp
	mkdir -p $(mkfile_dir)/bin
	mkdir -p $(mkfile_dir)/rpt
	time -o $(mkfile_dir)/rpt/original_compilation.txt \
		g++ -o $@ $^

$(mkfile_dir)/bin/instrument.exe: \
		$(mkfile_dir)/refactored/kernel_instrument.cpp \
		$(mkfile_dir)/src/testbed.cpp
	mkdir -p $(mkfile_dir)/bin
	mkdir -p $(mkfile_dir)/rpt
	time -o $(mkfile_dir)/rpt/instrument_compilation.txt \
		g++ -o $@ $^

$(mkfile_dir)/refactored/kernel_instrument.cpp: $(mkfile_dir)/src/kernel.cpp
	mkdir -p $(mkfile_dir)/rpt
	time -o $(mkfile_dir)/rpt/instrument_refactoring_time.txt \
		$(mkfile_dir)/../../heterorefactor/refactoring/build/heterorefactor \
		-instrument $(instrumentation_profile) \
		-u $@ $^

clean:
	-rm -f $(mkfile_dir)/refactored/kernel_instrument.cpp
	-rm -f $(mkfile_dir)/refactored/kernel_refactored.cpp
	-rm -rf $(mkfile_dir)/invariant
	-rm -rf $(mkfile_dir)/bin
	-rm -rf $(mkfile_dir)/tmp
