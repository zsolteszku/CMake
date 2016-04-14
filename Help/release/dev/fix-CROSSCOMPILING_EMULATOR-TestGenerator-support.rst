Fix CROSSCOMPILING_EMULATOR TestGenerator support
-------------------------------------------------

If specified, emulator is now consistently added as a prefix
to :command:`add_test` test commands for built target system
executables. It works for tests added in a parent directory.
