# The launcher's own gate is verify_launcher.py (not pytest/npm); this target exists so tooling
# that looks for a Makefile can find and run it. `make test` = the canonical command.
PY ?= .venv/Scripts/python.exe

.PHONY: test compile

test:
	$(PY) verify_launcher.py

compile:
	$(PY) -m compileall -q main.py verify_launcher.py dmw3launcher
