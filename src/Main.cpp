#include <chrono>
#include "SmallestEnclosingCircle.hpp"
#include "Tester.hpp"
#include "Generator.hpp"

/** 実験に使用するパラメータ群 */
constexpr double EPSILON = 1e-8;
constexpr int32_t N = 100;

enum class ApplicationMode {
    FastTest,
    RandomTest,
    GenerateInput
};
// Modify this line to change the behavior of this application.
constexpr ApplicationMode mode = ApplicationMode::GenerateInput;

void Main() {
    switch (mode)
    {
        case ApplicationMode::FastTest:
            
            break;
        case ApplicationMode::RandomTest:
            VisualRandomTest();
            break;
        case ApplicationMode::GenerateInput:
            GenerateLargeInputs();
            break;
    }
}
