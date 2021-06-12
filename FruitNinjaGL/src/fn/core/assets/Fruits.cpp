#include "Fruits.hpp"
#include <fn\core\assets\assets.hpp>

// Pesi https://weightofstuff.com/average-weight-of-all-fruits-and-vegetables/#

const Fruits Fruits::apple_red = {
	.name = "mela rossa",
	.species = Fruits::Species::apple,
	.modeldir = assets::dir::models / "fruit/Meshes/Apple_R",
	.whole_filename = "Apple_R.fbx",
	.half_back_filename = "Apple_R_Half_Back.fbx",
	.half_front_filename = "Apple_R_Half_Front.fbx",
	.weight = 0.195f
};

const Fruits Fruits::banana = {
	.name = "banana",
	.species = Fruits::Species::banana,
	.modeldir = assets::dir::models / "fruit/Meshes/Banana",
	.whole_filename = "Banana.fbx",
	.half_back_filename = "Banana_Half_Bottom.fbx",
	.half_front_filename = "Banana_Half_Top.fbx",
	.weight = 0.120f
};

const Fruits Fruits::coconut = {
	.name = "cocco",
	.species = Fruits::Species::coconut,
	.modeldir = assets::dir::models / "fruit/Meshes/Coconut",
	.whole_filename = "Coconut.fbx",
	.half_back_filename = "Coconut_Half_Bottom.fbx",
	.half_front_filename = "Coconut_Half_Top.fbx",
	.weight = 0.685f
};

const Fruits Fruits::honeydew_melon = {
	.name = "honeydew melon",
	.species = Fruits::Species::honeydew_melon,
	.modeldir = assets::dir::models / "fruit/Meshes/HoneydewMelon",
	.whole_filename = "HoneydewMelon.fbx",
	.half_back_filename = "HoneydewMelon_Half_Bottom.fbx",
	.half_front_filename = "HoneydewMelon_Half_Top.fbx",
	.weight = 1.8f
};

const Fruits Fruits::kiwi = {
	.name = "kiwi",
	.species = Fruits::Species::kiwi,
	.modeldir = assets::dir::models / "fruit/Meshes/Kiwi",
	.whole_filename = "Kiwi.fbx",
	.half_back_filename = "Kiwi_Half_Bottom.fbx",
	.half_front_filename = "Kiwi_Half_Top.fbx",
	.weight = 0.075f
};

const Fruits Fruits::lemon = {
	.name = "lemon",
	.species = Fruits::Species::lemon,
	.modeldir = assets::dir::models / "fruit/Meshes/Lemon",
	.whole_filename = "Lemon.fbx",
	.half_back_filename = "Lemon_Half_Bottom.fbx",
	.half_front_filename = "Lemon_Half_Top.fbx",
	.weight = 0.100f
};

const Fruits Fruits::lime = {
	.name = "lime",
	.species = Fruits::Species::lime,
	.modeldir = assets::dir::models / "fruit/Meshes/Lime",
	.whole_filename = "Lime.fbx",
	.half_back_filename = "Lime_Half_Bottom.fbx",
	.half_front_filename = "Lime_Half_Top.fbx",
	.weight = 0.050f
};

const Fruits Fruits::orange = {
	.name = "orange",
	.species = Fruits::Species::orange,
	.modeldir = assets::dir::models / "fruit/Meshes/Orange",
	.whole_filename = "Orange.fbx",
	.half_back_filename = "Orange_Half_Bottom.fbx",
	.half_front_filename = "Orange_Half_Top.fbx",
	.weight = 0.130f
};

const Fruits Fruits::pear = {
	.name = "pear",
	.species = Fruits::Species::pear,
	.modeldir = assets::dir::models / "fruit/Meshes/Pear",
	.whole_filename = "Pear.fbx",
	.half_back_filename = "Pear_Half_Back.fbx",
	.half_front_filename = "Pear_Half_Front.fbx",
	.weight = 0.180f
};

const Fruits Fruits::pineapple = {
	.name = "pineapple",
	.species = Fruits::Species::pineapple,
	.modeldir = assets::dir::models / "fruit/Meshes/Pineapple",
	.whole_filename = "Pineapple.fbx",
	.half_back_filename = "Pineapple_Half_Bottom.fbx",
	.half_front_filename = "Pineapple_Half_Top.fbx",
	.weight = 1.590f
};

const Fruits Fruits::plum = {
	.name = "plum",
	.species = Fruits::Species::plum,
	.modeldir = assets::dir::models / "fruit/Meshes/Plum",
	.whole_filename = "Plum.fbx",
	.half_back_filename = "Plum_Half_Back.fbx",
	.half_front_filename = "Plum_Half_Front.fbx",
	.weight = 0.065f
};

const Fruits Fruits::pomegranate = {
	.name = "pomegranate",
	.species = Fruits::Species::pomegranate,
	.modeldir = assets::dir::models / "fruit/Meshes/Pomegranate",
	.whole_filename = "Pomegranate.fbx",
	.half_back_filename = "Pomegranate_Half_Back.fbx",
	.half_front_filename = "Pomegranate_Half_Front.fbx",
	.weight = 0.255f
};

const Fruits Fruits::strawberry = {
	.name = "strawberry",
	.species = Fruits::Species::strawberry,
	.modeldir = assets::dir::models / "fruit/Meshes/Strawberry",
	.whole_filename = "Strawberry.fbx",
	.half_back_filename = "Strawberry_Half_Back.fbx",
	.half_front_filename = "Strawberry_Half_Front.fbx",
	.weight = 0.012f
};

const Fruits Fruits::watermelon = {
	.name = "watermelon",
	.species = Fruits::Species::watermelon,
	.modeldir = assets::dir::models / "fruit/Meshes/Watermelon",
	.whole_filename = "Watermelon.fbx",
	.half_back_filename = "Watermelon_Half_Bottom.fbx",
	.half_front_filename = "Watermelon_Half_Top.fbx",
	.weight = 9.0f
};

const Fruits Fruits::tomato = {
	.name = "tomato",
	.species = Fruits::Species::tomato,
	.modeldir = assets::dir::models / "fruit/Meshes/Tomato",
	.whole_filename = "Tomato.fbx",
	.half_back_filename = "Tomato_Half_Bottom.fbx",
	.half_front_filename = "Tomato_Half_Top.fbx",
	.weight = 0.170f
};

const Fruits Fruits::pepper_green = {
	.name = "pepper green",
	.species = Fruits::Species::pepper,
	.modeldir = assets::dir::models / "fruit/Meshes/Pepper_Green",
	.whole_filename = "Pepper_G.fbx",
	.half_back_filename = "Pepper_G_Half_Bottom.fbx",
	.half_front_filename = "Pepper_G_Half_Top.fbx",
	.weight = 0.170f
};

const Fruits Fruits::pepper_pointed = {
	.name = "pepper pointed",
	.species = Fruits::Species::pepper,
	.modeldir = assets::dir::models / "fruit/Meshes/Pepper_Pointed",
	.whole_filename = "Pepper_Pointed.fbx",
	.half_back_filename = "Pepper_Pointed_Half_Bottom.fbx",
	.half_front_filename = "Pepper_Pointed_Half_Top.fbx",
	.weight = 0.180f
};

const Fruits Fruits::pepper_red = {
	.name = "pepper red",
	.species = Fruits::Species::pepper,
	.modeldir = assets::dir::models / "fruit/Meshes/Pepper_Red",
	.whole_filename = "Pepper_R.fbx",
	.half_back_filename = "Pepper_R_Half_Bottom.fbx",
	.half_front_filename = "Pepper_R_Half_Top.fbx",
	.weight = 0.170f
};

const Fruits Fruits::pepper_yellow = {
	.name = "pepper yellow",
	.species = Fruits::Species::pepper,
	.modeldir = assets::dir::models / "fruit/Meshes/Pepper_Yellow",
	.whole_filename = "Pepper_Y.fbx",
	.half_back_filename = "Pepper_Y_Half_Bottom.fbx",
	.half_front_filename = "Pepper_Y_Half_Top.fbx",
	.weight = 0.170f
};