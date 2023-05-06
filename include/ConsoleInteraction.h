#ifndef TRIANGLE_INTERSECTIONS_CONSOLE_INTERACTION_H
#define TRIANGLE_INTERSECTIONS_CONSOLE_INTERACTION_H

#include <tuple>
#include <string>

namespace Interaction {
    using point_result_t = std::tuple<Geometry::Point, States::InputState>;
    using triangle_result_t = std::tuple<Geometry::Polygon, States::InputState>;
    using triangle_pair_t = std::tuple<Geometry::Polygon, Geometry::Polygon>;

    const int skipToTriangle1(Geometry::Letters::A),
              skipToTriangle2(Geometry::Letters::D),
              skipToIntersection(Geometry::Letters::G);

    void greeting(const std::string &userName);
    void goodbye(const std::string &userName);
    void welcomeToGui();
    std::string getUserName();
    triangle_result_t getTriangle(int numberOfTriangle, std::istream& inputStream = std::cin);
    point_result_t getPoint(Geometry::Letters letter, std::istream& inputStream = std::cin);
    bool checkPointsAreUnique(const std::vector<Geometry::Point> &vec);
    triangle_pair_t getBothTriangles(std::istream& inputStream = std::cin);

    void printPoint(const Geometry::Point &point, Geometry::Letters letter);
    void printTriangle(const Geometry::Polygon &triangle, int triangleNumber);
    void printIntersection(const Geometry::Intersection &intersection);
    void printPolygon(const Geometry::Polygon &polygon);

    std::string getTypeNameOfPolygon(const States::PolygonState &state);
}
#endif //TRIANGLE_INTERSECTIONS_CONSOLE_INTERACTION_H