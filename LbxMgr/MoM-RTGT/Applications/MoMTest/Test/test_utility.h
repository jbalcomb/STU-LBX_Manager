#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include <string>

// Implement function for POSIX diff that is not (always) available (in Windows)
int diff(const std::string& file1, const std::string& file2);

#endif // TEST_UTILITY_H
