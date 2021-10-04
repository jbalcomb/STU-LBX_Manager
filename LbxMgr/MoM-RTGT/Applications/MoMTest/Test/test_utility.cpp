#include <fstream>

#include "test_utility.h"

// Implement function for POSIX diff that is not (always) available (in Windows)
int diff(const std::string& file1, const std::string& file2)
{
    // Open both files
    std::ifstream ifs1(file1.c_str(), std::ios::binary);
    if (!ifs1)
        return 2;
    std::ifstream ifs2(file2.c_str(), std::ios::binary);
    if (!ifs2)
        return 2;

    // Get file sizes
    ifs1.seekg(0, std::ios_base::end);
    std::ios::pos_type size1 = ifs1.tellg();
    ifs2.seekg(0, std::ios_base::end);
    std::ios::pos_type size2 = ifs2.tellg();
    if (size1 != size2)
        return 1;
    ifs1.seekg(0, std::ios_base::beg);
    ifs2.seekg(0, std::ios_base::beg);

    // Read files
    std::string buf1(size1, '\0');
    ifs1.read(&buf1[0], size1);
    if (!ifs1)
        return 2;
    std::string buf2(size2, '\0');
    ifs2.read(&buf2[0], size2);
    if (!ifs2)
        return 2;

    if (buf1 == buf2)
        return 0;
    else
        return 1;
}

