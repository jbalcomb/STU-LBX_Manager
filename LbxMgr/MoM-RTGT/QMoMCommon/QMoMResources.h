// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

/// \file

#ifndef QMOMRESOURCES_H_
#define QMOMRESOURCES_H_

#include <qfont.h>
#include <qicon.h>
#include <qimage.h>
#include <qvector.h>

#include "MoMLbxBase.h"
#include "MoMTemplate.h"
#include "QMoMAnimation.h"
#include "QMoMLbx.h"
#include "QMoMSharedPointers.h"
#include "QMoMUtility.h"

namespace MoM {
    class MoMGameBase;
}

namespace MoM {

class QMoMResources
{
public:
    QMoMResources(const QMoMGamePtr& game);
    virtual ~QMoMResources();

    static void createInstance(const QMoMGamePtr& game);

    static QMoMResources& instance();

    void setGame(const QMoMGamePtr& game);

    const QMoMAnimation getAnimation(const LBXRecordID& lbxRecordID) const;
    const QMoMAnimation getAnimation(MoM::eBuilding building) const;
    const QMoMAnimation getAnimation(MoM::eRanged_Type rangedType, int heading = 3) const;
    const QMoMAnimation getAnimation(MoM::eTerrainType terrain) const;
    const QMoMAnimation getAnimation(MoM::eUnit_Type unitType, int heading = -1, MoM::eBannerColor bannerColor = MoM::BANNER_Green) const;

    const QMoMPalette& getColorTable()
	{
		return m_colorTable;
	}

    const HelpLBXentry* getHelpEntry(eHelpIndex helpTextNr);
    std::string getHelpText(eHelpIndex helpTextNr);

    std::string getHelpText(eBuilding building);
    std::string getHelpText(eHeroAbility heroAbility);
    std::string getHelpText(eItemPower itemPower);
    std::string getHelpText(ePortrait wizardPortrait);
    std::string getHelpText(eRace race);
    std::string getHelpText(eRanged_Type rangedType);
    const HelpLBXentry* getHelpEntry(eSpell spell);
    std::string getHelpText(eSpell spell);
    std::string getHelpText(eUnitAbility unitAbility);
    std::string getHelpText(eUnitEnchantment unitEnchantment);
    std::string getHelpText(eUnitMutation unitMutation);

    template<typename T>
    const QMoMIconPtr getIcon(T t, double scale = 1) const
    {
        QMoMIconPtr iconPtr(new QIcon(getPixmap(t, scale)));
        return iconPtr;
    }

    const QMoMImagePtr getImage(const LBXRecordID& lbxRecordID) const;
    const QMoMImagePtr getImage(MoM::eBannerColor bannerColor) const;
    const QMoMImagePtr getImage(MoM::eBuilding building) const;
    const QMoMImagePtr getImage(MoM::eCentralStructure structure) const;
    const QMoMImagePtr getImage(MoM::eCity_Size citySize, MoM::eBannerColor banner = MoM::BANNER_Green) const;
    const QMoMImagePtr getImage(MoM::eCityWall wall, bool broken = false) const;
    const QMoMImagePtr getImage(MoM::eItem_Icon itemIcon) const;
    const QMoMImagePtr getImage(MoM::eItemPower itemPower) const;
    const QMoMImagePtr getImage(MoM::eTower_Node_Lair_Type lair, bool overland = false) const;
    const QMoMImagePtr getImage(MoM::eProducing producing) const;
    const QMoMImagePtr getImage(MoM::eRace race) const;
    const QMoMImagePtr getImage(MoM::eRandomPickType randomPickType) const;
    const QMoMImagePtr getImage(MoM::eResource resource) const;
    const QMoMImagePtr getImage(MoM::eRanged_Type rangedType, int heading = 3) const;
    const QMoMImagePtr getImage(MoM::eSlot_Type8 slotType) const;
    const QMoMImagePtr getImage(MoM::eSlot_Type16 slotType) const;
    const QMoMImagePtr getImage(MoM::eSpell spell) const;
    const QMoMImagePtr getImage(MoM::eTerrainBattle terrain) const;
    const QMoMImagePtr getImage(MoM::eTerrainBonusDeposit bonusDeposit) const;
    const QMoMImagePtr getImage(MoM::eTerrainChange terrainChange, int roadDirection = 0) const;
    const QMoMImagePtr getImage(MoM::eTerrainType terrain) const;
    const QMoMImagePtr getImage(MoM::eUnit_Type unitType, int heading = -1, MoM::eBannerColor bannerColor = MoM::BANNER_Green) const;
    template<typename T>
    const QMoMImagePtr getImage(T) const
    {
        // Default match for getImage()
        return QMoMImagePtr(); 
    }
 
    template<typename T>
    QPixmap getPixmap(T t, double scale = 1.0) const
    {
        const QMoMImagePtr pImage = getImage(t);
        QPixmap pixmap;
        if (!pImage.isNull())
        {
            if (1.0 != scale)
			{
                QImage image = pImage->scaled(pImage->size() * scale, Qt::IgnoreAspectRatio, Qt::FastTransformation);
	            pixmap.convertFromImage(image);
			}
			else
			{
	            pixmap.convertFromImage(*pImage);
			}
        }
        return pixmap;
    }

    const QString getNameSkill(MoM::eSkill skill) const;
    // TODO: Why doesn't getName(eSkill) match instead of the default match?
    const QString getName(MoM::eSkill skill) const { return getNameSkill(skill); }
    const QString getName(MoM::eSpell spell) const;
    const QString getName(MoM::eSpell16 spell) const { return getName((MoM::eSpell)spell); }
    template<typename T>
    const QString getName(const T& t) const
    {
        // Default match
        return prettyQStr(t);
    }

    template<typename T, typename Value>
    QPixmap getPixmap(T t, double scale, Value value) const
    {
        const QMoMImagePtr pImage = getImage(t, value);
        QPixmap pixmap;
        if (!pImage.isNull())
        {
            if (1.0 != scale)
			{
                QImage image = pImage->scaled(pImage->size() * scale, Qt::IgnoreAspectRatio, Qt::FastTransformation);
	            pixmap.convertFromImage(image);
			}
			else
			{
	            pixmap.convertFromImage(*pImage);
			}
        }
        return pixmap;
    }

public:
    static const QFont g_FontSmall;
    static QFont g_Font;
    static const QFont g_FontFixed;

private:
    void changeBannerColor(MoM::eBannerColor bannerColor, QMoMAnimation& animation) const;
    void changeBannerColor(MoM::eBannerColor bannerColor, QMoMImagePtr& image) const;

    bool createColorTable();

    void createBuildingAnimations();
    void createCitySizeImages();
    void createFigureAnimations();
    void createLairImages();
    void createLbxAnimations(const std::string& lbxTitle, QVector<QMoMAnimation>& vecAnimations);
    void createLbxImages(const std::string& lbxTitle, QVector<QMoMImagePtr>& vecImages);
    void createSpellImages();
    void createTerrainImages();
    void createUnitImages();

    template<typename T, typename S>
    bool inVectorRange(const QVector<S>& v, T t) const
	{
		return (static_cast<unsigned>(t) < static_cast<unsigned>(v.size()));
	}

    static QMoMResources* m_instance;

    QMoMGamePtr m_game;

    QMoMLbxPtr m_helpLbx;

    QMoMPalette m_colorTable;
    QVector<QMoMImagePtr> m_backgrndImages;
    QVector<QMoMAnimation> m_buildingAnimations;
    QVector<QMoMImagePtr> m_citySizeImages;
    QVector<QMoMAnimation> m_citywallAnimations;
    QVector<QMoMImagePtr> m_cmbtcityImages;
    QVector<QMoMAnimation> m_figureAnimations;
    QVector<QMoMImagePtr> m_itemiscImages;
    QVector<QMoMImagePtr> m_itemsImages;
    QVector<QMoMImagePtr> m_lairImagesBig;
    QVector<QMoMImagePtr> m_lairImagesOverland;
    QVector<QMoMImagePtr> m_mapBackImages;
    QVector<QMoMImagePtr> m_specialImages;
    QVector<QMoMImagePtr> m_spellImages;
    QVector<QMoMImagePtr> m_terrainBattleImages;
    QVector<QMoMAnimation> m_terrainAnimations;
    QVector<QMoMImagePtr> m_unitImages;
    std::unique_ptr<MoMExeWizards> m_wizardsExe;
};

}

#endif // QMOMRESOURCES_H_
