//
// Created by Michael Lynch on 11/01/2025.
//

#include <stdbool.h>
#include <stdio.h>
#include "executor.h"
#include "../../inc/executor.h"
// #include "../../inc/executor.h"

#define ASSERT(x) { bool b = x; if (!b) { return false; } }
#define RUN_TEST(test) { bool testSuccess = test(); \
    if (!testSuccess) { printf(#test " failed\n"); failCount++; } \
    }

bool shouldSucceedNow()
{
    ASSERT(0 == 0);

    return true;
}

bool shouldFailNow()
{
    ASSERT(0 == 1);

    return true;
}

bool shouldEchoHello()
{
    // make the shell

    nonint_shell(NULL /* put shell here */, "echo hello");

    // Get output from shell (have some way of writing for tests to read rather than going straight to stdout)
    /*ASSERT(output == "hello")*/

    return true;
}

int main()
{
    printf("Shell tests\n");

    int failCount = 0;

    nonint_shell(NULL, "echo hello");

    RUN_TEST(shouldSucceedNow);
    RUN_TEST(shouldFailNow);
    RUN_TEST(shouldEchoHello);

    if (failCount > 0)
    {
        printf("%d tests failed\n", failCount);
        return 1;
    }
    return 0;
}
