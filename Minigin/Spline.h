#pragma once

#include <vector>
#include <glm.hpp>

namespace dae {

    enum class SplineType2D {
        EvaluationProportionalToLength, // max t == spline length -> means constant speed while evaluating spline
        EvaluationProportionalToSplinePointNumber // max t == spline point number -> means longer distances are traversed faster
    };

    struct SplinePoint2D {
        glm::vec2 LeftControlPoint, Point, RightControlPoint;
    };
    class Spline2D {
    public:

        Spline2D(SplineType2D splineType2D, int numOfSplineSegmentsToCalculateLength);
        
        //if numOfSplineSegmentsToCalculateLength < 0, then just keep the value and return cached length
        float GetLength(int numOfSplineSegmentsToCalculateLength = -1)const;

        SplineType2D GetSplineType() const { return m_SplineType2D; }

        void SetSplineType(SplineType2D splineType2D) {
            m_SplineType2D = splineType2D;
        }

        void Add(const SplinePoint2D& splinePoint) {
            m_SplinePoints.emplace_back(splinePoint);
        }

        void RemoveLast() {
            m_SplinePoints.pop_back();
        }

        glm::vec2 Evaluate(float t) const;


    private:
        std::vector<SplinePoint2D> m_SplinePoints;
        SplineType2D m_SplineType2D;
        float m_Length;
        int m_NumOfSplineSegmentsToCalculateLength;

        glm::vec2 Evaluate(
            float t,
            const glm::vec2& startPoint,
            const glm::vec2& controlPointOne,
            const glm::vec2& controlPointTwo,
            const glm::vec2& endPoint) const;
    };
}


