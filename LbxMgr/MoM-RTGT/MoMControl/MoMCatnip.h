// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMCATNIP_H
#define MOMCATNIP_H

#include <string>

namespace MoM
{

class MoMCatnip
{
public:
    MoMCatnip();

    bool apply(class MoMGameBase* game);

    const std::string& errorString() const
    {
        return m_errorString;
    }

private:
    std::string m_errorString;
};

}

#endif // MOMCATNIP_H
