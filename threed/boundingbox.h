//----------------------------------------------------------------------------
// ThreeD Bounding Box
//----------------------------------------------------------------------------

#ifndef _THREED_BOUNDING_BOX_H
#define _THREED_BOUNDING_BOX_H

#include <threed/vector.h>
#include <threed/transform.h>
#include <list>

namespace ThreeD {


/**
 * Bounding Box
 */
class BoundingBox
{
public:
    /**
     * Constructor.
     *
     * Constructs an empty bounding box.
     */
    BoundingBox() {}

    /**
     * Constructor.
     *
     * Constructs a bounding box initialized with the specified
     * bounds.
     */
    BoundingBox(const Vector &v1, const Vector &v2)
    {
        set(v1, v2);
    }

    /**
     * Constructor.
     *
     * Constructs a bounding box initialized by the bounds of
     * another bounding box object.
     */
    BoundingBox(const BoundingBox &other)
    {
        _vmin = other._vmin;
        _vmax = other._vmax;
    }

    /**
     * Sets the bounds of this bounding box.
     */
    void set(const Vector &v1, const Vector &v2)
    {
        vminmax(v1, v2, &_vmin, &_vmax);
    }

    /**
     * Merges the specified bounding box into this bounding box.
     */
    void merge(const BoundingBox &bbox2);

    /**
     * Merges the specified bounds into this bounding box.
     */
    void merge(const Vector &v1, const Vector &v2)
    {
        BoundingBox bbox;
        vminmax(v1, v2, &bbox._vmin, &bbox._vmax);
        merge(bbox);
    }

    /**
     * Returns the minimum and maximum of two values
     */
    static inline void fminmax(
        float f1, float f2,
        float *fmin, float *fmax);

    /**
     * Returns the minimum and maximum of two vectors
     */
    static void vminmax(
        const Vector &v1, const Vector &v2,
        Vector *vmin, Vector *vmax);

    /**
     * Split the bounding box by a point.  The operation will
     * result in eight bounding boxes covering the volume of
     * the original bounding box.  Each one of the eight
     * boxes will have the point as one of its corners.
     */
    inline bool splitByPoint(
        const Vector &point,
        std::list<BoundingBox> *outBoxes) const;

    /**
     * Split the bounding box by another bounding box.  The
     * operation will result in a number of bounding boxes
     * covering the volume of the original bounding boxes,
     * but no two boxes will share the same volume.
     */
    bool splitByBox(
        const BoundingBox &bbox2,
        std::list<BoundingBox> *outBoxes) const;

    /**
     * Intersect two bounding boxes
     */
    static bool splitTwoBoxes(
        const BoundingBox &bbox1, const BoundingBox &bbox2,
        std::list<BoundingBox> *outBoxes);

    /**
     * Transform this bounding box
     */
    void transform(const Transform &trans);

    /**
     * @return the minimum point of the bounding box
     */
    const Vector &vmin() const { return _vmin; }

    /**
     * @return the maximum point of the bounding box
     */
    const Vector &vmax() const { return _vmax; }

    /**
     * corners
     */
    Vector vTopLeftNear()  const { return Vector(_vmin.x(), _vmax.y(), _vmin.z()); }
    Vector vTopLeftFar()   const { return Vector(_vmin.x(), _vmax.y(), _vmax.z()); }
    Vector vBotLeftNear()  const { return _vmin; }
    Vector vBotLeftFar()   const { return Vector(_vmin.x(), _vmin.y(), _vmax.z()); }
    Vector vTopRightNear() const { return Vector(_vmax.x(), _vmax.y(), _vmin.z()); }
    Vector vTopRightFar()  const { return _vmax; }
    Vector vBotRightNear() const { return Vector(_vmax.x(), _vmin.y(), _vmin.z()); }
    Vector vBotRightFar()  const { return Vector(_vmax.x(), _vmin.y(), _vmax.z()); }

    /**
     * dimensions
     */
    float width() const { return (_vmax.x() - _vmin.x()); }
    float height() const { return (_vmax.y() - _vmin.y()); }
    float depth() const { return (_vmax.z() - _vmin.z()); }

    /**
     * @return true if this bounding box is wholly contained by
     * a larger bounding box @p bbox2
     */
    bool isInside(const BoundingBox &bbox2) const
    {
        const Vector &vmin1 = _vmin;
        const Vector &vmax1 = _vmax;
        const Vector &vmin2 = bbox2._vmin;
        const Vector &vmax2 = bbox2._vmax;

        return (
            vmin2.x() >= vmin1.x() && vmax2.x() <= vmax1.x() &&
            vmin2.y() >= vmin2.y() && vmax2.y() <= vmax2.y() &&
            vmin2.z() >= vmin2.z() && vmax2.z() <= vmax2.z());
    }

    /**
     * @return true if this bounding box is overlapping
     * with another box @p bbox2
     */
    bool overlapping(const BoundingBox &bbox2) const
    {
        return overlapping(*this, bbox2);
    }

    /**
     * @return true if two bounding boxes are overlapping
     */
    static bool overlapping(
        const BoundingBox &bbox1, const BoundingBox &bbox2)
    {
        const Vector &vmin1 = bbox1._vmin;
        const Vector &vmax1 = bbox1._vmax;
        const Vector &vmin2 = bbox2._vmin;
        const Vector &vmax2 = bbox2._vmax;

        bool non_overlapping =
            (vmax1.x() <= vmin2.x() || vmax2.x() <= vmin1.x() ||
             vmax1.y() <= vmin2.y() || vmax2.y() <= vmin1.y() ||
             vmax1.z() <= vmin2.z() || vmax2.z() <= vmin1.z());
        return !non_overlapping;
    }

    /**
     * @return the intersection bounding box of two bounding boxes
     * @p bbox1 and @p bbox2 which are known to be overlapping
     */
    static BoundingBox intersection(
        const BoundingBox &bbox1, const BoundingBox &bbox2)
    {
        const Vector &vmin1 = bbox1._vmin;
        const Vector &vmax1 = bbox1._vmax;
        const Vector &vmin2 = bbox2._vmin;
        const Vector &vmax2 = bbox2._vmax;

        BoundingBox bbox;
        bbox._vmin = Vector(THREED_MAX(vmin1.x(), vmin2.x()),
                            THREED_MAX(vmin1.y(), vmin2.y()),
                            THREED_MAX(vmin1.z(), vmin2.z()));
        bbox._vmax = Vector(THREED_MIN(vmax1.x(), vmax2.x()),
                            THREED_MIN(vmax1.y(), vmax2.y()),
                            THREED_MIN(vmax1.z(), vmax2.z()));

        return bbox;
    }

    /**
     * Comparison ==
     */
    bool operator==(const BoundingBox &other) const
    {
        return (_vmin == other._vmin && _vmax == other._vmax);
    }

    /**
     * Comparison !=
     */
    bool operator!=(const BoundingBox &other) const
    {
        return (_vmin != other._vmin || _vmax != other._vmax);
    }

protected:

    /*
     * data
     */

    Vector _vmin, _vmax;
};


} // namespace ThreeD
#endif // _THREED_BOUNDING_BOX_H
