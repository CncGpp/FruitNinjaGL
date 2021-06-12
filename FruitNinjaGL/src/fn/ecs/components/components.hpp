#pragma once
#include <utl/print.hpp>
#include <fn\ecs\ecs.hpp>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include "glm/gtx/string_cast.hpp"
#include <fn\core\gl\Model.hpp>
#include <fn\core\assets\Fruits.hpp>
#include <fn\core\Camera.hpp>
#include <fn\core\gl\Sprite.hpp>

namespace C {
    struct Position : public fn::Component<Position> {
        glm::vec3 position;       // Vettore posizione dell'entità
        glm::vec3 rotation;       // Rotazione (angoli di eulero) dell'entità rispetto agli assi canonici.

        [[nodiscard]] inline glm::vec3 position3() const { return this->position; }
        [[nodiscard]] inline glm::vec3 rotation3() const { return this->rotation; }
        [[nodiscard]] inline glm::vec2 position2() const { return { this->position.x, this->position.y }; }
        [[nodiscard]] inline glm::vec2 rotation2() const { return { this->rotation.x, this->rotation.y }; }

        [[nodiscard]] glm::mat4 model_matrix() const;

        void traslate(const glm::vec2& vect) { this->position += glm::vec3(vect, 0.0f); }
        void traslate(const glm::vec3& vect) { this->position += vect; }
        void rotate(const glm::vec2& vect) { this->rotation += glm::vec3(vect, 0.0f); }
        void rotate(const glm::vec3& vect) { this->rotation += vect; }
    };


    



    struct Movement : public fn::Component<Movement> {
        glm::vec3 velocity;       // Vettore posizione dell'entità
        glm::vec3 spin;           // Rotazione (angoli di eulero) dell'entità rispetto agli assi canonici.

        [[nodiscard]] inline glm::vec3 velocity3() const { return this->velocity; }
        [[nodiscard]] inline glm::vec3 spin3() const { return this->spin; }
        [[nodiscard]] inline glm::vec2 velocity2() const { return { this->velocity.x, this->velocity.y }; }
        [[nodiscard]] inline glm::vec2 spin2() const { return { this->spin.x, this->spin.y }; }

        void accelerate(const glm::vec2& vect) { this->velocity += glm::vec3(vect, 0.0f); }
        void accelerate(const glm::vec3& vect) { this->velocity += vect; }
        void setSpin(const glm::vec3& vect) { this->spin = vect; }
        void setSpin(const glm::vec2& vect) { this->spin = glm::vec3(vect, 0.0f); }
    };




    struct Render : public fn::Component<Render> {
        ShaderSP shader;
        ModelSP model;
    };


    struct Fruit : public fn::Component<Fruit> {
        Fruits fruit;
        Fruits::Model model_kind;
    };


    struct Sprite : public fn::Component<Sprite> {
        SpriteSP sprite;
    };


    struct Particle : public fn::Component<Particle> {
        core::seconds lifetime;
        core::seconds elapsed;
        std::function<float(float)> interpolator;
    };

    struct AABB : public fn::Component<AABB> {
        glm::mat2x3 box;
        float radius() const {
            auto diff = glm::abs(box[0] - box[1]);
            return *std::max_element(&diff[0], &diff[2]);
        }
    };

}