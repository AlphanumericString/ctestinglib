# TODO
## Short term
- [ ]   Add capture for stdout and stderr
- [ ]   Use testing framework to test the testing framework
- [ ]   Add single module mode (ie. run only this suite of tests)
- [ ]   Add option for NO_FORK in module flags instead of re-compiling
- [ ]   Add option for NO_COLOR in module flags instead of re-compiling
- [ ]   Clean up module flags for sub-modules (MODULE_PRINT_SBM, _LAST, ...)
- [ ]   Add alloc_fail / alloc_count for testing memory allocation failures

## Medium term
- [ ]   Add thread pool support to run in "true" parallel mode instead of
    forking processes
- [ ]   Add regex support for test names to run only a subset of tests/modules
- [ ]   Add support for test dependencies specifications (maybe via modules)
- [ ]   Add support for test fixtures (setup/teardown) per module
- [ ]   Add support for test parameterization (eg. same test but different
            inputs/outputs)
- [ ]   Add support for test timeouts

## Long term
- [ ]   Add support to "load" functions from shared libraries instead of
    compiling them in the main binary
- [ ]   Add support for other languages via hooks (eg. Python, Lua, ...)
- [ ]   Add built-in support for llvm-cov or gcov code coverage ? maybe a
            separate report step ?
