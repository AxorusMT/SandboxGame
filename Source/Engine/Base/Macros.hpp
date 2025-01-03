/*

This engine uses its own unique format for macros, depending on the type of macro:

Feature flags:
    Feature flag macros are in ALL_CAPS, and have FLAG_ before them, e.g.
        FLAG_IS_LINUX
        FLAG_IS_32_BIT
        FLAG_IS_DEBUG
        FLAG_USE_EXCEPTIONS

Functions:
    Many function macros deviate heavvily from normal convention, and use a system known as "dollar macros",
    where the macro is in the form:

        $PascalCaseName(arg1, arg2, ..., argn)

    e.g.
        $ErrorIf(string)
        $TestCase(name)
        $Trace()

All Feature flag macros should be undefined before their original defintion

Macros should only be used when necessary, and not randomly. Prefer C++ over macros
*/




