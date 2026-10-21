# ED08 governed R4 integration harness

Test-only MSVC19.51 x64 /MD /W4 /WX. Separate normal and TEST_ONLY library targets. No PST rebuild or production C2 wire. Uses byte-exact Connection/manager and transport components copied from the governed isolated API3 tree; the adapter and publication guard are isolated fixture composition. Process exit contains retained resources and is never destruction evidence.
