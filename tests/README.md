# Mini Shell Test Guide

## Step-by-step Process

### Step 1: Test a new function

you will copy the `test_template.c`

```c
cp tests/test_cases/test_template.c tests/test_cases/test_your_feature.c
```

### Step 2: Add test file to Test Makefile 

```c
TEST_SRCS += test_cases/test_your_feature.c
```

### Step 3: Register tests in `test_main.c`

```c
void    register_your_feature_tests(t_test_case **suite);

int main(void)
{
    t_test_case *suite = NULL;
    register_your_feature_tests(&suite);
    test_run_suite(suite);
    test_cleanup(suite);
    return (0);
}
```

### Step 4; Run tests in project root

```c
make test
```


