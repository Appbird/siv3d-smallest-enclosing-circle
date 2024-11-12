#include <chrono>
#include "SmallestEnclosingCircle.hpp"
#include "Tester.hpp"
#include "Generator.hpp"

enum class ApplicationMode {
    FullTest,
    RandomTest,
    GenerateInput
};
constexpr ApplicationMode mode = ApplicationMode::GenerateInput;
// Modify this line to change the behavior of this application.
constexpr double EPSILON = 1e-8;

void Main() {
    switch (mode) {
        case ApplicationMode::FullTest:
            FullTest(EPSILON);
            break;
        case ApplicationMode::RandomTest:
            VisualRandomTest();
            break;
        case ApplicationMode::GenerateInput:
            GenerateLargeInputs();
            break;
    }
}
