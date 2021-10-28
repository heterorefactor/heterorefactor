.NOTPARALLEL: $(mkfile_dir)/build/boost

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

all: $(mkfile_dir)/dist

$(mkfile_dir)/source:
	git clone -b release https://github.com/rose-compiler/rose $(mkfile_dir)/source
	cd $(mkfile_dir)/source && git checkout dab37577feb8eb129c8fc15f6972222c03171c9f
	cd $(mkfile_dir)/source && ./build

$(mkfile_dir)/build/rose: $(mkfile_dir)/source $(mkfile_dir)/build/boost
	mkdir -p $(mkfile_dir)/build/rose
	cd $(mkfile_dir)/build/rose && $(mkfile_dir)/source/configure --enable-static --prefix=$(mkfile_dir)/dist \
		--enable-languages=c,c++ --with-boost=$(mkfile_dir)/build/boost --without-java --enable-edg_version=5.0

$(mkfile_dir)/dist: $(mkfile_dir)/build/rose
	cd $(mkfile_dir)/build/rose && $(MAKE) && $(MAKE) install

$(mkfile_dir)/boost:
	wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.bz2 -O $(mkfile_dir)/boost.tar.bz2
	cd $(mkfile_dir) && tar -xjf boost.tar.bz2
	cd $(mkfile_dir) && mv boost_1_58_0 boost && rm boost.tar.bz2

$(mkfile_dir)/build/boost: $(mkfile_dir)/boost
	cd $(mkfile_dir)/boost && ./bootstrap.sh --prefix=$(mkfile_dir)/build/boost
	cd $(mkfile_dir)/boost && ./b2 -j 16 && ./b2 install

clean:
	-rm -rf $(mkfile_dir)/source
	-rm -rf $(mkfile_dir)/boost
	-rm -rf $(mkfile_dir)/build
	-rm -rf $(mkfile_dir)/dist
