BASE_IID_FILE := build/base_iid
BUILD_CID_FILE := build/build_cid

BASE_IID := `cat "$(BASE_IID_FILE)"`
BUILD_CID := `cat "$(BUILD_CID_FILE)"`

EXEC := docker exec -i -w /project $(BUILD_CID)
EXECT := docker exec -it -w /project $(BUILD_CID)
RUN_FREQ := $(EXEC) bazel run //src

.PHONY: all
all: build

.PHONY: rebuild
rebuild: clean build

.PHONY: test
test: build test_units test_canonical test_hash_attack
	@echo "\033[0;32m================\033[1;32m ALL TESTS PASSED \033[0;32m================\033[0m"

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
		--cap-add SYS_ADMIN \
		-v "$(abspath .bazelrc)":/project/.bazelrc:ro \
		-v "$(abspath bazel)":/project/bazel:ro \
		-v "$(abspath src)":/project/src:ro \
		-v "$(abspath test)":/project/test:ro \
        -v "$(abspath WORKSPACE)":/project/WORKSPACE:ro \
		$(BASE_IID)

.PHONY: sh
sh: $(BUILD_CID_FILE)
	$(EXECT) /bin/bash

.PHONY: build
build: $(BUILD_CID_FILE)
	$(EXEC) bazel build //src

.PHONY: test_units
test_units: $(BUILD_CID_FILE)
	$(EXEC) bazel test //test:unittests --test_output=errors

.PHONY: test_canonical
test_canonical:
	find test/samples/ -iname '*.txt' -type f -print0| xargs -0 -I{} sh -c '$(RUN_FREQ) <"{}" |cmp "{}.canonical" -'

.PHONY: test_hash_attack
test_hash_attack: $(BUILD_CID_FILE)
	$(EXEC) bazel run //test:find_hash_collisions -- -c1000 | $(EXEC) bazel run //test:test_hash_collisions -- -c1000 -r15
	$(EXEC) bazel run //test:find_hash_collisions -- -c3000 | $(EXEC) bazel run //test:test_hash_collisions -- -c3000 -r40

build/big_hash_attack.txt:
	$(EXEC) bazel run //test:find_hash_collisions > build/big_hash_attack.txt

.PHONY: test_hash_attack_long
test_hash_attack_long: $(BUILD_CID_FILE) build/big_hash_attack.txt
	cat build/big_hash_attack.txt | $(EXEC) bazel run //test:test_hash_collisions -- -r1000

.PHONY: canonize_samples
canonize_samples: build
	find test/samples/ -iname '*.txt' -type f -print0| xargs -0 -I{} sh -c '$(RUN_FREQ) <"{}" >"{}.canonical"'

.PHONY: print_run_cmd
print_run_cmd:
	@echo $(RUN_FREQ)

.PHONY: perf
perf: build
	$(EXECT) test/perf.py
