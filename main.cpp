#include "Examples/SimpleExample.h"
#include "Examples/SubDependencyExample.h"
#include "Examples/AdvancedExample.h"
#include "Examples/WebExample.h"
#include "Examples/AdvancedWebExample.h"

int main() {
    SimpleExample::RunSimpleService();
    SubDependencies::RunSubDependency();
    AdvancedExample::RunAdvanceExample();
    WebServerExample::RunWebServerExample();
    AdvancedWebExample::RunWebServerExample();

    return 0;
}