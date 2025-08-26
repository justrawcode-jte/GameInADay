GAME_DIR?=""

setup-project:
	@ cmake -S ./internal -B ./internal/build -DGAME_DIR=${GAME_DIR}

compile-deps:
	cd ./internal; sh ./bash/compile_deps.sh

compile-project:
	@ cmake --build ./internal/build

install-project:
	@ cmake --install ./internal/build --prefix ${GAME_DIR}/install

run-project:
	@ ${GAME_DIR}/install/bin/app