
#include "Array.hpp"

vector<double> GenerateMeshArray(double start, double end, double step) {
    vector<double> mesh;
    for (double val = start; val <= end; val += step) {
        mesh.push_back(val);
    }
    return mesh;
}
