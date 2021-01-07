FROM ubuntu:18.04

RUN apt update

# global updates
RUN apt update

# install & warmup bazel
RUN \
  apt install -y curl gnupg && \
  curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg && \
  mv bazel.gpg /etc/apt/trusted.gpg.d/ && \
  echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" > /etc/apt/sources.list.d/bazel.list && \
  apt update && apt install -y bazel && \
  bazel version

# install gtest
RUN \
  apt install -y libgtest-dev cmake && \
  cd /usr/src/gtest && cmake CMakeLists.txt && make && make install

# install boost-program-options
RUN apt install -y libboost-program-options-dev

# prepare project directories
RUN mkdir /project

# keep development environment running
CMD sleep infinity
