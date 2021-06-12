// FruitNinjaGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "src/fn/ecs/ecs.hpp"

struct Position : public fn::Component<Position>{
    Position() = default;
    Position(int x, int y) : x(x), y(y) {}
    int x, y;

    void print() {
        std::cout << "X: " << x << "jfoisdjfoj " << y << "\n";
    }
};

struct Rotation {
    float theta;
};

int main()
{
    fn::Database db;
    fn::Entity e = db.create_entity();
    fn::Entity e2 = db.create_entity();
    fn::Entity e3 = db.create_entity();


    std::cout << "Has: " << e.has<Position>() << "\n";
    
    e.set<Rotation>({.theta=0.23478023f});
    e.set<Position>({1, 3});
    e2.set<Position>({ 21, 4 });
    e3.set<Position>({ 231, 44 });
    std::cout << "Has: " << e.has<Position>() << "\n";

    e.remove<Position>();
    std::cout << "Has: " << e.has<Position>() << "\n";

    auto* p = e.get<Rotation>();
    std::cout << "struct Rotation: " << p->theta << "\n";
    std::cout << "Signature: " << e.signature() << "\n";

    auto rs = db.having(Position::cid);
    std::cout << "Result set size: " << rs.size() << "\n";
    rs = db.having<Position>();
    std::cout << "Result set size: " << rs.size() << "\n";


    db.for_each(fn::Sign<Position>, [](fn::Eid e) {std::cout << "Eid:" << e << "\n"; });
    db.for_each(fn::Sign<Rotation>, [](fn::Entity& e) {std::cout << "Rotation:" << e.get<Rotation>()->theta << "\n"; });

    std::cout << "\nSignature: " << fn::Sign<Position, Rotation> << "\n";

    e2.set<Rotation>({ .theta = 420.69f });
    std::cout << "Tupla" << std::get<1>(e2.get<Position, Rotation>())->theta << "\n";


    auto [dio, cane] = e2.get<Position, Rotation>();
    dio->print();

    std::cout << "Has: " << e.has<Position, Rotation>() << "\n";
    std::cout << "Has: " << e2.has<Position, Rotation>() << "\n";

    db.for_each<Position>([](Position& p) {std::cout << "Posizione.y: " << p.y << "\n"; });
    db.for_each<Position, Rotation>([](Position& p, Rotation& r) {std::cout << "Posizione.y: " << p.y << "\n"; });
}
