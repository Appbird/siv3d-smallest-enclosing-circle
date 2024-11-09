#include <chrono>
#include "SmallestEnclosingCircle.hpp"
#include "Tester.hpp"
#include "Generator.hpp"

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
            // TODO: ここの実装
            break;
        case ApplicationMode::RandomTest:
            VisualRandomTest();
            break;
        case ApplicationMode::GenerateInput:
            GenerateLargeInputs();
            break;
    }
}
