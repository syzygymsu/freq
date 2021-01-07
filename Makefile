.PHONY: all rebuild build clean stop sh

BASE_IID_FILE := build/base_iid
BUILD_CID_FILE := build/build_cid

BASE_IID := `cat "$(BASE_IID_FILE)"`
BUILD_CID := `cat "$(BUILD_CID_FILE)"`

EXEC := docker exec -it -w /project $(BUILD_CID)
RUN := docker exec -i -w /project $(BUILD_CID) bazel-bin/src/freq

.PHONY: all
all: build

.PHONY: rebuild
rebuild: clean build

.PHONY: test
test: test_units test_canonical

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
		-v "$(abspath .bazelrc)":/project/.bazelrc:ro \
		-v "$(abspath bazel)":/project/bazel:ro \
		-v "$(abspath src)":/project/src:ro \
		-v "$(abspath test)":/project/test:ro \
        -v "$(abspath WORKSPACE)":/project/WORKSPACE:ro \
		$(BASE_IID)

.PHONY: sh
sh: $(BUILD_CID_FILE)
	$(EXEC) /bin/bash


.PHONY: build
build: $(BUILD_CID_FILE)
	$(EXEC) bazel build //src

.PHONY: test_units
test_units: $(BUILD_CID_FILE)
	$(EXEC) bazel test //test/... --test_output=errors

.PHONY: test_canonical
test_canonical:
	find test/samples/ -iname '*.txt' -type f -print0| xargs -0 -I{} sh -c '$(RUN) <"{}" |cmp "{}.canonical" -'

.PHONY: canonize_samples
canonize_samples: build
	find test/samples/ -iname '*.txt' -type f -print0| xargs -0 -I{} sh -c '$(RUN) <"{}" >"{}.canonical"'

.PHONY: print_run_cmd
print_run_cmd:
	@echo $(RUN)
