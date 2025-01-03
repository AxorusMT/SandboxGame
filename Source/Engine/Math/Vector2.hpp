#include "Engine/Base/Base.hpp"
#include <concepts>
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <compare>

namespace Engine::Concepts {
    template <typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;
}

namespace Engine::Math {
    class Vector2 {
        public:
            Vector2(float x, float y);

            /**
             * @brief Returns the up vector (assuming +y is up)
             * 
             * Note that most graphics APIs flip the y axis, meaning -y is up instead. For graphics, use the
             * down function instead.
             * 
             *
             * @return The up vector, i.e. (0, 1)
             */
            static Vector2 up();

            /**
             * @brief Returns the down vector (assuming -y is down)
             * 
             * Note that most graphics APIs flip the y axis, meaning +y is down instead. For graphics, use the
             * up function instead.
             * 
             * @return The down vector, i.e (0, -1)
             */
            static Vector2 down();

            /**
             * @brief Returns the left vector (assuming -x is left)
             * 
             * @return The left vector, i.e. (-1, 0)
             */
            static Vector2 left();

            /**
             * @brief Returns the right vector (assuming +x is right)
             * 
             * @return The right vector, i.e. (1, 0)
             */
            static Vector2 right();

            /**
             * @brief Returns the zero vector
             * 
             * @return The zero vector, i.e. (0, 0)
             */
            static Vector2 zero();

            //static float dot(const Vector2& x, const Vector2& y);

            /**
             * @brief Return the length of the vector (i.e. length of the hypotenuse of the triangle formed by x and y)
             * 
             * @return The length of the vector
             */
            float getLength() const;

            /**
             * @brief Return the X (horizontal) component of the vector
             * 
             * @return The X component
             */
            float getX() const;

            /**
             * @brief Return the Y (vertical) component of the vector
             * 
             * @return The Y component
             */
            float getY() const;

            /**
             * @brief Sets the x component of the vector
             * 
             * @param x The X component 
             */
            void setX(float x); 

            /**
             * @brief Sets the y component of the vector
             * 
             * @param y The Y component
             */
            void setY(float y);

            /**
             * @brief Get the normalized vector
             * 
             * @return A normalized (unit length) vector
             */
            Vector2 getNormalized() const;


        private:
            glm::vec2 vector;
    };
}

