# Makefile for WayCrateLock (Wrapper around CMakeLists.txt)

BUILD_DIR = build
CMAKE_FLAGS = -DCMAKE_INSTALL_PREFIX=/usr

all: setup-pam build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..

build: configure
	@cd $(BUILD_DIR) && make -j$(nproc)

install: build
	@cd $(BUILD_DIR) && sudo make install

setup-pam:
	@if [ ! -f /etc/pam.d/waycratelock ]; then \
	    echo "[INFO] Copying PAM configuration..."; \
	    sudo cp misc/waycratelock /etc/pam.d/waycratelock; \
	    echo "[SUCCESS] PAM configuration set."; \
	else \
	    echo "[INFO] PAM configuration already exists, skipping."; \
	fi

clean:
	@rm -rf $(BUILD_DIR)

reinstall: clean all setup-pam
