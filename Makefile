# ===================================
# Configuration
# ===================================

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Iinclude
LDFLAGS ?= -lz -lSDL2 -lSDL2main -lSDL2_image -lfmt
OUT_DIR := bin
SRC := main.cpp

# ===================================
# Native build
# ===================================

main:
	@rm -rf bin
	@mkdir -p $(OUT_DIR)
	@cp -r assets/* $(OUT_DIR)/ 2>/dev/null || true
	@$(CXX) $(SRC) -o $(OUT_DIR)/main.o $(CXXFLAGS) -fmodules-ts $(LDFLAGS) 
	@chmod +x $(OUT_DIR)/main.o
	@echo "✅ Built native binary -> $(OUT_DIR)/main.o"

test:
	@mkdir -p $(OUT_DIR)
	@cp -r assets/* $(OUT_DIR)/ 2>/dev/null || true
	@$(CXX) "tests/main.cpp" -o $(OUT_DIR)/tests.o $(CXXFLAGS) -fmodules-ts $(LDFLAGS) 
	@chmod +x $(OUT_DIR)/tests.o
	@echo "✅ Built native binary -> $(OUT_DIR)/tests.o"
	@./$(OUT_DIR)/tests.o

run: main
	@./$(OUT_DIR)/main.o


# ===================================
# WebAssembly build (Emscripten)
# ===================================

main-web:
	@rm -rf bin
	@mkdir -p $(OUT_DIR)/web
	@echo "🌐 Building WebAssembly target..."
	@EM_CACHE=$(HOME)/.emscripten_cache \
	FROZEN_CACHE=0 \
	em++ main.cpp -Iinclude -o bin/web/index.html $(CXXFLAGS) \
		-s USE_SDL=2 \
		-s USE_SDL_IMAGE=2 \
		-s SDL2_IMAGE_FORMATS='["png"]' \
		--preload-file assets@/ \
		-sASSERTIONS \
		-O2

	@echo "✅ Web build complete -> $(OUT_DIR)/web/index.html"
