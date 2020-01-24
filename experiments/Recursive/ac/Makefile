testdata := 1024 2048 4096 8192 16384 32768


mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
hrf_dir := $(mkfile_dir)/../../../heterorefactor/
instrumentation_profile := /tmp/hetero-profile

# Contention on $(instrumentation_profile) file, disallow parallelization
.NOTPARALLEL:
.PHONY: all clean invariant refactor runtime original_runtime compiletime

all: invariant refactor runtime

invariant: $(addprefix $(mkfile_dir)/invariant/invariant_, $(addsuffix .ivr, $(testdata)))
refactor: $(addprefix $(mkfile_dir)/refactored/kernel_device_, $(addsuffix .cpp, $(testdata)))
runtime: invariant original_runtime

original_runtime: $(addprefix $(mkfile_dir)/rpt/original_run_time_, $(addsuffix .txt, $(testdata)))
compiletime: $(mkfile_dir)/bin/original.exe $(mkfile_dir)/bin/instrument.exe

$(addprefix $(mkfile_dir)/invariant/invariant_, $(addsuffix .ivr, $(testdata))): \
		$(mkfile_dir)/bin/instrument.exe \
		$(mkfile_dir)/testdata/data_generator.sh
	mkdir -p $(mkfile_dir)/tmp
	mkdir -p $(mkfile_dir)/rpt
	mkdir -p $(mkfile_dir)/invariant
	$(eval @_base := $(basename $(notdir $@)))
	$(eval @_tmp := $(@_base:%.ivr=%))
	$(eval @_size := $(@_tmp:invariant_%=%))
	$(mkfile_dir)/testdata/data_generator.sh $(@_size) |  \
		time -o $(mkfile_dir)/rpt/instrument_run_time_$(@_size).txt \
		$(mkfile_dir)/bin/instrument.exe > \
		$(mkfile_dir)/tmp/instrument_output_$(@_size).txt ; \
	awk -f $(hrf_dir)/instrumentation/recursive/generate_invariant.awk \
		$(instrumentation_profile) \
		> $(mkfile_dir)/invariant/invariant_$(@_size).ivr ; \

$(addprefix $(mkfile_dir)/rpt/original_run_time_, $(addsuffix .txt, $(testdata))): \
		$(mkfile_dir)/bin/original.exe \
		$(mkfile_dir)/testdata/data_generator.sh
	mkdir -p $(mkfile_dir)/tmp
	mkdir -p $(mkfile_dir)/rpt
	mkdir -p $(mkfile_dir)/invariant
	$(eval @_base := $(basename $(notdir $@)))
	$(eval @_tmp := $(@_base:%.txt=%))
	$(eval @_size := $(@_tmp:original_run_time_%=%))
	$(mkfile_dir)/testdata/data_generator.sh $(@_size) |  \
		time -o $(mkfile_dir)/rpt/original_run_time_$(@_size).txt \
		$(mkfile_dir)/bin/original.exe > \
		$(mkfile_dir)/tmp/original_output_$(@_size).txt ; \

$(addprefix $(mkfile_dir)/refactored/kernel_device_, $(addsuffix .cpp, $(testdata))): \
		$(mkfile_dir)/src/kernel.cpp invariant
	mkdir -p $(mkfile_dir)/rpt
	$(eval @_base := $(basename $(notdir $@)))
	$(eval @_tmp := $(@_base:%.cpp=%))
	$(eval @_size := $(@_tmp:kernel_device_%=%))
	time -o $(mkfile_dir)/rpt/refactoring_time_$(@_size).txt \
		$(hrf_dir)/refactoring/build/heterorefactor \
		-rec -recfile $(mkfile_dir)/invariant/invariant_$(@_size).ivr \
		-u $@ $<

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
		$(hrf_dir)/refactoring/build/heterorefactor \
		-instrument $(instrumentation_profile) \
		-u $@ $^

clean:
	-rm -f $(mkfile_dir)/refactored/kernel_instrument.cpp
	-rm -f $(mkfile_dir)/refactored/kernel_refactored.cpp
	-rm -rf $(mkfile_dir)/invariant
	-rm -rf $(mkfile_dir)/bin
	-rm -rf $(mkfile_dir)/tmp
	-rm -rf $(mkfile_dir)/rpt
