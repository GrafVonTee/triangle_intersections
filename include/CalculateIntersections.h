#ifndef POLYGON_INTERSECTIONS_CALCULATEINTERSECTIONS_H
#define POLYGON_INTERSECTIONS_CALCULATEINTERSECTIONS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "GeometryUtils.h"
#include "StatesUtils.h"

namespace Math {

// Important case - it is when 2 lines not parallel and sections have intersection.
// If case is important, we get coords of intersection point, else we get (0,0) point
    struct LineIntersection{
        Geometry::Point point;
        bool isImportantCase;
    };

    // Determinant of the 2-dimensional matrix
    coord_t det(coord_t a1, coord_t a2 ,coord_t b1, coord_t b2);

    // Pushing point into vector only if it is a new point of intersection
    void pushBackUnique(std::vector<Geometry::Point>& list, const Geometry::Point& point);

    // Checking that point is locating into polygon (inside on the border)
    bool isPointInside(const Geometry::Point& point, const Geometry::Polygon& figure);

    // Adding into vector all vertex of "internal" polygon which is locating into "external polygon"
    void addInsideVertex(std::vector<Geometry::Point>& list, const Geometry::Polygon& internal, const Geometry::Polygon& external);

    // Important case - it is when 2 lines not parallel and sections have intersection.
    // If case is important, we get coords of intersection point, else we get (0,0) point
    LineIntersection findLinesInter(const Geometry::Point &firstStart,  const Geometry::Point &firstEnd,
                                    const Geometry::Point &secondStart, const Geometry::Point &secondEnd);

    // Main function of math module, find intersection area
    Geometry::Intersection findPolygonsInter(const Geometry::Polygon &first, const Geometry::Polygon &second);

}
#endif //POLYGON_INTERSECTIONS_CALCULATEINTERSECTIONS_H