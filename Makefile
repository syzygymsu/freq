.PHONY: all rebuild build clean stop sh

BASE_IID_FILE := build/base_iid
BUILD_CID_FILE := build/build_cid

BASE_IID := `cat "$(BASE_IID_FILE)"`
BUILD_CID := `cat "$(BUILD_CID_FILE)"`

EXEC := docker exec -ti -w /project $(BUILD_CID)

.PHONY: all
all: build

.PHONY: rebuild
rebuild: clean build

.PHONY: test
test: unittests

.PHONY: stop
stop:
	if [ -f "$(BUILD_CID_FILE)" ]; then \
		docker container rm -f $(BUILD_CID) && \
		rm "$(BUILD_CID_FILE)"; \
	fi

.PHONY: stop
clean: stop
	if [ -f "$(BASE_IID_FILE)" ]; then \
		docker image rm $(BASE_IID) && \
		rm "$(BASE_IID_FILE)"; \
	fi

$(BASE_IID_FILE):
	docker build --iidfile="$(BASE_IID_FILE)" .

$(BUILD_CID_FILE): $(BASE_IID_FILE)
	docker run -d --cidfile="$(BUILD_CID_FILE)" \
		-v "$(abspath src)":/project/src:ro \
		-v "$(abspath bazel)":/project/bazel:ro \
        -v "$(abspath WORKSPACE)":/project/WORKSPACE:ro \
		-v "$(abspath .bazelrc)":/project/.bazelrc:ro \
		$(BASE_IID)

.PHONY: sh
sh: $(BUILD_CID_FILE)
	$(EXEC) /bin/bash


.PHONY: build
build: $(BUILD_CID_FILE)
	$(EXEC) bazel build //src

.PHONY: unittests
unittests: $(BUILD_CID_FILE)
	$(EXEC) bazel test //src:unittests --test_output=errors
