#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	

	//Create City
	CreateMap(-100, -100, 300, 300, 40, 40, 30);
	CreateLimits();

	//Borders
	CreateWall(96, -150, 10, 70);
	CreateWall(56, -150, 10, 70);
	CreateWall(-157, -115, 70, 10);
	CreateWall(-115, 165, 10, 70);
	CreateWall(157, 55, 60, 10);

	//Interior Walls
	CreateWall(5, -85, 30, 10);
	CreateWall(5, -45, 30, 10);
	CreateWall(5, 25, 30, 10);
	CreateWall(5, -15, 30, 10);
	CreateWall(75, -15, 30, 10);
	CreateWall(-65, -45, 30, 10);
	CreateWall(-65, -115, 30, 10);
	CreateWall(75, -45, 30, 10);
	CreateWall(-65, 95, 30, 10);
	CreateWall(75, 95, 30, 10);
	CreateWall(5, 125, 30, 10);

	CreateWall(96, -65, 10, 30);
	CreateWall(96, 0, 10, 30);
	CreateWall(55, 75, 10, 30);
	CreateWall(-115, 5, 10, 30);
	CreateWall(-45, 75, 10, 30);
	

	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	//Left light
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		App->player->vehicle->left_light_turned = !App->player->vehicle->left_light_turned;
		limit_time_left.Start();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		App->player->vehicle->right_light_turned = !App->player->vehicle->right_light_turned;
		limit_time_right.Start();
	}
	CarLights();
	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	buildings_phys.clear();
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.color = White;
	p.axis = true;
	
	p2List_item<Cube>* item_building = buildings.getFirst();
	p2List_item<PhysBody3D*>* item_phy_b = buildings_phys.getFirst();
	
	p2List_item<Cube>* item_car = cars.getFirst();
	p2List_item<PhysBody3D*>* item_phy_car = cars_phys.getFirst();

	p2List_item<Sphere>* item_rew = rewards.getFirst();
	p2List_item<PhysBody3D*>* item_phy_rew = rewards_phys.getFirst();


	p2List_item<Cube>* item_grou = ground.getFirst();
	p2List_item<PhysBody3D*>* item_phy_grou = ground_phys.getFirst();

	while (item_building && item_phy_b)
	{
		item_phy_b->data->GetTransform(&item_building->data.transform);
		item_building->data.color.Set(0.69, 0.69, 0.69,1);

		item_building->data.Render();
		item_phy_b = item_phy_b->next;
		item_building = item_building->next;
	}

	while (item_car && item_phy_car)
	{
		item_phy_car->data->GetTransform(&item_car->data.transform);
		item_car->data.color.Set(0.9, 0.31, 0.39, 1);

		item_car->data.Render();
		item_phy_car = item_phy_car->next;
		item_car = item_car->next;
	}

	while (item_grou && item_phy_grou)
	{
		item_phy_grou->data->GetTransform(&item_grou->data.transform);
		item_grou->data.color.Set(0.27, 0.92, 0.74, 0.05);

		item_grou->data.Render();
		item_phy_grou = item_phy_grou->next;
		item_grou = item_grou->next;
	}

	
	//while (item_rew && item_phy_rew)
	//{
	//	item_phy_rew->data->GetTransform(&item_rew->data.transform);
	//	/*item_rew->data.color.Set(0.99, 0.87, 0.54, 1);*/

	//	item_rew->data.Render();
	//	item_phy_rew = item_phy_rew->next;
	//	item_rew = item_rew->next;
	//}
	p.Render();
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("OwO!");
	if (body1 == App->player->vehicle)
	{
		p2List_item<PhysBody3D*>* item_phy_rew = rewards_phys.getFirst();
		if (item_phy_rew)
		{
			if (body2 == item_phy_rew->data)
			{
				LOG("Hey!");
			}
			item_phy_rew = item_phy_rew->next;
		}
	}
	
}

void ModuleSceneIntro::CreateLimits()
{
	Cube limit_one(400, 10, 10);
	Cube limit_two(10, 10, 370);
	Cube limit_three(10, 10, 370);
	Cube limit_four(400, 10, 10);
	Cube plane(400, 0, 400);

	ground.add(limit_one);
	ground.add(limit_two);
	ground.add(limit_three);
	ground.add(limit_four);
	ground.add(plane);

	limit_one.SetPos(0, 0, 195);
	limit_two.SetPos(-195, 0, 0);
	limit_three.SetPos(195, 0, 0);
	limit_four.SetPos(0, 0, -195);
	plane.SetPos(0, -10, 0);


	ground_phys.add(App->physics->AddBody(limit_one, 100000.00F));
	ground_phys.add(App->physics->AddBody(limit_two, 100000.00F));
	ground_phys.add(App->physics->AddBody(limit_three, 100000.00F));
	ground_phys.add(App->physics->AddBody(limit_four, 100000.00F));
	ground_phys.add(App->physics->AddBody(plane, 100000.00F));
}

void ModuleSceneIntro::CreateMap(int x, int z, int width, int height, int b_width, int b_height, int street)
{
	int building_row = (int)(width / (b_width + street));
	int building_col = (int)(height / (b_height + street));
	for (int col = 0; col < building_col; col++)
	{
		for (int row = 0; row < building_row; row++)
		{
			int b_x = row * (street + b_width);
			int b_z = col * (street + b_height);

			Cube c(b_width, 50, b_height);
			//Cube box(15, 3.75, 5);
			//Sphere r (2.0);
			Cube block(10, 1.75, 4);
			
			buildings.add(c);
			//cars.add(box);
			//rewards.add(r);
			wall.add(block);

			c.SetPos(x + b_x, 0, z + b_z);
			//box.SetPos(x + b_x, 0, z + b_z);
			//r.SetPos(20 + x + b_x, 0, 30 + z + b_z);
			block.SetPos(x + b_x, 0, 50 + z + b_z);

			buildings_phys.add(App->physics->AddBody(c, 100000.00F));
			//cars_phys.add(App->physics->AddBody(box, 100000.00F));
			//rewards_phys.add(App->physics->AddBody(r, 100.00F));
			wall_phys.add(App->physics->AddBody(block, 100.00F));
		}
	}
}

void ModuleSceneIntro::CreateWall(int x, int z, int width, int height)
{
	Cube c(width, 50, height);
	c.SetPos(x, 0, z);

	buildings.add(c);
	buildings_phys.add(App->physics->AddBody(c, 100000.00F));
	c.Render();
}

void ModuleSceneIntro::CarLights()
{
	//Left Light
	if (App->player->vehicle->left_light_turned)
	{
		if (timer_left.Read() > 500)
		{
			App->player->vehicle->left_light.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
			if (timer_left.Read() > 1000)
				timer_left.Start();
		}
		else
		{
			App->player->vehicle->left_light.color.Set(0.6f, 0.5f, 0.1f, 0.8f);
		}
	}
	else
	{
		timer_left.Start();
		App->player->vehicle->left_light.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//Right Light
	if (App->player->vehicle->right_light_turned)
	{
		if (timer_right.Read() > 500)
		{
			App->player->vehicle->right_light.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
			if (timer_right.Read() > 1000)
				timer_right.Start();
		}
		else
		{
			App->player->vehicle->right_light.color.Set(0.6f, 0.5f, 0.1f, 0.8f);
		}
	}
	else
	{
		timer_right.Start();
		App->player->vehicle->right_light.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (limit_time_left.Read() > 5000)
	{
		App->player->vehicle->left_light_turned = false;
	}
	if (limit_time_right.Read() > 5000)
	{
		App->player->vehicle->right_light_turned = false;
	}
	
}

