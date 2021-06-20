FROM krisnova/dname-builder
COPY . /dname
ENV LD_LIBRARY_PATH=/usr/local/lib
RUN cd /dname/build &&\
    rm -rf * &&\
    cmake ../ &&\
    make &&\
    make install &&\
    cd ../dname &&\
    make &&\
    make install
