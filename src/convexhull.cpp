#include "convexhull.h"

std::vector<vec2> convexHull(std::vector<vec2>& points) {
    //jarvis algorithm
    std::vector<vec2> hull;

    auto leftmost = points.begin();
    for (auto it = points.begin(); it < points.end(); ++it) {
        if ((*it)[0] < (*leftmost)[0]) {
            leftmost = it;
        }
    }

    auto p = leftmost;
    auto q = p;
    do {
        hull.push_back(*p);
        q = p + 1;
        if (q == points.end()) q = points.begin();
        for (auto r = points.begin(); r < points.end(); ++r) {
            if ((q->y - p->y) * (r->x - q->x) - (q->x - p->x) * (r->y - q->y) > 0.0f) {
                q = r;
            }
        }
        p = q;
    } while (p != leftmost);

    return hull;
}