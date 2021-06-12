<p align="center">
  <img src="./relazione/images/ch10/6.png" height="200"/>
</p>

# FruitNinjaGL
A data oriented Entity Component System based clone of the famous Fruit Ninja game written in c++ and opengl.

    NOTE: This is a Computer graphics university project. 
    
For a more detailed description, see the [Report](./relazione/relazioneCG.pdf) (Italian)

## The game
Fruit Ninja GL è un clone di Fruit Ninja sviluppato in OpenGL dalla" Space Mambo Studios" un’immaginaria software house. L’idea di base è quella di replicare la modalità Zen del gioco integrandola però di alcune caratteristiche mancanti ma necessarie ai requisiti del progetto senza però snaturare o alterare il gameplay originale.

<p align="center">
  <img src="./relazione/images/ch10/a2.png" height="200"/>
  <img src="./relazione/images/ch10/a1.jpg" height="200"/> <br>
  <img src="./relazione/images/ch10/a3.jpg" height="200"/>
  <img src="./relazione/images/ch10/a4.jpg" height="200"/>
</p>

The player will have 1:30 minutes to slice as many points as possible by making combos and blitzes to maximize the final score. To complete the whole there are the original music and effects of the game trying to replicate the graphics in detail.


## Overview
Fruit Ninja GL is based on OpenGL 4.6 Core Profile using the GLFW 3.3.2 library for context creation, window, input and event handling. The application was developed in C++ 20 in the Visual Studio 2019 environment.

###  Similar to the original
During development we tried to remain as faithful as possible to the game in its original version released in 2010. Music, sounds, fonts and backgrounds are freely available on the net while the sprites and elements of the GUI have been reproduced using photoshop. (All created and used `.psd` files are included with the assets.).

The 3D models of the fruits both in their _whole_ and _sliced_ versions are taken from one of the many Fruit packs available on the net.

###  Entity Component System
The development of the game logic was based on the Entity Component System, a very common architectural pattern in game development.

###  Abstraction of primitives
All the main graphic primitives (of OpenGL and not) such as _texture_, _mesh_, _model_, _shader_ or _audio_ have been abstracted into specific classes to make their use more simple, immediate and convenient.

###  VAOs, VBOs, Vertex and Shaders
The rendering of the various objects is carried out using the various buffers (Vertex Array Object, Vertex Buffer Array, Index Buffer Array) and Shaders (Vertex and Fragment) of OpenGL. Different shaders have been implemented depending on the graphic element or how you want to draw it in the framebuffer.

###  State Engine
The game engine is based on the Game State 'Stack', another very popular architectural pattern for managing the various possible game states (Loading, playng, pause, game over, ...).

##  Descrizione Architetturale
Architecturally FruitNinjaGL is structured in five _macro-modules_ each of which is responsible for managing a part of the application.
- `core/` classes aimed at managing low-level OpenGL and non-OpenGL details.
- `ecs/` implementation of ECS with components, systems and related entity management classes.
- `logic/` details related to gameplay and game elements.
- `utl/` utility or debug classes and functions.



# Entity component system
The **Database** (Also called world, universe or enetity manager), as it is easy to guess from the name, has the task of create and manage the entities providing them with unique identifiers, keeping track of the components assigned to them and finally performing queries and searches for the systems. All while trying to maintain a data-oriented approach.

For example, let's create three entities `e1`, `e2` and `e3` starting from an instance of a `fn::Database db` in which the various components are already registered.

```c++
 fn::Entity e1 = db.create();
 fn::Entity e2 = db.create();
 fn::Entity e3 = db.create();
```

The three entities are now empty and have no associated components.

### **Set components**
A component can be added to a `E::Entity` in two ways via the `database` and its `Eid` or via its methods.

```c++
 // set attraverso E::Entity
 e1.set<C::Position>({});   //Costruttore di default
 e1.set<C::Movement>({		//Init list
 	.velocity=glm::vec3(1.5f, 3.6f, 9.1f),
 	.spin=glm::vec3(3.14f, -1.21f, -0.1f),
 });
 e1.set<C::Fruit>({ ... });

 e2.set<C::Position>({}); //Costruttore di default

 // set attraverso fn::Database
 db.set<C::Position>(e3.Eid, {
    .velocity=glm::vec3(1.5f, 3.6f, 9.1f),
    .spin=glm::vec3(3.14f, -1.21f, -0.1f),
 });
```

### **Get components**
In the same way as for `set` it is also possible to access components using the  `get` function. The returned value is always a pointer to the component. In addition it is also possible to access multiple components together, the value returned in this case is a  `std::tuple` of pointers.

```c++
 // get di una singola componente
 auto* p = e1.get<C::Position>();
 
 // get di più componenti classico
 auto t = e1.get<C::Position, C::Movement, C::Fruit>();
 std::get<1>(t)->accelerate(...); //--> C::Movement
 
 // get di più componenti con un-pack (c++17)
 auto [p, m, f] = e1.get<C::Position, C::Movement, C::Fruit>();
 m->accelerate(...); //--> C::Movement
```

### **Component check**
In the same way as for `get` you can check if an entity has one or more components using the `has` method.
```c++
bool a = e1.has<C::Movement, C::Fruit>(); //true 
bool a = e2.has<C::Sprite>(); //false  
```


## Query
Database queries can only be performed on a database object and in two ways:
- Using the  `Database::having<>` method which simply returns a vector, this (method is actually never used in practice.) of eid's that match the query.
- Using the  `Database::for_each<>` method in combination with a lambda function that is applied to every entity that matches the query.


```c++
 // (a) lambda sugli eid
 db.for_each<C::Movement, C::Fruit>([](fn::Eid e){
	// fai qualcosa con e	
 }); 

 // (b) lambda sugli E::Entity
 db.for_each<C::Fruit>([](E::Entity& e){
	// fai qualcosa con e
 }); 
 
 // (c) lambda sui componenti
 db.for_each<C::Position, C::Movement>([](C::Position& p, 
                                          C::Movement& m){
	// fai qualcosa con p ed m
 }); 

 // (c) lambda sui componenti + E::Entity
 db.for_each<C::Sprite>([](E::Entity& e, C::Sprite& s){
	// fai qualcosa con e ed s
 }); 
```