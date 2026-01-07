#include "DemoRunner.hpp"

// Linux, C++17
// Названия файлов не менял и заданные сущности не именял за исключением DemoRunner.
// Добавил проверку результатов.
// Проверял DemoRunner локально с использованием sanitizers.

int main()
{
    plotter::DemoRunner::RunAllDemos();
}