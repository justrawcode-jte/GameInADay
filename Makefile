GAME_DIR?=""

setup-project:
	cmake -S ./internal -B ./internal/build -DGAME_DIR=${GAME_DIR}

compile-project:
	cmake --build ./internal/build

install-project:
	cmake --install ./internal/build --prefix ${GAME_DIR}/install

run-project:
	${GAME_DIR}/install/bin/app