#include "Fruit.hpp"
#include <fn\ecs\components\components.hpp>
#include <fn\core\assets\AssetManager.hpp>
#include <utl/random.hpp>

namespace E {
    Fruit Fruit::make(fn::Database& db, const Fruits& fruit, const Fruits::Model model_kind)
    {
        Fruit f = std::move(db.create_entity());
        f.set<C::Position>({});
        f.set<C::Movement>({});
        f.set<C::Fruit>({
            .fruit = fruit, 
            .model_kind = model_kind
        });
        f.set<C::Render>({
            .shader = AssetManager::loadShader(S_DEFAULT_VERTEX_SHADER, S_DEFAULT_FRAGMENT_SHADER),
            .model = AssetManager::loadModel(fruit.modelpath(model_kind)),
        });
        f.set<C::AABB>({ .box = f.get<C::Render>()->model->AABB() });
        return std::move(f);
    }

    void Fruit::split(fn::Database& db, const fn::Eid eid)
    {
        assert( (db.has<C::Position, C::Movement, C::Fruit>(eid) == true) );
        auto [p, m, f] = db.get<C::Position, C::Movement, C::Fruit>(eid);

        Fruit half_front = Fruit::make(db, f->fruit, Fruits::Model::half_front);
        Fruit half_back = Fruit::make(db, f->fruit, Fruits::Model::half_back);

        constexpr auto alpha = 0.1f;
        const auto beta = fn::rand::uniform(4.0f, 8.0f);
        const auto gamma = fn::rand::uniform(1.0f, 5.0f);

        glm::vec3 hfPosition = p->position + alpha * glm::vec3(-1.0f, 0.0f, -1.0f);
        glm::vec3 hbPosition = p->position + alpha * glm::vec3(+1.0f, 0.0f, +1.0f);
        glm::vec3 hfVelocity = m->velocity - beta * fn::rand::uniform<3>();
        hfVelocity.y = m->velocity.y + beta * fn::rand::uniform(-0.1f, 0.1f);
        glm::vec3 hbVelocity = m->velocity + beta * fn::rand::uniform<3>();
        hbVelocity.y = m->velocity.y + beta * fn::rand::uniform(-0.1f, 0.1f);
        glm::vec3 hfSpin = m->spin - gamma * fn::rand::uniform<3>();
        glm::vec3 hbSpin = m->spin + gamma * fn::rand::uniform<3>();

        if (fn::rand::uniform(0.0f, 1.0f) < 0.5) {
            std::swap(hfVelocity, hbVelocity);
        }
        if (fn::rand::uniform(0.0f, 1.0f) < 0.5) {
            std::swap(hfSpin, hbSpin);
        }

        half_front.set<C::Position>({ .position = hfPosition, .rotation = p->rotation })\
                  .set<C::Movement>({ .velocity = hfVelocity, .spin = hfSpin })\
                  .set<C::AABB>({ .box = half_front.get<C::Render>()->model->AABB() });

        half_back.set<C::Position>({ .position = hbPosition, .rotation = p->rotation })\
                 .set<C::Movement>({ .velocity = hbVelocity, .spin = hbSpin })\
                 .set<C::AABB>({ .box = half_back.get<C::Render>()->model->AABB() });

        db.destroy_entity(eid);
    }

}

