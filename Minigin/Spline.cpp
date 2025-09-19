#include "Spline.h"

glm::vec2 dae::Spline2D::Evaluate(float t) const
{
    if (m_SplinePoints.empty()) return glm::vec2(0.0f);

    size_t numPoints = m_SplinePoints.size();
    float segmentT = t * numPoints;
    size_t index = static_cast<size_t>(segmentT);
    if (index >= (numPoints - 1))
    {
        return m_SplinePoints[index].Point;
    }

    const SplinePoint2D& currentPoint = m_SplinePoints[index];
    const SplinePoint2D& nextPoint = m_SplinePoints[index + 1];

    float localT = segmentT - index;
    return Evaluate(
        localT,
        currentPoint.Point,
        currentPoint.RightControlPoint,
        nextPoint.LeftControlPoint,
        nextPoint.Point);
}

glm::vec2 dae::Spline2D::Evaluate(float t, const glm::vec2& startPoint, const glm::vec2& controlPointOne, const glm::vec2& controlPointTwo, const glm::vec2& endPoint) const
{
    float u = 1.0f - t;
    float u2 = u * u;
    float u3 = u2 * u;
    float t2 = t * t;
    float t3 = t2 * t;

    return (u3 * startPoint) + (3 * u2 * t * controlPointOne) + (3 * u * t2 * controlPointTwo) + (t3 * endPoint);
}
