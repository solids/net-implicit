//----------------------------------------------------------------------------
// ThreeD Camera
//----------------------------------------------------------------------------

#include <threed/boundingbox.h>

using namespace ThreeD;

#define min THREED_MIN
#define max THREED_MAX

//----------------------------------------------------------------------------

void BoundingBox::merge(const BoundingBox &bbox2)
{
    const Vector &vmin = bbox2._vmin;
    const Vector &vmax = bbox2._vmax;

    if (_vmin == Vector() && _vmax == Vector()) {
        _vmin = vmin;
        _vmax = vmax;
        return;
    }

    float xmin = min(vmin.x(), _vmin.x());
    float ymin = min(vmin.y(), _vmin.y());
    float zmin = min(vmin.z(), _vmin.z());
    float xmax = max(vmax.x(), _vmax.x());
    float ymax = max(vmax.y(), _vmax.y());
    float zmax = max(vmax.z(), _vmax.z());

    _vmin = Vector(xmin, ymin, zmin);
    _vmax = Vector(xmax, ymax, zmax);
}

//----------------------------------------------------------------------------

void BoundingBox::fminmax(
    float f1, float f2,
    float *fmin, float *fmax)
{
    if (f1 < f2) {
        *fmin = f1;
        *fmax = f2;
    } else {
        *fmin = f2;
        *fmax = f1;
    }
}

//----------------------------------------------------------------------------

void BoundingBox::vminmax(
    const Vector &v1, const Vector &v2,
    Vector *vmin, Vector *vmax)
{
    float xmin, ymin, zmin;
    float xmax, ymax, zmax;
    fminmax(v1.x(), v2.x(), &xmin, &xmax);
    fminmax(v1.y(), v2.y(), &ymin, &ymax);
    fminmax(v1.z(), v2.z(), &zmin, &zmax);
    *vmin = Vector(xmin, ymin, zmin);
    *vmax = Vector(xmax, ymax, zmax);
}

//----------------------------------------------------------------------------

bool BoundingBox::splitByPoint(
    const Vector &point,
    std::list<BoundingBox> *outBoxes) const
{
    // skip processing if point is outside bounding box
    if (point.x() < _vmin.x() || point.x() > _vmax.x() ||
        point.y() < _vmin.y() || point.y() > _vmax.y() ||
        point.z() < _vmin.z() || point.z() > _vmax.z())
        return false;

    // skip processing if point is one of the eight corners
    // of the bounding box
    if ((point.x() == _vmin.x() || point.x() == _vmax.x()) &&
        (point.y() == _vmin.y() || point.y() == _vmax.y()) &&
        (point.z() == _vmin.z() || point.z() == _vmax.z()))
        return false;

    // construct new bounding boxes, two boxes at a time,
    // the near box followed by the far box
    BoundingBox bboxes[8];
    int num = 0;
    Vector vmin, vmax;

    // bottom left
    bboxes[num]._vmin = Vector(_vmin.x(), _vmin.y(), _vmin.z());
    bboxes[num]._vmax = Vector(point.x(), point.y(), point.z());
    ++num;

    bboxes[num]._vmin = Vector(_vmin.x(), _vmin.y(), point.z());
    bboxes[num]._vmax = Vector(point.x(), point.y(), _vmax.z());
    ++num;

    // bottom right
    bboxes[num]._vmin = Vector(point.x(), _vmin.y(), _vmin.z());
    bboxes[num]._vmax = Vector(_vmax.x(), point.y(), point.z());
    ++num;

    bboxes[num]._vmin = Vector(point.x(), _vmin.y(), point.z());
    bboxes[num]._vmax = Vector(_vmax.x(), point.y(), _vmax.z());
    ++num;

    // top left
    bboxes[num]._vmin = Vector(_vmin.x(), point.y(), _vmin.z());
    bboxes[num]._vmax = Vector(point.x(), _vmax.y(), point.z());
    ++num;

    bboxes[num]._vmin = Vector(_vmin.x(), point.y(), point.z());
    bboxes[num]._vmax = Vector(point.x(), _vmax.y(), _vmax.z());
    ++num;

    // top right
    bboxes[num]._vmin = Vector(point.x(), point.y(), _vmin.z());
    bboxes[num]._vmax = Vector(_vmax.x(), _vmax.y(), point.z());
    ++num;

    bboxes[num]._vmin = Vector(point.x(), point.y(), point.z());
    bboxes[num]._vmax = Vector(_vmax.x(), _vmax.y(), _vmax.z());
    ++num;

    // add those boxes that aren't degenerate
    for (int i = 0; i < 8; ++i) {
        const BoundingBox &bbox = bboxes[i];
        if (bbox._vmin.x() == bbox._vmax.x() ||
            bbox._vmin.y() == bbox._vmax.y() ||
            bbox._vmin.z() == bbox._vmax.z())
            continue;
        outBoxes->push_back(bbox);
    }

    return true;
}

//----------------------------------------------------------------------------

bool BoundingBox::splitByBox(
    const BoundingBox &bbox2,
    std::list<BoundingBox> *outBoxes) const
{
    const Vector &vmin2 = bbox2.vmin();
    const Vector &vmax2 = bbox2.vmax();

    // compute the coordinates of the eight corners of bbox2
    Vector corners[8];
    int num = 0;
    // bottom left
    corners[num++] = vmin2;
    corners[num++] = Vector(vmin2.x(), vmin2.y(), vmax2.z());
    // bottom right
    corners[num++] = Vector(vmax2.x(), vmin2.y(), vmin2.z());
    corners[num++] = Vector(vmax2.x(), vmin2.y(), vmax2.z());
    // top left
    corners[num++] = Vector(vmin2.x(), vmax2.y(), vmin2.z());
    corners[num++] = Vector(vmin2.x(), vmax2.y(), vmax2.z());
    // top right
    corners[num++] = Vector(vmax2.x(), vmax2.y(), vmin2.z());
    corners[num++] = Vector(vmax2.x(), vmax2.y(), vmax2.z());

    // split this box by the computed corners of bbox2
    std::list<BoundingBox> tempBoxes;
    tempBoxes.push_back(*this);

    bool anySplits = false;

    for (int i = 0; i < 8; ++i) {
        const Vector &point = corners[i];
        std::list<BoundingBox>::iterator it = tempBoxes.begin();
        while (it != tempBoxes.end()) {
            const BoundingBox &tempBox = (*it);
            if (tempBox.splitByPoint(point, &tempBoxes)) {
                it = tempBoxes.erase(it);
                anySplits = true;
            } else
                ++it;
        }
    }

    if (! anySplits)
        return false;

    outBoxes->splice(outBoxes->end(), tempBoxes);
    return true;
}

//----------------------------------------------------------------------------

bool BoundingBox::splitTwoBoxes(
    const BoundingBox &bbox1, const BoundingBox &bbox2,
    std::list<BoundingBox> *outBoxes)
{
    std::list<BoundingBox> boxes1, boxes2;

    // split bbox1 by bbox2.  if cannot, then just return the
    // split of bbox2 by bbox1
    if (!bbox1.splitByBox(bbox2, &boxes1))
        return bbox2.splitByBox(bbox1, outBoxes);

    std::list<BoundingBox>::const_iterator it1 = boxes1.begin();
    while (it1 != boxes1.end()) {
        const BoundingBox &box = (*it1);
        ++it1;
        bbox2.splitByBox(box, &boxes2);
    }

    // find and remove duplicate boxes
    it1 = boxes1.begin();
    while (it1 != boxes1.end()) {
        const BoundingBox &box1 = (*it1);
        ++it1;

        std::list<BoundingBox>::iterator it2 = boxes2.begin();
        while (it2 != boxes2.end()) {
            const BoundingBox &box2 = (*it2);
            if (box2 == box1)
                it2 = boxes2.erase(it2);
            else
                ++it2;
        }

        outBoxes->push_back(box1);
    }

    //outBoxes->splice(outBoxes->end(), boxes1);
    outBoxes->splice(outBoxes->end(), boxes2);
    return true;
}

//----------------------------------------------------------------------------

void BoundingBox::transform(const Transform &trans)
{
    Vector v1 = _vmin;
    Vector v8 = _vmax;
    Vector v2(v1.x(), v1.y(), v8.z());
    Vector v3(v1.x(), v8.y(), v1.z());
    Vector v4(v1.x(), v8.y(), v8.z());
    Vector v5(v8.x(), v1.y(), v1.z());
    Vector v6(v8.x(), v1.y(), v8.z());
    Vector v7(v8.x(), v8.y(), v1.z());

    trans.transform(&v1);
    trans.transform(&v2);
    trans.transform(&v3);
    trans.transform(&v4);
    trans.transform(&v5);
    trans.transform(&v6);
    trans.transform(&v7);
    trans.transform(&v8);

    /*merge(v1, v2);
    merge(v3, v4);
    merge(v5, v6);
    merge(v7, v8);*/

    set(v1, v8);
    merge(v2, v7);
    merge(v3, v6);
    merge(v4, v5);
}
