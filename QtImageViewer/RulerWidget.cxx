#include "RulerWidget.h"
#include "QtGlSliceView.h"
#include <Qt>
#include <QtOpenGL/qgl.h>
#include <QMouseEvent>
#include <QGuiApplication>
#include <cmath>
#include "itkMath.h"

bool operator< (std::unique_ptr< RulerToolMetaData > const& lhs, std::unique_ptr< RulerToolMetaData > const& rhs) {
    return lhs->sortId < rhs->sortId;
}

RainbowMetaDataGenerator::RainbowMetaDataGenerator() { 
    curColor = colors.begin(); 
}

std::unique_ptr< RulerToolMetaData > RainbowMetaDataGenerator::operator()(void) {
    std::string name = std::to_string(curId);
    QColor color = QColor(QString(curColor->c_str()));
    int id = curId;

    // wrap around to the beginning of the color list if at end
    ++curColor;
    if (curColor == colors.end()) {
        curColor = colors.begin();
    }

    ++curId;
    return std::unique_ptr< RulerToolMetaData >(new RulerToolMetaData{ id, name, color });
}

std::unique_ptr< RulerToolMetaData > ONSDMetaDataGenerator::operator()(void) {
    std::string name = flipper ? "R1" : "ONSD";
    QColor color = QColor(colors[(int)flipper].c_str());
    int id = curId;

    flipper = !flipper;
    ++curId;

    return std::unique_ptr< RulerToolMetaData >(new RulerToolMetaData{ id, name, color });
}


RulerToolMetaDataFactory::RulerToolMetaDataFactory(std::unique_ptr< MetaDataGenerator > generator) : generator{ std::move(generator) } { }

std::unique_ptr< RulerToolMetaData > RulerToolMetaDataFactory::getNext() {
    std::unique_ptr< RulerToolMetaData > ans;
    if (refunds.size() > 0) {
        ans = std::move(*(refunds.begin()));
        refunds.erase(refunds.begin());
    }
    else {
        ans = (*generator)();
    }
    return ans;
}

void RulerToolMetaDataFactory::refund(std::unique_ptr< RulerToolMetaData > ruler_meta) {
    refunds.push_back(std::move(ruler_meta));
    std::sort(refunds.begin(), refunds.end());
}

RulerTool::RulerTool(QtGlSliceView* parent0, PointType3D &index0, std::unique_ptr< RulerToolMetaData > metaData) :
    parent{ parent0 }, indices{ index0, index0 }, points{ parent0->indexToPhysicalPoint(index0), parent0->indexToPhysicalPoint(index0) }, metaData{ std::move(metaData) }, clickRadius{ 10.0 }, state{ RulerToolState::drawing }, floatingIndex{ 1 }, crossStart{ 18 }, crossEnd{ 3 }, lineWidth{ 4 }
{

}

RulerTool::~RulerTool() {}

int RulerTool::isOver(double index[]) const {
    int ans = -1;
    auto screenPt = parent->indexToScreenPoint(PointType3D(index));
    if (screenPt.EuclideanDistanceTo(parent->indexToScreenPoint(indices[0])) < clickRadius) {
        ans = 0;
    }
    else if (screenPt.EuclideanDistanceTo(parent->indexToScreenPoint(indices[1])) < clickRadius) {
        ans = 1;
    }
    return ans;
}

void RulerTool::updateFloatingIndex(double index[]) {
    this->indices[this->floatingIndex] = PointType3D(index);
    this->points[this->floatingIndex] = this->parent->indexToPhysicalPoint(index);
}


void RulerTool::setFloatingIndex(int id) {
    this->floatingIndex = id;
}

int RulerTool::getFloatingIndex() const {
    return this->floatingIndex;
}

double RulerTool::length() {
    return this->points[0].EuclideanDistanceTo(this->points[1]);
}

void RulerTool::paint() { 

    QPainter painter(this->parent);
    QPen pen;

    // could optimize this by memoization and catching parent stage changes
    PointType2D screen0 = parent->indexToScreenPoint(indices[0]);
    PointType2D screen1 = parent->indexToScreenPoint(indices[1]);

    QColor color = metaData->color;
    color.setAlphaF(0.7);
    pen.setWidth(lineWidth);
    pen.setColor(color);
    painter.setPen(pen);

    // TODO make line dotted

    // crosshair1
    painter.drawLine(QLine(screen0[0] - crossStart, screen0[1] - crossStart, screen0[0] - crossEnd, screen0[1] - crossEnd));
    painter.drawLine(QLine(screen0[0] + crossStart, screen0[1] + crossStart, screen0[0] + crossEnd, screen0[1] + crossEnd));
    painter.drawLine(QLine(screen0[0] - crossStart, screen0[1] + crossStart, screen0[0] - crossEnd, screen0[1] + crossEnd));
    painter.drawLine(QLine(screen0[0] + crossStart, screen0[1] - crossStart, screen0[0] + crossEnd, screen0[1] - crossEnd));

    // crosshair2
    painter.drawLine(QLine(screen1[0] - crossStart, screen1[1] - crossStart, screen1[0] - crossEnd, screen1[1] - crossEnd));
    painter.drawLine(QLine(screen1[0] + crossStart, screen1[1] + crossStart, screen1[0] + crossEnd, screen1[1] + crossEnd));
    painter.drawLine(QLine(screen1[0] - crossStart, screen1[1] + crossStart, screen1[0] - crossEnd, screen1[1] + crossEnd));
    painter.drawLine(QLine(screen1[0] + crossStart, screen1[1] - crossStart, screen1[0] + crossEnd, screen1[1] - crossEnd));

    QPen pen2;
    pen2.setWidth(lineWidth);
    pen2.setColor(color);
    pen2.setStyle(Qt::DashDotLine);
    painter.setPen(pen2);

    //line
    painter.drawLine(QLine(screen0[0], screen0[1], screen1[0], screen1[1]));
}

std::string RulerTool::toJson() {
    char txt[256];

    int n = snprintf(txt, sizeof(txt),
        "{ \"name\" : \"%s\", \"indices\" : [ [%.4f, %.4f, %.4f], [%.4f, %.4f, %.4f] ], \"points\" : [ [%.4f, %.4f, %.4f], [%.4f, %.4f, %.4f] ], \"distance\" : %.4f}",
        metaData->name.c_str(),
        indices[0].GetElement(0),
        indices[0].GetElement(1),
        indices[0].GetElement(2),
        indices[1].GetElement(0),
        indices[1].GetElement(1),
        indices[1].GetElement(2),
        points[0].GetElement(0),
        points[0].GetElement(1),
        points[0].GetElement(2),
        points[1].GetElement(0),
        points[1].GetElement(1),
        points[1].GetElement(2),
        points[0].EuclideanDistanceTo(points[1]));

    if (n >= sizeof(txt)) {
        throw "RulerTool::toJson tried to write too big of string";
    }
    return std::string(txt);
}

RulerToolCollection::RulerToolCollection(QtGlSliceView* parent, std::shared_ptr< RulerToolMetaDataFactory > metaDataFactory, unsigned short axis, unsigned int slice)
    : parent{ parent }, metaDataFactory{ metaDataFactory }, axis{ axis }, slice{ slice }, currentId { -1 }, state { RulerToolState::standing }
{

}

RulerToolCollection::~RulerToolCollection() {}

void RulerToolCollection::handleMouseEvent(QMouseEvent* event, double index[]) {

    int over = -1;
    switch (this->state) {
    case RulerToolState::standing:
        over = isOver(index);
        if (over >= 0) {
            if (QGuiApplication::overrideCursor() == nullptr) {
                QGuiApplication::setOverrideCursor(Qt::OpenHandCursor);
            }
            if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::RightButton) { // we're over a ruler and have right-clicked to move the crosshair
                this->currentId = over;
                RulerTool* r = rulers[currentId].get();
                r->setFloatingIndex(r->isOver(index));
                this->state = RulerToolState::drawing;
                r->paint();
            }
        }
        else {
            QGuiApplication::restoreOverrideCursor();
        }

        // we're idle and we're clicking to create a new ruler
        if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::LeftButton) {
            // blank click on screen, make a ruler

            std::unique_ptr< RulerTool > r(new RulerTool(this->parent, RulerTool::PointType3D(index), metaDataFactory->getNext()));
            this->rulers.push_back(std::move(r));
            this->currentId = this->rulers.size() - 1;
            this->state = RulerToolState::drawing;
            this->paint();
        }
        break;
    case RulerToolState::drawing:
        if (QGuiApplication::overrideCursor() != nullptr) {
            QGuiApplication::restoreOverrideCursor(); // if we are drawing go back to a normal cursor, even if we're over a ruler
        }
        // we have one free end of the ruler and we are clicking to set it
        if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::LeftButton) {
            RulerTool* r = rulers[currentId].get();
            r->updateFloatingIndex(index);
            this->state = RulerToolState::standing;
        }
        // actively moving the free end
        else if (event->type() == QEvent::MouseMove) {
            RulerTool* r = rulers[currentId].get();
            r->updateFloatingIndex(index);
        }
        else if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::RightButton) { // delete and go back to standing
            metaDataFactory->refund(std::move(rulers[currentId]->metaData));
            rulers.erase(rulers.begin() + currentId);
            currentId = rulers.size() > 0 ? 0 : -1;
            this->state = RulerToolState::standing;
        }
        break;
    }

}

void RulerToolCollection::setMetaDataFactory(std::shared_ptr< RulerToolMetaDataFactory > factory) {
    metaDataFactory = factory;
}

void RulerToolCollection::paint() {
    for (auto &r : this->rulers) {
        r->paint();
    }
}

int RulerToolCollection::isOver(double index[]) {
    for (int i=0; i < rulers.size(); ++i) {
        auto r = rulers[i].get();
        int x = r->isOver(index);
        if (x >= 0) {
            return i; // return the index of the ruler (different than RulerTool::isOver() which returns the index of the ruler point covered)
        }
    }
    return -1;
}

RulerTool* RulerToolCollection::getActive() {
    return currentId >= 0 ? rulers[currentId].get() : nullptr;
}

std::string RulerToolCollection::toJson() {
    std::string ans = "{ \"axis\" : " + std::to_string(axis) + ", \"slice\" : " + std::to_string(slice) + ", \"rulers\" : [";
    if (rulers.size() > 0) {
        ans += rulers[0]->toJson();
    }
    for (auto iter = rulers.begin() + 1; iter != rulers.end(); ++iter) {
        ans += "," + (*iter)->toJson();
    }
    ans += "]}";
    return ans;
}