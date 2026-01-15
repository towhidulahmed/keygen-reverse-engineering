# Makefile for KeygenMe3 Reverse Engineering Project

CC = gcc
CFLAGS = -Wall -Wextra -O2
BINDIR = bin
SRCDIR = src

# Target binaries
CRACKER = $(BINDIR)/cracker
MAIN = $(BINDIR)/main
VARIFY = $(BINDIR)/varify

# Source files
CRACKER_SRC = $(SRCDIR)/cracker.c
MAIN_SRC = $(SRCDIR)/main.c
VARIFY_SRC = $(SRCDIR)/varify.c

# Default target: build all
all: $(CRACKER) $(MAIN) $(VARIFY)

# Create bin directory if it doesn't exist
$(BINDIR):
	@mkdir -p $(BINDIR)

# Build the keygen
$(CRACKER): $(CRACKER_SRC) | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "✓ Keygen built: $(CRACKER)"

# Build the reimplementation
$(MAIN): $(MAIN_SRC) | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "✓ Main reimplementation built: $(MAIN)"

# Build the verification tool
$(VARIFY): $(VARIFY_SRC) | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "✓ Verification tool built: $(VARIFY)"

# Individual targets
cracker: $(CRACKER)

main: $(MAIN)

varify: $(VARIFY)

# Clean build artifacts
clean:
	rm -rf $(BINDIR)
	@echo "✓ Build artifacts cleaned"

# Rebuild everything
rebuild: clean all

# Help target
help:
	@echo "KeygenMe3 Build Targets:"
	@echo "  make all      - Build all tools (default)"
	@echo "  make cracker  - Build only the keygen"
	@echo "  make main     - Build only the reimplementation"
	@echo "  make varify   - Build only the verification tool"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make rebuild  - Clean and rebuild everything"
	@echo "  make help     - Show this help message"

.PHONY: all cracker main varify clean rebuild help
