FROM krisnova/novix
COPY . /dname
RUN pacman --noconfirm -Syu
RUN pacman --noconfirm -S make cmake sudo
RUN pacman --noconfirm -S clang

