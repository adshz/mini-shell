test/
├── framework/
│   ├── test_framework.h     # Test framework declarations, common utilities
│   ├── test_framework.c     # Common test utilities and framework functions
│   └── test_utils.c        # Helper functions for redirection, cleanup, etc.
├── suites/
│   ├── cd_tests/
│   │   ├── cd_tests.h      # Declarations for CD command tests
│   │   ├── cd_basic.c      # Basic CD tests (cd /, cd ~)
│   │   ├── cd_special.c    # Special CD tests (cd -, cd with args)
│   │   └── cd_errors.c     # CD error cases
│   ├── builtin_tests/
│   │   ├── builtin_tests.h
│   │   ├── echo_tests.c
│   │   └── env_tests.c
│   └── shell_tests/
│       ├── shell_tests.h
│       ├── init_tests.c
│       └── cleanup_tests.c
└── main.c                  # Main test runner