FROM ubuntu:18.04
RUN  apt-get update
RUN  apt-get -y install gawk git wget tar bzip2 gcc automake autoconf \
    libhpdf-dev libc6-dev autotools-dev bison flex libtool libbz2-dev \
    libpython2.7-dev ghostscript libhpdf-dev libmpfrc++-dev cmake \
    build-essential
COPY . /app
RUN  make -C /app/heterorefactor/libraries -j 16
RUN  make -C /app/heterorefactor -j 16
RUN  apt-get -y time
RUN  ln -s /app/heterorefactor/refactoring/build/heterorefactor /usr/bin
