#include "StatesLibrary.h"
#include "CalculateIntersections.h"
#include "GetImVecFromPolygon.h"

namespace Manipulator {
    bool StatesLibrary::isEmpty() const { return m_states.empty(); }

    size_t StatesLibrary::getSize() const { return m_states.size(); }

    const FiguresState& StatesLibrary::getStateView(size_t stateIndex) const {
        if (isEmpty())
            throw std::underflow_error("Library is empty! Nothing to get.");

        if (stateIndex == -1)
            return m_states.back();

        try {
            return m_states.at(stateIndex);
        } catch (const std::out_of_range &err) {
            throw std::out_of_range("Invalid index of state!");
        }
    }

    FiguresState StatesLibrary::getStateCopy(size_t stateIndex) const {
        if (isEmpty())
            throw std::underflow_error("Library is empty! Nothing to get.");

        if (stateIndex == -1)
            return m_states.back();

        try {
            return m_states.at(stateIndex);
        } catch (const std::out_of_range &err) {
            throw std::out_of_range("Invalid index of state!");
        }
    }

    FiguresState& StatesLibrary::getStateRef(size_t stateIndex) {
        if (isEmpty())
            throw std::underflow_error("Library is empty! Nothing to get.");

        if (stateIndex == -1)
            return m_states.back();
            
        try {
            return m_states.at(stateIndex);
        } catch (const std::out_of_range &err) {
            throw std::out_of_range("Invalid index of state!");
        }
    }

    void StatesLibrary::addState(const FiguresState &state) {
        m_states.push_back(state);
    }

    void StatesLibrary::emplaceState(FiguresState &state) {
        m_states.emplace_back(std::move(state));
    }

    void
    StatesLibrary::emplaceState(FiguresState&& state) {
        m_states.emplace_back(state);
    }

    void StatesLibrary::popState() {
        if (isEmpty())
            throw std::underflow_error("Library is empty! Nothing to pop.");

        m_states.pop_back();
    }

    void StatesLibrary::updateState() {
        auto &manipulator = StatesLibrary::getInstance();
        if (manipulator.isEmpty())
           return;

        auto &state = manipulator.getStateRef();
        state.intersection = Math::findPolygonsInter(state.polygon1, state.polygon2);
        DrawUtils::setActualPointsLabels(state.polygon1,
                                         state.polygon2,
                                         state.intersection);
    }

    void StatesLibrary::updateStateWith(const Geometry::Polygon &polygon, States::FigureName figname) {
        using namespace States;

        polygon.checkPolygon(polygon.getPointsCopy());
        
        auto &manipulator = StatesLibrary::getInstance();
        if (manipulator.isEmpty())
            throw std::underflow_error("Library is empty! Nothing to update.");

        auto figures = manipulator.getStateCopy();
        switch (figname) {
            case FigureName::Polygon1:
                figures.polygon1 = polygon;
                break;
            case FigureName::Polygon2:
                figures.polygon2 = polygon;
                break;
            case FigureName::Intersection:
                return;
        }

        manipulator.emplaceState(figures);
        manipulator.updateState();
    }

}