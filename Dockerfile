FROM ubuntu:18.04

RUN apt update

# install bazel
RUN \
  apt update && apt install -y curl gnupg && \
  curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg && \
  mv bazel.gpg /etc/apt/trusted.gpg.d/ && \
  echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" > /etc/apt/sources.list.d/bazel.list && \
  apt update && apt install -y bazel

# install gtest
RUN apt install -y libgtest-dev cmake
RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && make install

# prepare project directories
RUN mkdir /project

# do not stop development environment
CMD sleep infinity
