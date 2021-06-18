FROM krisnova/novix
COPY . /dname
RUN pacman --noconfirm -Syu
RUN pacman --noconfirm -S make cmake sudo
RUN pacman --noconfirm -S clang
ENV LD_LIBRARY_PATH=/usr/local/lib
