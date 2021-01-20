FROM ubuntu:18.04

# ======== build tools ========
RUN \
  apt update && \
  # -------- bazel --------
  apt install -y curl gnupg && \
  curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg && \
  mv bazel.gpg /etc/apt/trusted.gpg.d/ && \
  echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" > /etc/apt/sources.list.d/bazel.list && \
  apt update && apt install -y bazel && \
  bazel version

# ======== build dependencies ========
RUN apt install -y libboost-program-options-dev

# ======== testing tools ========
RUN \
  # -------- gtest --------
  apt install -y libgtest-dev cmake && \
  cd /usr/src/gtest && cmake CMakeLists.txt && make && make install && \
  # -------- python --------
  apt install -y python-pip && \
  pip install progressbar2 statistics && \
  # -------- perf --------
  #apt install -y linux-tools-generic && \ # repository is flaky today
  # -------- end --------
  true

# ======== project structure ========
RUN mkdir /project

# keep development environment running
CMD sleep infinity
