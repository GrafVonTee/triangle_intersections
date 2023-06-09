#ifndef TRIANGLE_INTERSECTIONS_GETIMVECFROMPOLYGON_H
#define TRIANGLE_INTERSECTIONS_GETIMVECFROMPOLYGON_H

#include <tuple>
#include <vector>
#include <algorithm>
#include "imgui.h"
#include "GeometryUtils.h"

namespace DrawUtils {

    struct scalingParameters {
        float scaleX = 0;
        float scaleY = 0;
        float deltaX = 0;
        float deltaY = 0;
        float minX = 0;
        float minY = 0;
    };

    std::vector<ImVec2> getVectorOfPointsFromPolygon(const Geometry::Polygon& polygon,
                                                     ImVec2 offset = ImVec2(0,0));
    ImVec2 getImVec2(const Geometry::Point &p,
                     ImVec2 offset = ImVec2(0,0));

    scalingParameters findParameters(const Geometry::Polygon &tr1, 
                        const Geometry::Polygon &tr2,
                        coord_t squareSideSize);

    Geometry::Polygon scaleAndTranslate(const Geometry::Polygon &polygon,
                                        const scalingParameters& parameters);

    Geometry::Point scaleAndTranslatePoint(const Geometry::Point &point, 
                                        const scalingParameters& parameters);

    size_t *checkAvailableLabels(Geometry::Polygon &polygon);

    void setActualLabels(Geometry::Polygon &polygon, 
                         char polygonLetter = 0);

    void setActualPointsLabels(Geometry::Polygon &polygon1,
                               Geometry::Polygon &polygon2,
                               Geometry::Intersection &intersection);
}
#endif //TRIANGLE_INTERSECTIONS_GETIMVECFROMPOLYGON_H