# TODO
## Short term
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
- [~]   Tests and coverage
    - [~]   Add tests for utils modules
    - [~]   Add tests for core modules
    - [x]   Add code coverage support
    - [ ]   Alloc failure tests
- [x]   Clean up module flags for sub-modules (MODULE_PRINT_SBM, _LAST, ...)
    - [ ]   Add json format
- [ ]   Add single module mode (ie. run only this suite of tests)
- [ ]   Add option for NO_COLOR in module flags instead of re-compiling
- [ ]   Add option for NO_FORK in module flags instead of re-compiling

## Medium term
- [ ]   Add thread pool support to run in "true" parallel mode instead of
    forking processes
- [ ]   Add regex support for test names to run only a subset of tests/modules
- [ ]   Add support for test dependencies specifications (maybe via modules)
- [x]   Add support for test fixtures (setup/teardown) per module
- [ ]   Add support for test parameterization (eg. same test but different
            inputs/outputs)
- [ ]   Add support for test timeouts

## Long term
- [ ]   Add support to "load" functions from shared libraries instead of
    compiling them in the main binary
- [ ]   Add support for other languages via hooks (eg. Python, Lua, ...)
- [ ]   Add built-in support for llvm-cov or gcov code coverage ? maybe a
            separate report step ?
