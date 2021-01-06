.PHONY: all rebuild build clean stop sh

BASE_IID_FILE := build/base_iid
BUILD_CID_FILE := build/build_cid

BASE_IID := `cat "$(BASE_IID_FILE)"`
BUILD_CID := `cat "$(BUILD_CID_FILE)"`

EXEC := docker exec -ti -w /project/src $(BUILD_CID)

all: build
rebuild: clean build

stop:
	if [ -f "$(BUILD_CID_FILE)" ]; then \
		docker container rm -f $(BUILD_CID) && \
		rm "$(BUILD_CID_FILE)"; \
	fi

clean: stop
	if [ -f "$(BASE_IID_FILE)" ]; then \
		docker image rm $(BASE_IID) && \
		rm "$(BASE_IID_FILE)"; \
	fi

$(BASE_IID_FILE):
	docker build --iidfile="$(BASE_IID_FILE)" docker/

$(BUILD_CID_FILE): $(BASE_IID_FILE)
	docker run -d --cidfile="$(BUILD_CID_FILE)" -v "$(abspath src)":/project/src $(BASE_IID)

sh: $(BUILD_CID_FILE)
	$(EXEC) /bin/bash

build: $(BUILD_CID_FILE)
	$(EXEC) bazel build //src
