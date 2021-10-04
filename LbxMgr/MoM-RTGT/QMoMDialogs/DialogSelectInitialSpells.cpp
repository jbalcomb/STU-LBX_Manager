// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "DialogSelectInitialSpells.h"
#include "ui_DialogSelectInitialSpells.h"

#include <algorithm>    // random_shuffle()
#include <stdlib.h>     // rand(), srand()
#include <time.h>       // time()

#include "MoMUtility.h"
#include "MoMGenerated.h"
#include "MainWindow.h"


//        starting guaranteed  totals   discount   total # spells
//books | C U R VR| C U R VR| C U R VR|          |
//1.....|         |         | 2       |    0%    |    2
//2.....|         | 1       | 3 1     |    0%    |    4
//3.....|         | 1       | 4 2     |    0%    |    6
//4.....|         | 2       | 4 3 1   |    5%    |    8
//5.....|         | 2       | 5 3 2   |   10%    |   10
//6.....|         | 2 1     | 5 4 2 1 |   15%    |   12
//7.....| 1       | 2 1     | 6 4 3 1 |   20%    |   14
//8.....| 1       | 3 1 1   | 6 5 3 2 |   25%    |   16
//9.....| 2       | 3 2 1   | 7 5 4 2 |   30%    |   18
//10....| 2       | 3 3 1 1 | 7 6 4 3 |   35%    |   20
//11....| 2 1     | 4 3 2 2 | 8 6 5 4 |   40%    |   23

struct SpellsPerBook
{
    size_t starting[4];
    size_t guaranteed[4];
    size_t total[4];
};

const SpellsPerBook gSpellsPerBook[12] =
{
    { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },   // 0
    { {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 0, 0} },
    { {0, 0, 0, 0}, {1, 0, 0, 0}, {3, 1, 0, 0} },
    { {0, 0, 0, 0}, {1, 0, 0, 0}, {4, 2, 0, 0} },
    { {0, 0, 0, 0}, {2, 0, 0, 0}, {4, 3, 1, 0} },
    { {0, 0, 0, 0}, {2, 0, 0, 0}, {5, 3, 2, 0} },   // 5
    { {0, 0, 0, 0}, {2, 1, 0, 0}, {5, 4, 2, 1} },
    { {1, 0, 0, 0}, {2, 1, 0, 0}, {6, 4, 3, 1} },
    { {1, 0, 0, 0}, {3, 1, 1, 0}, {6, 5, 3, 2} },
    { {2, 0, 0, 0}, {3, 2, 1, 0}, {7, 5, 4, 2} },
    { {2, 0, 0, 0}, {3, 3, 1, 1}, {7, 6, 4, 3} },   // 10
    { {2, 1, 0, 0}, {4, 3, 2, 2}, {8, 6, 5, 4} },
};


class InitialSpells
{
public:
    enum eStatus { none, random, guaranteed, starting };

    InitialSpells()
    {
        memset(m_spells, '\0', sizeof(m_spells));
        memset(m_allotedRandom, '\0', sizeof(m_allotedRandom));
    }

    size_t getAllottedRandom(MoM::eRealm_Type realm, MoM::eRarity rarity)
    {
        return m_allotedRandom[realm][rarity];
    }

    size_t getNrStarting(MoM::eRealm_Type realm, MoM::eRarity rarity)
    {
        size_t n = 0;
        for (size_t i = 0; i < 10; ++i)
        {
            MoM::eSpell spell = (MoM::eSpell)(1 + realm * 40 + rarity * 10 + i);
            if (starting == m_spells[spell])
            {
                ++n;
            }
        }
        return n;
    }

    size_t getNrGuaranteed(MoM::eRealm_Type realm, MoM::eRarity rarity)
    {
        size_t n = 0;
        for (size_t i = 0; i < 10; ++i)
        {
            MoM::eSpell spell = (MoM::eSpell)(1 + realm * 40 + rarity * 10 + i);
            if (guaranteed == m_spells[spell])
            {
                ++n;
            }
        }
        return n;
    }

    eStatus getSpellStatus(MoM::eSpell spell)
    {
        return m_spells[spell];
    }

    void setAllottedRandom(MoM::eRealm_Type realm, MoM::eRarity rarity, size_t value)
    {
        m_allotedRandom[realm][rarity] = value;
    }

    void setSpellStatus(MoM::eSpell spell, eStatus status)
    {
        m_spells[spell] = status;
    }

    void randomize();

private:
    eStatus m_spells[MoM::eSpell_MAX];
    size_t m_allotedRandom[MoM::eRealm_Type_MAX][MoM::eRarity_MAX];
};

void InitialSpells::randomize()
{
    for (MoM::eRealm_Type color = (MoM::eRealm_Type)0; color < 5; MoM::inc(color))
    {
        for (MoM::eRarity rarity = (MoM::eRarity)0; rarity < MoM::eRarity_MAX; MoM::inc(rarity))
        {
            std::vector<MoM::eSpell> listSpells;

            for (size_t i = 0; i < 10; ++i)
            {
                MoM::eSpell spell = (MoM::eSpell)(1 + color * 40 + rarity * 10 + i);
                if (none == m_spells[spell] || random == m_spells[spell])
                {
                    listSpells.push_back(spell);
                    m_spells[ spell ] = none;
                }
            }

            std::random_shuffle(listSpells.begin(), listSpells.end());

            // Set selected spells to random
            // Set remaining spells to none
            for (size_t i = 0; i < listSpells.size(); ++i)
            {
                if (i < getAllottedRandom(color, rarity))
                {
                    m_spells[ listSpells[i] ] = random;
                }
            }

        }
    }

}


DialogSelectInitialSpells::DialogSelectInitialSpells(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogSelectInitialSpells),
    m_Realm_Type(MoM::REALM_Life),
    m_InitialSpells(0)
{
    ui->setupUi(this);

    ui->listWidget_Common->setFont(m_font);
    ui->listWidget_Uncommon->setFont(m_font);
    ui->listWidget_Rare->setFont(m_font);
    ui->listWidget_VeryRare->setFont(m_font);

    srand(time(NULL));

    m_InitialSpells = new InitialSpells;

    postInitialize();
}

DialogSelectInitialSpells::~DialogSelectInitialSpells()
{
    postInitialize();

    delete m_InitialSpells;

    delete ui;
}

void DialogSelectInitialSpells::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;

    MoM::eSpellKnown* known = 0;
    if (!m_game.isNull())
    {
        MoM::Wizard* wizard = m_game->getWizard(MoM::PLAYER_YOU);
        if (0 != wizard)
        {
            known = &wizard->m_Spells_Known.No_spell;     // First spell
        }
    }

    if (0 != known)
    {
        for (MoM::eSpell spell = (MoM::eSpell)0; spell < MoM::eSpell_MAX; MoM::inc(spell))
        {
            switch (known[spell])
            {
            default:
            case MoM::SPELLKNOWN_unknown:           m_InitialSpells->setSpellStatus(spell, InitialSpells::none); break;
            case MoM::SPELLKNOWN_knowable:
            case MoM::SPELLKNOWN_researchable:      m_InitialSpells->setSpellStatus(spell, InitialSpells::guaranteed); break;
            case MoM::SPELLKNOWN_currently_known:   m_InitialSpells->setSpellStatus(spell, InitialSpells::starting); break;
            }
        }
    }
}

void DialogSelectInitialSpells::slot_gameUpdated()
{
}

bool DialogSelectInitialSpells::apply()
{
    MoM::eSpellKnown* known = 0;
    if (m_game.isNull())
        return false;
    MoM::Wizard* wizard = m_game->getWizard(MoM::PLAYER_YOU);
    if (0 == wizard)
        return false;
    known = &wizard->m_Spells_Known.No_spell;     // First spell

    // Check if numbers match
    for (MoM::eRealm_Type color = (MoM::eRealm_Type)0; color < 5; MoM::inc(color))
    {
        for (MoM::eRarity rarity = (MoM::eRarity)0; rarity < MoM::eRarity_MAX; MoM::inc(rarity))
        {
            const SpellsPerBook* spellsPerBook = &gSpellsPerBook[*(&wizard->m_Number_of_Spellbooks_Nature + color)];
            if (spellsPerBook->guaranteed[rarity] != m_InitialSpells->getNrGuaranteed(color, rarity))
            {
                return false;
            }
            if (spellsPerBook->starting[rarity] != m_InitialSpells->getNrStarting(color, rarity))
            {
                return false;
            }

            m_InitialSpells->setAllottedRandom(color, rarity, spellsPerBook->total[rarity] - spellsPerBook->starting[rarity] - spellsPerBook->guaranteed[rarity]);
        }
    }

    // Randomize remaining spells
    m_InitialSpells->randomize();

    // Update MoM
    if (0 != known)
    {
        for (MoM::eSpell spell = (MoM::eSpell)0; spell < MoM::eSpell_MAX; MoM::inc(spell))
        {
            switch (m_InitialSpells->getSpellStatus(spell))
            {
            default:
            case InitialSpells::none:       known[spell] = MoM::SPELLKNOWN_unknown; break;
            case InitialSpells::random:     known[spell] = MoM::SPELLKNOWN_knowable; break;
            case InitialSpells::guaranteed: known[spell] = MoM::SPELLKNOWN_knowable; break;
            case InitialSpells::starting:   known[spell] = MoM::SPELLKNOWN_currently_known; break;
            }
        }
    }

    // TODO: set spells to researchable
    for (size_t i = 0; i < 8; ++i)
    {
        wizard->m_Research_Spell_candidates[i] = MoM::SPELL16_None;
    }

    // Commit
    if (!m_game->commitData(&wizard->m_Spells_Known, known, sizeof(wizard->m_Spells_Known)))
    {
        std::cout << "Failed to commit data" << std::endl;
    }

    return true;
}

void DialogSelectInitialSpells::update()
{
    int16_t* nrSpellbooks = 0;
    if (!m_game.isNull())
    {
        MoM::Wizard* wizard = m_game->getWizard(MoM::PLAYER_YOU);
        if (0 != wizard)
        {
            nrSpellbooks = &wizard->m_Number_of_Spellbooks_Nature;
        }
    }

    QColor color = Qt::gray;
    switch (m_Realm_Type)
    {
    case MoM::REALM_Nature:     color = Qt::green; break;
    case MoM::REALM_Sorcery:    color = Qt::cyan; break;
    case MoM::REALM_Chaos:      color = Qt::red; break;
    case MoM::REALM_Life:       color = Qt::white; break;
    case MoM::REALM_Death:      color = Qt::black; break;
    case MoM::REALM_Arcane:     color = Qt::yellow; break;
    default:                    break;
    }

    if (0 != nrSpellbooks)
    {
        int nrBooks = nrSpellbooks[ m_Realm_Type ];
        ui->label_Common_Values->setText(QString("%0 Starting, %1 Guaranteed")
                .arg(gSpellsPerBook[ nrBooks ].starting[0])
                .arg(gSpellsPerBook[ nrBooks ].guaranteed[0]));
        ui->label_Uncommon_Values->setText(QString("%0 Starting, %1 Guaranteed")
                .arg(gSpellsPerBook[ nrBooks ].starting[1])
                .arg(gSpellsPerBook[ nrBooks ].guaranteed[1]));
        ui->label_Rare_Values->setText(QString("%0 Starting, %1 Guaranteed")
                .arg(gSpellsPerBook[ nrBooks ].starting[2])
                .arg(gSpellsPerBook[ nrBooks ].guaranteed[2]));
        ui->label_VeryRare_Values->setText(QString("%0 Starting, %1 Guaranteed")
                .arg(gSpellsPerBook[ nrBooks ].starting[3])
                .arg(gSpellsPerBook[ nrBooks ].guaranteed[3]));
    }

    for (int index = 0; index < 40; ++index)
    {
        MoM::eSpell spell = (MoM::eSpell)(1 + m_Realm_Type * 40 + index);
        int row = index % 10;

        QListWidget* listWidget = 0;
        switch (index / 10)
        {
        default:
        case 0: listWidget = ui->listWidget_Common; break;
        case 1: listWidget = ui->listWidget_Uncommon; break;
        case 2: listWidget = ui->listWidget_Rare; break;
        case 3: listWidget = ui->listWidget_VeryRare; break;
        }
        QListWidgetItem* listItem = listWidget->item(row);

        listItem->setText(MoM::QMoMResources::instance().getName(spell));
        listItem->setForeground(QBrush(color));

        Qt::CheckState checkState;

        switch (m_InitialSpells->getSpellStatus(spell))
        {
        default:
        case InitialSpells::none:       checkState = Qt::Unchecked; break;
        case InitialSpells::guaranteed: checkState = Qt::PartiallyChecked; break;
        case InitialSpells::starting:   checkState = Qt::Checked; break;
        }

        listItem->setCheckState(checkState);
    }
}

void DialogSelectInitialSpells::on_listWidget_Common_itemClicked(QListWidgetItem*)
{
//    Qt::CheckState checkState;
//    switch (item->checkState())
//    {
//    case Qt::Unchecked:         checkState = Qt::PartiallyChecked; break;
//    case Qt::PartiallyChecked:  checkState = Qt::Checked; break;
//    default:
//    case Qt::Checked:           checkState = Qt::Unchecked; break;
//    }
//    item->setCheckState(checkState);
}

void DialogSelectInitialSpells::on_listWidget_Uncommon_itemClicked(QListWidgetItem*)
{
//    Qt::CheckState checkState;
//    switch (item->checkState())
//    {
//    case Qt::Unchecked:         checkState = Qt::PartiallyChecked; break;
//    case Qt::PartiallyChecked:  checkState = Qt::Checked; break;
//    default:
//    case Qt::Checked:           checkState = Qt::Unchecked; break;
//    }
//    item->setCheckState(checkState);
}

void DialogSelectInitialSpells::on_listWidget_Rare_itemClicked(QListWidgetItem*)
{
//    Qt::CheckState checkState;
//    switch (item->checkState())
//    {
//    case Qt::Unchecked:         checkState = Qt::PartiallyChecked; break;
//    case Qt::PartiallyChecked:  checkState = Qt::Checked; break;
//    default:
//    case Qt::Checked:           checkState = Qt::Unchecked; break;
//    }
//    item->setCheckState(checkState);
}

void DialogSelectInitialSpells::on_listWidget_VeryRare_itemClicked(QListWidgetItem*)
{
//    Qt::CheckState checkState;
//    switch (item->checkState())
//    {
//    case Qt::Unchecked:         checkState = Qt::PartiallyChecked; break;
//    case Qt::PartiallyChecked:  checkState = Qt::Checked; break;
//    default:
//    case Qt::Checked:           checkState = Qt::Unchecked; break;
//    }
//    item->setCheckState(checkState);
}

void DialogSelectInitialSpells::on_pushButton_Life_clicked()
{
    m_Realm_Type = MoM::REALM_Life;
    update();
}

void DialogSelectInitialSpells::on_pushButton_Death_clicked()
{
    m_Realm_Type = MoM::REALM_Death;
    update();
}

void DialogSelectInitialSpells::on_pushButton_Chaos_clicked()
{
    m_Realm_Type = MoM::REALM_Chaos;
    update();
}

void DialogSelectInitialSpells::on_pushButton_Nature_clicked()
{
    m_Realm_Type = MoM::REALM_Nature;
    update();
}

void DialogSelectInitialSpells::on_pushButton_Sorcery_clicked()
{
    m_Realm_Type = MoM::REALM_Sorcery;
    update();
}

void DialogSelectInitialSpells::on_buttonBox_clicked(QAbstractButton* button)
{
    if (QDialogButtonBox::ApplyRole == ui->buttonBox->buttonRole(button))
    {
        if (!apply())
        {
            (void)QMessageBox::warning(this, tr("Select Initial Spells"),
                                        tr("The selection does not match the allowed picks"));
        }
        else
        {
            update();
        }
    }
}

void DialogSelectInitialSpells::on_listWidget_Common_clicked(QModelIndex index)
{
    MoM::eSpell spell = (MoM::eSpell)(1 + this->m_Realm_Type * 40 + index.row());
    QListWidgetItem* item = ui->listWidget_Common->item(index.row());
    switch (item->checkState())
    {
    case Qt::Unchecked:         m_InitialSpells->setSpellStatus(spell, InitialSpells::guaranteed); break;
    case Qt::PartiallyChecked:  m_InitialSpells->setSpellStatus(spell, InitialSpells::starting); break;
    default:
    case Qt::Checked:           m_InitialSpells->setSpellStatus(spell, InitialSpells::none); break;
    }
    update();
}

void DialogSelectInitialSpells::on_listWidget_Uncommon_clicked(QModelIndex index)
{
    MoM::eSpell spell = (MoM::eSpell)(1 + this->m_Realm_Type * 40 + 10 + index.row());
    QListWidgetItem* item = ui->listWidget_Uncommon->item(index.row());
    switch (item->checkState())
    {
    case Qt::Unchecked:         m_InitialSpells->setSpellStatus(spell, InitialSpells::guaranteed); break;
    case Qt::PartiallyChecked:  m_InitialSpells->setSpellStatus(spell, InitialSpells::starting); break;
    default:
    case Qt::Checked:           m_InitialSpells->setSpellStatus(spell, InitialSpells::none); break;
    }
    update();
}

void DialogSelectInitialSpells::on_listWidget_Rare_clicked(QModelIndex index)
{
    MoM::eSpell spell = (MoM::eSpell)(1 + this->m_Realm_Type * 40 + 20 + index.row());
    QListWidgetItem* item = ui->listWidget_Rare->item(index.row());
    switch (item->checkState())
    {
    case Qt::Unchecked:         m_InitialSpells->setSpellStatus(spell, InitialSpells::guaranteed); break;
    case Qt::PartiallyChecked:  m_InitialSpells->setSpellStatus(spell, InitialSpells::starting); break;
    default:
    case Qt::Checked:           m_InitialSpells->setSpellStatus(spell, InitialSpells::none); break;
    }
    update();
}

void DialogSelectInitialSpells::on_listWidget_VeryRare_clicked(QModelIndex index)
{
    MoM::eSpell spell = (MoM::eSpell)(1 + this->m_Realm_Type * 40 + 30 + index.row());
    QListWidgetItem* item = ui->listWidget_VeryRare->item(index.row());
    switch (item->checkState())
    {
    case Qt::Unchecked:         m_InitialSpells->setSpellStatus(spell, InitialSpells::guaranteed); break;
    case Qt::PartiallyChecked:  m_InitialSpells->setSpellStatus(spell, InitialSpells::starting); break;
    default:
    case Qt::Checked:           m_InitialSpells->setSpellStatus(spell, InitialSpells::none); break;
    }
    update();
}
