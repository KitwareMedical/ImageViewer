#include "BoxWidget.h"
#include "QtGlSliceView.h"
#include <Qt>
#include <QtOpenGL/qgl.h>
#include <QMouseEvent>
#include <QGuiApplication>
#include <cmath>
#include "itkMath.h"

bool operator< (std::unique_ptr< BoxToolMetaData > const& lhs, std::unique_ptr< BoxToolMetaData > const& rhs) {
    return lhs->sortId < rhs->sortId;
}

std::unique_ptr< BoxToolMetaData > ConstantBoxMetaDataGenerator::operator()(void) {
    std::string name;
    if (mName.size() > 0) {
        name = mName;
    } else {
        std::string name = std::to_string(curId);
    }

    QColor color = mColor;
    // QColor color = QColor(QString(mColor.c_str()));

    int id = curId;
    ++curId;
    return std::unique_ptr< BoxToolMetaData >(new BoxToolMetaData{ id, name, color });
}


BoxToolMetaDataFactory::BoxToolMetaDataFactory(std::unique_ptr< BoxMetaDataGenerator > generator) : generator{ std::move(generator) } { }

std::unique_ptr< BoxToolMetaData > BoxToolMetaDataFactory::getNext() {
    std::unique_ptr< BoxToolMetaData > ans;
    if (refunds.size() > 0) {
        ans = std::move(*(refunds.begin()));
        refunds.erase(refunds.begin());
    }
    else {
        ans = (*generator)();
    }
    return ans;
}

void BoxToolMetaDataFactory::refund(std::unique_ptr< BoxToolMetaData > box_meta) {
    refunds.push_back(std::move(box_meta));
    std::sort(refunds.begin(), refunds.end());
}

BoxTool::BoxTool(QtGlSliceView* parent0, PointType3D index0, std::unique_ptr< BoxToolMetaData > metaData) :
    parent{ parent0 }, indices{ index0, index0 }, points{ parent0->indexToPhysicalPoint(index0), parent0->indexToPhysicalPoint(index0) }, metaData{ std::move(metaData) }, clickRadius{ 10.0 }, state{ BoxToolState::drawing }, floatingIndex{ 1 }, crossStart{ 18 }, crossEnd{ 3 }, lineWidth{ 4 }
{

}

BoxTool::~BoxTool() {}

int BoxTool::isOver(double index[]) const {
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

void BoxTool::updateFloatingIndex(double index[]) {
    this->indices[this->floatingIndex] = PointType3D(index);
    this->points[this->floatingIndex] = this->parent->indexToPhysicalPoint(index);
}


void BoxTool::setFloatingIndex(int id) {
    this->floatingIndex = id;
}

int BoxTool::getFloatingIndex() const {
    return this->floatingIndex;
}

double BoxTool::area(int axis) {
    // axis is one of X_AXIS, Y_AXIS, Z_AXIS, and assumes
    // they correspond to 0, 1, 2 values.
    int side1 = (axis + 1) % 3;
    int side2 = (axis + 2) % 3;
    return std::abs(
        (this->points[0][side1] - this->points[1][side1]) *
        (this->points[0][side2] - this->points[1][side2]));
}

void BoxTool::paint() {

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

    // box
    painter.drawLine(QLine(screen0[0], screen0[1], screen0[0], screen1[1]));
    painter.drawLine(QLine(screen0[0], screen1[1], screen1[0], screen1[1]));
    painter.drawLine(QLine(screen1[0], screen1[1], screen1[0], screen0[1]));
    painter.drawLine(QLine(screen1[0], screen0[1], screen0[0], screen0[1]));
}

std::string BoxTool::toJson() {
    char txt[256];

    int n = snprintf(txt, sizeof(txt),
        "{ \"name\" : \"%s\", \"indices\" : [ [%.4f, %.4f, %.4f], [%.4f, %.4f, %.4f] ], \"points\" : [ [%.4f, %.4f, %.4f], [%.4f, %.4f, %.4f] ]}",
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
        points[1].GetElement(2));

    if (n >= sizeof(txt)) {
        throw "BoxTool::toJson tried to write too big of string";
    }
    return std::string(txt);
}

BoxToolCollection::BoxToolCollection(QtGlSliceView* parent, std::shared_ptr< BoxToolMetaDataFactory > metaDataFactory, unsigned short axis, unsigned int slice)
    : parent{ parent }, metaDataFactory{ metaDataFactory }, axis{ axis }, slice{ slice }, currentId { -1 }, state { BoxToolState::standing }
{

}

BoxToolCollection::~BoxToolCollection() {}

void BoxToolCollection::handleMouseEvent(QMouseEvent* event, double index[]) {

    int over = -1;
    switch (this->state) {
    case BoxToolState::standing:
        over = isOver(index);
        if (over >= 0) {
            if (QGuiApplication::overrideCursor() == nullptr) {
                QGuiApplication::setOverrideCursor(Qt::OpenHandCursor);
            }
            if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::RightButton) { // we're over a box and have right-clicked to move the crosshair
                this->currentId = over;
                BoxTool* r = boxes[currentId].get();
                r->setFloatingIndex(r->isOver(index));
                this->state = BoxToolState::drawing;
                r->paint();
            }
        }
        else {
            QGuiApplication::restoreOverrideCursor();
        }

        // we're idle and we're clicking to create a new box
        if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::LeftButton) {
            // blank click on screen, make a box

            std::unique_ptr< BoxTool > r(new BoxTool(this->parent, BoxTool::PointType3D(index), metaDataFactory->getNext()));
            this->boxes.push_back(std::move(r));
            this->currentId = this->boxes.size() - 1;
            this->state = BoxToolState::drawing;
            this->paint();
        }
        break;
    case BoxToolState::drawing:
        if (QGuiApplication::overrideCursor() != nullptr) {
            QGuiApplication::restoreOverrideCursor(); // if we are drawing go back to a normal cursor, even if we're over a box
        }
        // we have one free end of the box and we are clicking to set it
        if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::LeftButton) {
            BoxTool* r = boxes[currentId].get();
            r->updateFloatingIndex(index);
            this->state = BoxToolState::standing;
        }
        // actively moving the free end
        else if (event->type() == QEvent::MouseMove) {
            BoxTool* r = boxes[currentId].get();
            r->updateFloatingIndex(index);
        }
        else if (event->type() == QEvent::MouseButtonRelease && event->button() == Qt::RightButton) { // delete and go back to standing
            metaDataFactory->refund(std::move(boxes[currentId]->metaData));
            boxes.erase(boxes.begin() + currentId);
            currentId = boxes.size() > 0 ? 0 : -1;
            this->state = BoxToolState::standing;
        }
        break;
    }

}

void BoxToolCollection::setMetaDataFactory(std::shared_ptr< BoxToolMetaDataFactory > factory) {
    metaDataFactory = factory;
}

void BoxToolCollection::paint() {
    for (auto &r : this->boxes) {
        r->paint();
    }
}

int BoxToolCollection::isOver(double index[]) {
    for (int i=0; i < boxes.size(); ++i) {
        auto r = boxes[i].get();
        int x = r->isOver(index);
        if (x >= 0) {
            return i; // return the index of the box (different than BoxTool::isOver() which returns the index of the box point covered)
        }
    }
    return -1;
}

BoxTool* BoxToolCollection::getActive() {
    return currentId >= 0 ? boxes[currentId].get() : nullptr;
}

std::string BoxToolCollection::toJson() {
    std::string ans = "{ \"axis\" : " + std::to_string(axis) + ", \"slice\" : " + std::to_string(slice) + ", \"boxes\" : [";
    if (boxes.size() > 0) {
        ans += boxes[0]->toJson();
    }
    for (auto iter = boxes.begin() + 1; iter != boxes.end(); ++iter) {
        ans += "," + (*iter)->toJson();
    }
    ans += "]}";
    return ans;
}
