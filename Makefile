# The launcher's own gate is tools/verify_launcher.py (not pytest/npm); this target exists so tooling
# that looks for a Makefile can find and run it. `make test` = the canonical command.
PY ?= .venv/Scripts/python.exe

.PHONY: test compile

test:
	$(PY) tools/verify_launcher.py

compile:
	$(PY) -m compileall -q main.py tools/verify_launcher.py dmw3launcher
