// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMMAGICEXE_H
#define MOMMAGICEXE_H

#include "MoMExeBase.h"
#include "MoMTemplate.h"

namespace MoM {

class MoMExeMagic : public MoMExeBase
{
public:
    MoMExeMagic();

    virtual void close() throw();

    virtual bool load(const std::string& filename);

    virtual uint8_t* getMagicExeContents()
    {
        return getExeContents();
    }

private:
};

}

#endif
