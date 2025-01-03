#include "Engine/Math/Vector2.hpp"
#include <print>
using namespace Engine::Math;

int main() {
    
    Vector2 a = Vector2::left();
    

    std::println("{}, {}", a.getX(), a.getY());

    return 0;
}


