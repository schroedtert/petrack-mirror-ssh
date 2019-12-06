#include <QtWidgets>

#include "petrack.h"
#include "view.h"
#include "codeMarkerItem.h"
#include "codeMarkerWidget.h"
#include "tracker.h"
#include "animation.h"

// in x und y gleichermassen skaliertes koordinatensystem,
// da von einer vorherigen intrinsischen kamerakalibrierung ausgegenagen wird,
// so dass pixel quadratisch 
CodeMarkerItem::CodeMarkerItem(QWidget *wParent, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    mMainWindow = (class Petrack*) wParent;
//    mImage = NULL;
    //setAcceptsHoverEvents(true);

    //    setEnabled(false); // all mouse events connot access this item, but it will be seen
    // einzig move koennte interessant sein, um grid zu verschieben?!
//     setAcceptsHoverEvents(true);
}
// // bounding box wird durch linke obere ecke und breite/hoehe angegeben
// // wenn an den rand gescrollt wurde im view, dann wird durch das dynamische anpassen
// // bei trans und scale zwar zuerst alles neu gezeichnet durch update, 
// // aber beim verkleinern des scrollbereichs nur der teil von tracker neu gezeichnet
QRectF CodeMarkerItem::boundingRect() const
{
    if (mMainWindow->getImage())
//        return QRectF(mUlc.x(),mUlc.y(),mMainWindow->width(),mMainWindow->height());
        return QRectF(-mMainWindow->getImageBorderSize(), -mMainWindow->getImageBorderSize(), mMainWindow->getImage()->width(), mMainWindow->getImage()->height());
//         return QRectF(-mMainWindow->getImageBorderSize(), -mMainWindow->getImageBorderSize(), mImage->width(), mImage->height());
    else
        return QRectF(0, 0, 0, 0);
//     // bounding box wird in lokalen koordinaten angegeben!!! (+-10 wegen zahl "1")
//     if (mControlWidget->getCalibCoordShow())
//         return QRectF(-110., -110., 220., 220.);
//     else                    ;

//         return QRectF(0., 0., 0., 0.);

//     // sicher ware diese boundingbox, da alles
//     //     return QRectF(xMin, yMin, xMax-xMin, yMax-yMin);
//     // eigentlich muesste folgende Zeile reichen, aber beim ranzoomen verschwindet dann koord.sys.
//     //     return QRectF(mControlWidget->getCalibCoordTransX()/10.-scale, mControlWidget->getCalibCoordTransY()/10.-scale, 2*scale, 2*scale);
}

void CodeMarkerItem::setRect(Vec2F& v)
{
    mUlc = v; // upper left corner to draw
}

void CodeMarkerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (mMainWindow->getCodeMarkerWidget()->showDetectedCandidates->isChecked())
    {
        QColor textColor = QColor(255,0,0),
               cornerColor = QColor(0,0,255),
               borderColor = QColor(0,255,0);

        int nMarkers = mCorners.size();
        int nRejected = mRejected.size();

//        debout << "#markers: " << nMarkers << " #rejected: " << nRejected << endl;
        Point2f p0, p1;
        for(int i = 0; i < nMarkers; i++)
        {
            vector<Point2f> currentMarker = mCorners.at(i);
            // draw marker sides
            for(int j = 0; j < 4; j++) {
                p0 = currentMarker.at(j);
                p1 = currentMarker.at((j + 1) % 4);
//                debout << "p0: " << p0 << " p1: " << p1 << endl;
                painter->setPen(borderColor);
                painter->drawLine(QPointF(mUlc.x()+p0.x,mUlc.y()+p0.y),QPointF(mUlc.x()+p1.x,mUlc.y()+p1.y));
            }
            painter->setPen(cornerColor);
            painter->drawRect(QRectF(mUlc.x()+currentMarker.at(0).x-3.0,mUlc.y()+currentMarker.at(0).y-3.0,6.0,6.0));
        }
        for(int i = 0; i < nRejected; i++)
        {
            vector<Point2f> currentMarker = mRejected.at(i);

            for(int j = 0; j < 4; j++) {
                p0 = currentMarker.at(j);
                p1 = currentMarker.at((j + 1) % 4);
//                debout << "p0: " << p0 << " p1: " << p1 << endl;
                painter->setPen(textColor);
                painter->drawLine(QPointF(mUlc.x()+p0.x,mUlc.y()+p0.y),QPointF(mUlc.x()+p1.x,mUlc.y()+p1.y));
            }
            painter->setPen(borderColor);
            painter->drawRect(QRectF(mUlc.x()+currentMarker.at(0).x-3.0,mUlc.y()+currentMarker.at(0).y-3.0,6.0,6.0));
        }
    }
    if (false) // Show min/max marker size
    {
        int minPerimeter = mMainWindow->getCodeMarkerWidget()->minMarkerPerimeter->value();
        int maxPerimeter = mMainWindow->getCodeMarkerWidget()->maxMarkerPerimeter->value();
        double height = mMainWindow->getStatusPosRealHeight();

        Point2f p0,p1,p2,p3;

        p0 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(0,0,height));
        p1 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(maxPerimeter,0,height));
        p2 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(maxPerimeter,maxPerimeter,height));
        p3 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(0,maxPerimeter,height));

        painter->setPen(qRgb(0,0,0));
        painter->drawLine(p0.x,+p0.y,p1.x,p1.y);
        painter->drawLine(p1.x,+p1.y,p2.x,p2.y);
        painter->drawLine(p2.x,+p2.y,p3.x,p3.y);
        painter->drawLine(p3.x,p3.y,p0.x,p0.y);


        p0 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(0,0,height));
        p1 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(minPerimeter,0,height));
        p2 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(minPerimeter,minPerimeter,height));
        p3 = mMainWindow->getExtrCalibration()->getImagePoint(Point3f(0,minPerimeter,height));

        painter->setPen(qRgb(255,255,255));
        painter->drawLine(p0.x,p0.y,p1.x,p1.y);
        painter->drawLine(p1.x,p1.y,p2.x,p2.y);
        painter->drawLine(p2.x,p2.y,p3.x,p3.y);
        painter->drawLine(p3.x,p3.y,p0.x,p0.y);

    }

}

void CodeMarkerItem::setDetectedMarkers(vector<vector<Point2f> > corners, vector<int> ids)
{
    mCorners = corners;
    mIds = ids;
}

void CodeMarkerItem::setRejectedMarkers(vector<vector<Point2f> > rejected)
{
    mRejected = rejected;
}
