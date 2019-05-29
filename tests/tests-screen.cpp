#include "catch.hpp"
#include <Screen.h>
#include <chrono>


using namespace std::chrono;

TEST_CASE("Screen clear speed test", "[screen]"){
    const int numberOfClears = 10000;
    Screen screen0(64,64,0);
    auto msStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    for(int i = 0; i < numberOfClears; i++){
        screen0.fill(Color::red());
        screen0.clear();
    }
    auto msTotal = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - msStart;
    WARN("Total Time for " << numberOfClears << " fill&clear events: " << msTotal.count() << " ms");
    CHECK(msTotal.count() < 500);
}