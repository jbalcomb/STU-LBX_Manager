// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

// OS Specific
#include <errno.h>      // errno, strerror()
#include <fcntl.h>      // O_RDONLY, O_WRONLY, O_LARGEFILE
#include <sys/ptrace.h> // ptrace()
#include <sys/time.h>   // gettimeofday()
#include <sys/types.h>  // pid_t, __off64_t
#include <sys/wait.h>   // waitpid()
#include <unistd.h>     // open(), close(), pread64(), pwrite64(), usleep()

// Standard C++ Library
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>     // kill()
#include <sstream>
#include <stdio.h>      // fflush()
#include <stdlib.h>     // system()
#include <string.h>     // memcmp()

// Libraries / Local
#include "MoMUtility.h"

#include "MoMProcess.h"

namespace MoM {

const std::string gPID_FILENAME = "dosbox.pid";

static void detachProcess(pid_t pid)
{
    // Try a couple of times
//    std::cout << "PTRACE_DETACH from pid=" << (unsigned)pid << std::endl;
    if (-1 == ptrace(PTRACE_DETACH, pid, NULL, NULL))
    {
        MoMProcess::printError(errno, "PTRACE_DETACH");
    }
}

static bool attachProcess(pid_t pid)
{
    // Try a couple of times
//    std::cout << "PTRACE_ATTACH to pid=" << pid << std::endl;
    if (-1 == ptrace(PTRACE_ATTACH, pid, NULL, NULL))
    {
        MoMProcess::printError(errno, "PTRACE_ATTACH");
        return false;
    }

    int status = 0;
    if (pid != waitpid(pid, &status, 0))
    {
        MoMProcess::printError(errno, "waitpid");
        // Note: if we choose to return false here, we need to DETACH the process first
    }
    else
    {
//        std::cout << "waitpid reports status 0x" << std::hex << status << std::dec << std::endl;
    }

    // Note: if we choose to return false here, we need to DETACH the process first
    return true;
}

double getPerformanceTime()
{
    struct timeval tp = {0, 0};
    (void)gettimeofday(&tp, 0);
    return tp.tv_sec + tp.tv_usec * 1e-6;
}

void MoMProcess::closeProcess() throw()
{
    if (NULL != m_hProcess)
    {
        pid_t* ppid = (pid_t*)m_hProcess;
        delete ppid;
        m_hProcess = NULL;
    }
}

bool MoMProcess::createProcess(const char* applicationName, const char* commandLine, const char* currentDirectory)
{
    std::cout << "MoMProcess::createProcess() not implemented yet" << std::endl;
    return false;
}

bool MoMProcess::terminateCreatedProcess()
{
    std::cout << "MoMProcess::terminateCreatedProcess() not implemented yet" << std::endl;
    return false;
}

bool MoMProcess::findProcessAndData()
{
    const std::string title = "dosbox";

    // Find pid of DOSBox
    std::string cmd = "ps -e | awk '/" + title + "/ { print $1; }' >" + gPID_FILENAME;
    std::cout << cmd << std::endl;
    fflush(NULL);   // Flush before system()
    if (0 != system(cmd.c_str()))
    {
        std::cout << "MoM process '" << title << "' not found" << std::endl;
        return false;
    }

    pid_t pid = 0;
    bool ok = false;
    std::ifstream ifs(gPID_FILENAME.c_str());
    while (!ok && ifs >> pid)
    {
        close();

        std::cout << "Trying process '" << title << "' with pid=" << pid << std::endl;

        ok = tryLinuxPid(new pid_t(pid));
    }

    return ok;
}

bool MoMProcess::tryLinuxPid(void* vPid)
{
    // Ownership of vPid is transfered
    pid_t pid = *(pid_t*)vPid;
    m_hProcess = vPid;

    bool ok = true;
    std::vector<uint8_t> data;
    double timer_start[2] = {0, 0};
    double timer_tot[2] = {0, 0};

    // Try to find the memory region of MoM within DOSBox (or use the defaults)
    std::cout << "Search for MoM Data Segment (DS) Identifier (size " << ARRAYSIZE(gDATASEGMENT_IDENTIFIER) << "): ";
    std::cout << gDATASEGMENT_IDENTIFIER + 4 << std::endl;
    std::string filename = "/proc/" + toStr(pid) + "/maps";
    std::ifstream ifs(filename.c_str());
    if (!ifs)
    {
        printError(errno, "Could not open '" + filename + "'");
        ok = false;
    }
    while (ok && ifs && (0 == m_lpBaseAddress) && ((0 == m_dwOffsetDatasegment) || m_exeFilepath.empty()))
    {
        unsigned long start = 0;
        char dashChar = '\0';
        unsigned long stop = 0;
        std::string perms;
        unsigned long offsetMem = 0;
        std::string dev;
        std::string inode;
        std::string pathname;

        ifs >> std::hex >> start >> dashChar >> stop >> perms >> offsetMem >> dev >> inode;
        std::getline(ifs, pathname);
        std::istringstream iss(pathname);
        iss >> pathname;

        size_t size = stop - start;
        if (m_verbose) std::cout << "Scanning 0x" << std::hex << start << " size 0x" << size
                                 << " perms='" << perms << " pathname='" << pathname << "'" << std::dec << std::endl;

        if ((perms != "rw-p") || (!pathname.empty() && (pathname[0] != ' ')))
        {
            if (m_verbose) std::cout << "Skipping due to mismatching attributes" << std::endl;
            continue;
        }

        timer_start[0] = getPerformanceTime();

        data.resize(size);
        if (readProcessData(m_hProcess, (const uint8_t*)start, size, &data[0]))
        {
            timer_start[1] = getPerformanceTime();
            timer_tot[0] += timer_start[1] - timer_start[0];

            ok = findSignatures(start, data);

            timer_tot[1] += getPerformanceTime() - timer_start[1];
        }
    }
    ifs.close();

    ok = registerResults(ok);

    std::cout << "Tot time reading process memory = " << timer_tot[0] << " seconds" << std::endl;
    std::cout << "Tot time finding signatures = " << timer_tot[1] << " seconds" << std::endl;

    return ok;
}

bool MoMProcess::readProcessData(void* hProcess, const uint8_t* lpBaseAddress, size_t size, uint8_t* data)
{
    if (NULL == hProcess)
        return false;
    if (0 == lpBaseAddress)
        return false;
    if (0 == size)
        return false;

    pid_t pid = *(pid_t*)hProcess;

    if (!attachProcess(pid))
    {
        return false;
    }

    bool ok = true;

    char memfile[256];
    sprintf(memfile, "/proc/%u/mem", (unsigned)pid);
    int fd = open(memfile, O_RDONLY | O_LARGEFILE);
    if (-1 == fd)
    {
        printError(errno, "open");
        ok = false;
    }

    if (-1 == pread64(fd, data, size, (unsigned long)lpBaseAddress))
    {
        printError(errno, "pread64");
        ok = false;
    }

    if (-1 != fd)
    {
        ::close(fd);
    }

    detachProcess(pid);

    return ok;
}

bool MoMProcess::writeData(const void* pointer, size_t size)
{
    std::cout << "MoMProcess::writeData()" << std::endl;
    if (NULL == m_hProcess)
        return false;
    if (0 == m_lpBaseAddress)
        return false;
    if (0 == m_dwOffsetDatasegment)
        return false;
    if (0 == m_dwOffsetDatasegment)
        return false;
    if (m_dataSegmentAndUp.empty())
        return false;
    size_t offset = (const uint8_t*)pointer - &m_dataSegmentAndUp[0];
    if (m_dwOffsetDatasegment + offset + size > m_dwBaseAddressSize)
        return false;
    if (offset + size > m_dataSegmentAndUp.size())
        return false;

    pid_t pid = *(pid_t*)m_hProcess;

    if (!attachProcess(pid))
    {
        return false;
    }

    bool ok = true;

    size_t i = 0;
    for(; i + sizeof(long) - 1 < size; i += sizeof(long))
    {
        if (-1 == ptrace(PTRACE_POKEDATA, pid, m_lpBaseAddress + m_dwOffsetDatasegment + offset + i, *(long*)((uint8_t*)pointer + i)))
        {
            printError(errno, "PTRACE_POKEDATA");
            ok = false;
            break;
        }
    }

    if (ok && i < size)
    {
        long buf = ptrace(PTRACE_PEEKDATA, pid, m_lpBaseAddress + m_dwOffsetDatasegment + offset + i, NULL);
        if (0 != errno)
        {
            printError(errno, "PTRACE_PEEKDATA");
            ok = false;
        }
        if (ok)
        {
            uint8_t* ptr = (uint8_t*)&buf;
            size_t last_offset = i;
            for (; i < size; ++i)
            {
                ptr[i - last_offset] = *((uint8_t*)pointer + i);
            }
            if (-1 == ptrace(PTRACE_POKEDATA, pid, m_lpBaseAddress + m_dwOffsetDatasegment + offset + last_offset, buf))
            {
                printError(errno, "PTRACE_POKEDATA(last bytes)");
                ok = false;
            }
        }
    }

    detachProcess(pid);

    return ok;
}

void MoMProcess::printError(int errorNumber, const std::string& msg)
{
    std::cout << "WARN: " << msg << ": " << strerror(errorNumber) << std::endl;
}

void MoMProcess::sleepSec(double timeout)
{
    useconds_t microseconds = (useconds_t)(timeout * 1000000 + 0.5);
    usleep(microseconds);
}

}
