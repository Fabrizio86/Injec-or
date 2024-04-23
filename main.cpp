#include "Examples/SimpleExample.h"
#include "Examples/SubDependencyExample.h"
#include "Examples/AdvancedExample.h"

int main() {
    SimpleExample::RunSimpleService();
    SubDependencies::RunSubDependency();
    AdvancedExample::RunAdvanceExample();

    return 0;
}