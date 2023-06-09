#ifndef TRIANGLE_INTERSECTIONS_STATES_LIBRARY_H
#define TRIANGLE_INTERSECTIONS_STATES_LIBRARY_H

#include <vector>
#include "GeometryUtils.h"
#include "StatesUtils.h"

namespace Manipulator {
    struct FiguresState {
        Geometry::Polygon polygon1;
        Geometry::Polygon polygon2;
        Geometry::Intersection intersection;
    };

    class StatesLibrary {
    private:
        std::vector<FiguresState> m_states;

        StatesLibrary() = default;

    public:
        StatesLibrary(const StatesLibrary &) = delete;
        StatesLibrary &operator=(StatesLibrary &) = delete;

        static StatesLibrary &getInstance() {
            static StatesLibrary instance;
            return instance;
        }

        bool isEmpty() const;
        size_t getSize() const;

        const FiguresState& getStateView(size_t stateIndex = -1) const;
        FiguresState getStateCopy(size_t stateIndex = -1) const;
        FiguresState &getStateRef(size_t stateIndex = -1);

        void addState(const FiguresState &state);
        void emplaceState(FiguresState &&state);
        void emplaceState(FiguresState &state);

        void updateState();
        void updateStateWith(const Geometry::Polygon &, States::FigureName);

        void popState();
    };
}
#endif //TRIANGLE_INTERSECTIONS_STATES_LIBRARY_H