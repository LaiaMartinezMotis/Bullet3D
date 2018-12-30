#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateLimits();


	void CreateLeftTriggers(int x, int z);
	void CreateRightTriggers(int x, int z);
	void CreatePendul(int x, int z, int r, int s, int with, int heigh);
	void CreateWin();
	void CreateRewards(int x, int z);
	void CreateMap(int x, int z, int width, int height, int row, int column, int street);
	void CreateWall(int x, int z, int width, int height);
	void CarLights();
	void ResetGame();

public:

	p2List<Cube> buildings;
	p2List<PhysBody3D*> buildings_phys;

	p2List<Cube> left_triggers;
	p2List<PhysBody3D*> left_triggers_phys;

	p2List<Cube> right_triggers;
	p2List<PhysBody3D*> right_triggers_phys;

	p2List<Sphere> rewards;
	p2List<PhysBody3D*> rewards_phys;

	p2List<Cube> wall;
	p2List<PhysBody3D*> wall_phys;


	p2List<Cube> hammer_list;
	p2List<PhysBody3D*> hammer;
	p2List<Cube> hinge_list;
	p2List<PhysBody3D*> hinge_phys;

	p2List<Cube> win;
	p2List<PhysBody3D*> win_phys;

	PhysBody3D*	win_ground;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	p2List<Cube> ground;
	p2List<PhysBody3D*> ground_phys;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Timer timer_left;
	Timer timer_right;
	Timer limit_time_right;
	Timer limit_time_left;

	uint reward;
	uint lose_fx;
	uint win_fx;
	uint coin_fx;

	Timer game_timer;
	int minutes;
	int finish_time = 0;

};
