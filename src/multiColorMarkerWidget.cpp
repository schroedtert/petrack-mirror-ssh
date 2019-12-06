#include "multiColorMarkerWidget.h"

MultiColorMarkerWidget::MultiColorMarkerWidget(QWidget *parent)
    : QWidget(parent)
{
    mMainWindow = (class Petrack*) parent;
    setupUi(this);
    mOldMinArea = minArea->value();
    mOldMaxArea = maxArea->value();
}

// store data in xml node
void MultiColorMarkerWidget::setXml(QDomElement &elem)
{
    QDomElement subElem;

    subElem = (elem.ownerDocument()).createElement("BLACK_DOT");
    subElem.setAttribute("USE", useDot->isChecked());
    subElem.setAttribute("SIZE", dotSize->value());
    subElem.setAttribute("IGNORE_WITHOUT", ignoreWithoutDot->isChecked());
    subElem.setAttribute("USE_COLOR", useColor->isChecked());
    subElem.setAttribute("RESTRICT_POSITION", restrictPosition->isChecked());
    elem.appendChild(subElem);

    subElem = (elem.ownerDocument()).createElement("AUTO_CORRECT");
    subElem.setAttribute("USE", autoCorrect->isChecked());
    subElem.setAttribute("ONLY_EXPORT", autoCorrectOnlyExport->isChecked());
    elem.appendChild(subElem);

    subElem = (elem.ownerDocument()).createElement("MASK");
    subElem.setAttribute("SHOW", showMask->isChecked());
    subElem.setAttribute("OPACITY", opacity->value());
    subElem.setAttribute("MASK", maskMask->isChecked());
    elem.appendChild(subElem);

    subElem = (elem.ownerDocument()).createElement("PARAM");
    subElem.setAttribute("CLOSE_RADIUS", closeRadius->value());
    subElem.setAttribute("CLOSE_USED", useClose->isChecked());
    subElem.setAttribute("OPEN_RADIUS", openRadius->value());
    subElem.setAttribute("OPEN_USED", useOpen->isChecked());
    subElem.setAttribute("MIN_AREA", minArea->value());
    subElem.setAttribute("MAX_AREA", maxArea->value());
    subElem.setAttribute("USE_HEAD_SIZE", useHeadSize->isChecked());
    subElem.setAttribute("MAX_RATIO", maxRatio->value());
    elem.appendChild(subElem);
}

// read data from xml node
void MultiColorMarkerWidget::getXml(QDomElement &elem)
{
    QDomElement subElem;
    int h=0, s=0, v=0; // init, damit compiler nicht meckert
    QString styleString;
    QColor col;
    col.convertTo(QColor::Hsv);

    for(subElem = elem.firstChildElement(); !subElem.isNull(); subElem = subElem.nextSiblingElement())
    {
        if (subElem.tagName() == "MASK")
        {
            if (subElem.hasAttribute("SHOW"))
                showMask->setCheckState(subElem.attribute("SHOW").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("OPACITY"))
                opacity->setValue(subElem.attribute("OPACITY").toInt());
            if (subElem.hasAttribute("MASK"))
                maskMask->setCheckState(subElem.attribute("MASK").toInt() ? Qt::Checked : Qt::Unchecked);
        }
        if (subElem.tagName() == "BLACK_DOT")
        {
            if (subElem.hasAttribute("USE"))
                useDot->setCheckState(subElem.attribute("USE").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("SIZE"))
                dotSize->setValue(subElem.attribute("SIZE").toDouble());
            if (subElem.hasAttribute("IGNORE_WITHOUT"))
                ignoreWithoutDot->setCheckState(subElem.attribute("IGNORE_WITHOUT").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("USE_COLOR"))
                useColor->setCheckState(subElem.attribute("USE_COLOR").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("RESTRICT_POSITION"))
                restrictPosition->setCheckState(subElem.attribute("RESTRICT_POSITION").toInt() ? Qt::Checked : Qt::Unchecked);
        }
        if (subElem.tagName() == "AUTO_CORRECT")
        {
            if (subElem.hasAttribute("USE"))
                autoCorrect->setCheckState(subElem.attribute("USE").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("ONLY_EXPORT"))
                autoCorrectOnlyExport->setCheckState(subElem.attribute("ONLY_EXPORT").toInt() ? Qt::Checked : Qt::Unchecked);
        }
        if (subElem.tagName() == "PARAM")
        {
            if (subElem.hasAttribute("CLOSE_RADIUS"))
                closeRadius->setValue(subElem.attribute("CLOSE_RADIUS").toInt());
            if (subElem.hasAttribute("CLOSE_USED"))
                useClose->setCheckState(subElem.attribute("CLOSE_USED").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("OPEN_RADIUS"))
                openRadius->setValue(subElem.attribute("OPEN_RADIUS").toInt());
            if (subElem.hasAttribute("OPEN_USED"))
                useOpen->setCheckState(subElem.attribute("OPEN_USED").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("MIN_AREA"))
                minArea->setValue(subElem.attribute("MIN_AREA").toInt());
            if (subElem.hasAttribute("MAX_AREA"))
                maxArea->setValue(subElem.attribute("MAX_AREA").toInt());
            if (subElem.hasAttribute("USE_HEAD_SIZE"))
                useHeadSize->setCheckState(subElem.attribute("USE_HEAD_SIZE").toInt() ? Qt::Checked : Qt::Unchecked);
            if (subElem.hasAttribute("MAX_RATIO"))
                maxRatio->setValue(subElem.attribute("MAX_RATIO").toDouble());
        }
    }
}
