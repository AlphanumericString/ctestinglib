# TODO
## Done
- [x]   Initial commit of the testing framework
- [x]   Basic test registration and execution
- [x]   Basic assert functions
- [x]   Basic reporting (console)
- [x]   Basic module support
- [x]   Forking support for isolating tests
- [X]   Nested module support
- [X]   Basic tests for the testing framework itself
### updates 2025-11-14
- [x]   Add capture for stdout and stderr
- [x]   Use testing framework to test the testing framework
- [x]   Alloc module
    - [x]   Add alloc failure simulation
    - [x]   Add ptr tracking for auto cleanups
    - [x]   Safe alloc + wrapper arround malloc/free
- [x]   Asserts
    - [x]   assert_mem
    - [x]   assert_str
    - [x]   assert_bool
- [x]   capture fds
- [x]   Utils
    - [x]   Utils - mem
    - [x]   Utils - str
- [X]   Refactor flags system to be more manageable and better named
    - [X]   Manageable
    - [X]   Better named
    - [X]   Documented
- [X]   Display Flags
    - [X]   Tests names (long/short)
    - [X]   Tests results (only fail/all)
    - [X]   Modules summary (...)
    - [X]   Sub Modules summary
- [x]   Clean up module flags for sub-modules (MODULE_PRINT_SBM, _LAST, ...)
- [X]   Add report output formats
    - [X]   JSON format
    - [X]   Text format
- [x]   Add support for test fixtures (setup/teardown) per module

## Short term
- [ ]   Add single module mode (ie. run only this suite of tests)
- [ ]   Add option for NO_COLOR in module flags instead of re-compiling
- [ ]   Add option for NO_FORK in module flags instead of re-compiling
- [~]   Tests and coverage
    - [~]   Add tests for utils modules
    - [~]   Add tests for core modules
    - [x]   Add code coverage support
    - [ ]   Alloc failure tests
- [ ]   Improve documentation
- [~]   Add report output formats
    - [X]   JSON format
    - [X]   Text format
    - [ ]   HTML format
    - [ ]   XML format

## Medium term
- [ ]   Add regex support for test names to run only a subset of tests/modules
- [ ]   Add support for test parameterization (eg. same test but different
            inputs/outputs)
- [ ]   Add support for test timeouts

## Long term
- [ ]   Add support to "load" functions from shared libraries instead of
    compiling them in the main binary
- [ ]   Add support for other languages via hooks (eg. Python, Lua, ...)
- [ ]   Add built-in support for llvm-cov or gcov code coverage ? maybe a
            separate report step ?
