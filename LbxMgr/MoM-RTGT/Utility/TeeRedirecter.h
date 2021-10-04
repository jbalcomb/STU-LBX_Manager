// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef TEEREDIRECTER_H
#define TEEREDIRECTER_H

#include <fstream>
#include <iostream>
#include <streambuf>

class Teebuf : public std::streambuf
{
public:
    typedef std::char_traits<char> traits_type;
    typedef traits_type::int_type  int_type;

    Teebuf(std::streambuf* sb1, std::streambuf* sb2) :
        m_sb1(sb1), m_sb2(sb2) {}

protected:
    int_type overflow(int_type c)
    {
        int r1 = m_sb1->sputc(c);
        int r2 = m_sb2->sputc(c);
        if ((r1 != c) || (r2 != c))
        {
            return traits_type::eof();
        }
        else
        {
            return traits_type::not_eof(c);
        }
    }

    int sync()
    {
        int r1 = m_sb1->pubsync();
        int r2 = m_sb2->pubsync();
        int rc = 0;
		// In Windows cout will fail on sync(), but we want to keep going anyway
        if ((r1 != 0) && (r2 != 0))
        {
            rc = -1;
        }
        return rc;
    }

private:
    std::streambuf* m_sb1;
    std::streambuf* m_sb2;
};

class TeeRedirecter
{
public:
    explicit TeeRedirecter(const char* output_filename, bool verbose = false) :
        m_sb_cout(std::cout.rdbuf()),
        m_sb_cerr(std::cerr.rdbuf()),
        m_ofs_out(output_filename),
        m_tee_cout(m_sb_cout, m_ofs_out.rdbuf()),
        m_tee_cerr(m_sb_cerr, m_ofs_out.rdbuf()),
        m_verbose(verbose)
    {
        std::cout.rdbuf(&m_tee_cout);
        if (m_verbose) std::cout << "Output tee'ed from cout" << std::endl;
        std::cerr.rdbuf(&m_tee_cerr);
        if (m_verbose) std::cerr << "Output tee'ed from cerr" << std::endl;
    }
    ~TeeRedirecter()
    {
        // Restore cout and cerr
        if (m_verbose) std::cout << "Restoring cerr" << std::endl;
        std::cerr.rdbuf(m_sb_cerr);
        if (m_verbose)  std::cerr << "Output restored from cerr" << std::endl;
        if (m_verbose) std::cout << "Restoring cout" << std::endl;
        std::cout.rdbuf(m_sb_cout);
        std::cout << "Output redirection restored" << std::endl;
    }
private:
    std::streambuf* m_sb_cout;
    std::streambuf* m_sb_cerr;
    std::ofstream   m_ofs_out;
    Teebuf          m_tee_cout;
    Teebuf          m_tee_cerr;
    bool            m_verbose;
};

#endif // TEEREDIRECTER_H
