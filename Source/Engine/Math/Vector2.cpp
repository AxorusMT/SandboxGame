#include "Vector2.hpp"

namespace Engine::Math {
   
    Vector2::Vector2(float x, float y) {
        this->vector.x = x;
        this->vector.y = y;
    }   

    
    float Vector2::getLength() const{
        return glm::length(this->vector);
    }

    
    float Vector2::getX() const {
        return this->vector.x;
    }

    
    float Vector2::getY() const {
        return this->vector.y;
    }
 
    
    void Vector2::setX(float x) {
        this->vector.x = x;
    }

    
    void Vector2::setY(float y) {
        this->vector.y = y;
    }

    
    Vector2 Vector2::getNormalized() const {
        auto v = glm::normalize(this->vector);
        return Vector2(v.x, v.y);
    }

    
    Vector2 Vector2::zero() {
        return Vector2(
            (0.f),
            (0.f)
        );
    }

    
    Vector2 Vector2::up() {
        return Vector2(
            (0.f),
            (1.f)
        );
    }

    
    Vector2 Vector2::down() {
        return Vector2(
            (0.f),
            (-1.f)
        );
    }

    
    Vector2 Vector2::left() {
        return Vector2(
            (-1.f),
            (0.f)
        );
    }

    
    Vector2 Vector2::right() {
        return Vector2(
            (1.f),
            (0.f)
        );
    }



}

