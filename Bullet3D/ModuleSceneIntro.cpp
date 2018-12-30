#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

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


	//Play BSO
	App->audio->PlayMusic("Drift/Deja_Vu.ogg");

	//Load Fx
	lose_fx = App->audio->LoadFx("Drift/lose.wav");
	win_fx = App->audio->LoadFx("Drift/win1.wav");
	coin_fx = App->audio->LoadFx("Drift/coin.wav");
	

	//Create City
	CreateMap(-100, -100, 300, 300, 40, 40, 30);
	CreateLimits();

	CreateRightTriggers(80,-135);
	//Borders
	CreateWall(96, -150, 10, 70);
	CreateWall(56, -150, 10, 70);
	CreateWall(-157, -115, 70, 10);
	CreateWall(-115, 165, 10, 70);
	CreateWall(157, 55, 60, 10);

	//Interior Walls
	CreateWall(5, -85, 30, 10);
	CreateWall(5, -45, 30, 10);
	/*CreateWall(5, 25, 30, 10);*/
	CreateWall(5, -15, 30, 10);
	CreateWall(75, -15, 30, 10);
	CreateWall(-65, -45, 30, 10);
	CreateWall(-65, -115, 30, 10);
	CreateWall(75, -45, 30, 10);
	CreateWall(-65, 95, 30, 10);
	CreateWall(75, 95, 30, 10);
	/*CreateWall(5, 125, 30, 10);*/

	CreateWall(96, -65, 10, 30);
	CreateWall(96, 0, 10, 30);
	CreateWall(55, 75, 10, 30);
	CreateWall(-115, 5, 10, 30);
	CreateWall(-45, 75, 10, 30);

	//Create Rewards
	CreateRewards(-65, -105);
	CreateRewards(-65, -35);
	CreateRewards(-150, -65);
	CreateRewards(-105, 5);
	CreateRewards(5, 5);
	CreateRewards(-65, 105);

	CreateRewards(35, 75);
	CreateRewards(5, 75);
	CreateRewards(-25, 75);
	CreateRewards(5, 165);
	CreateRewards(75, 165);
	CreateRewards(75, 75);

	//Create Pendul
	CreatePendul(5,125, 5, 25, 25, 1);
	//Win condition
	CreateWin();

	game_timer.Start();

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
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		ResetGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->player->vehicle->SetPos(80, 0, 100);
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
	
	p2List_item<Cube>* item_building = buildings.getFirst();
	p2List_item<PhysBody3D*>* item_phy_b = buildings_phys.getFirst();
	
	p2List_item<Sphere>* item_rew = rewards.getFirst();
	p2List_item<PhysBody3D*>* item_phy_rew = rewards_phys.getFirst();


	p2List_item<Cube>* item_grou = ground.getFirst();
	p2List_item<PhysBody3D*>* item_phy_grou = ground_phys.getFirst();

	p2List_item<Cube>* item_win = win.getFirst();
	p2List_item<PhysBody3D*>* item_phy_win = win_phys.getFirst();

	p2List_item<Cube>* item_hammer = hammer_list.getFirst();
	p2List_item<PhysBody3D*>* item_phy_ham = hammer.getFirst();

	p2List_item<Cube>* item_hinge = hinge_list.getFirst();
	p2List_item<PhysBody3D*>* item_phy_hin = hinge_phys.getFirst();

	p2List_item<Cube>* item_right = right_triggers.getFirst();
	p2List_item<PhysBody3D*>* item_phy_right = right_triggers_phys.getFirst();

	p2List_item<Cube>* item_left = left_triggers.getFirst();
	p2List_item<PhysBody3D*>* item_phy_left = left_triggers_phys.getFirst();


	while (item_building && item_phy_b)
	{
		item_phy_b->data->GetTransform(&item_building->data.transform);
		item_building->data.color.Set(0.69, 0.69, 0.69,1);

		item_building->data.Render();
		item_phy_b = item_phy_b->next;
		item_building = item_building->next;
	}

	while (item_grou && item_phy_grou)
	{
		item_phy_grou->data->GetTransform(&item_grou->data.transform);
		item_grou->data.color.Set(0.39, 0.46, 0.52, 1);

		item_grou->data.Render();
		item_phy_grou = item_phy_grou->next;
		item_grou = item_grou->next;
	}

	while (item_rew && item_phy_rew)
	{
		item_phy_rew->data->GetTransform(&item_rew->data.transform);
		item_rew->data.color.Set(0.57, 0.43, 0.85, 1);

		item_rew->data.Render();
		item_phy_rew = item_phy_rew->next;
		item_rew = item_rew->next;
	}

	while (item_win && item_phy_win)
	{
		item_phy_win->data->GetTransform(&item_win->data.transform);


		item_win->data.color.Set(0.62, 0.50, 0.37, 1);

		item_win->data.Render();
		item_phy_win = item_phy_win->next;
		item_win = item_win->next;
	}

	while (item_hinge && item_phy_hin)
	{
		item_phy_hin->data->GetTransform(&item_hinge->data.transform);


		item_hinge->data.color.Set(0.61, 0.83, 0.67, 1);

		item_hinge->data.Render();
		item_phy_hin = item_phy_hin->next;
		item_hinge = item_hinge->next;
	}

	while (item_hammer && item_phy_ham)
	{
		item_phy_ham->data->GetTransform(&item_hammer->data.transform);


		item_hammer->data.color.Set(0.61, 0.83, 0.67, 1);

		item_hammer->data.Render();
		item_phy_ham = item_phy_ham->next;
		item_hammer = item_hammer->next;
	}
	while (item_right && item_phy_right)
	{
		item_phy_right->data->GetTransform(&item_right->data.transform);


		item_right->data.color.Set(0.99, 0.72, 0.38, 1);

		item_right->data.Render();
		item_phy_right = item_phy_right->next;
		item_right = item_right->next;
	}

	while (item_left && item_phy_left)
	{
		item_phy_left->data->GetTransform(&item_left->data.transform);


		item_left->data.color.Set(0.61, 0.83, 0.67, 1);

		item_left->data.Render();
		item_phy_left = item_phy_left->next;
		item_left = item_left->next;
	}

	finish_time = 95000 - App->scene_intro->game_timer.Read();
	minutes = finish_time / 60000;

	if (finish_time < 0)
	{
		App->audio->PlayFx(lose_fx);
		ResetGame();
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == App->player->vehicle)
	{
		LOG("owo");
		p2List_item<PhysBody3D*>* item_phy_rew = rewards_phys.getFirst();
		while (item_phy_rew)
		{
			if (body2 == item_phy_rew->data)
			{
				App->audio->PlayFx(coin_fx);
				App->player->vehicle->score += 100;
			}
			item_phy_rew = item_phy_rew->next;
		}

		p2List_item<PhysBody3D*>* item_win = win_phys.getFirst();

		if (body2 == item_win->data)
		{
			App->audio->PlayFx(win_fx);
			ResetGame();
		}

		p2List_item<PhysBody3D*>* item_phy_b = buildings_phys.getFirst();
		while (item_phy_b)
		{
			if (body2 == item_phy_b->data)
			{
				App->player->vehicle->score -= 100;
			}
			item_phy_b = item_phy_b->next;
		}

		/*p2List_item<PhysBody3D*>* item_right = right_triggers_phys.getFirst();
		while (item_right)
		{
			if (body2 == item_right->data && App->player->vehicle->right_light_turned)
			{

				App->player->vehicle->score += 1000;
			}
			else
			{
				App->player->vehicle->score -= 1000;
			};
			item_right = item_right->next;
		}
		p2List_item<PhysBody3D*>* item_left = left_triggers_phys.getFirst();
		while (item_left)
		{
			if (body2 == item_left->data && App->player->vehicle->left_light_turned)
			{
				App->player->vehicle->score += 1000;
			}
			else {
				App->player->vehicle->score -= 1000;
			};
			item_left = item_left->next;
		}*/
	}
}
	


void ModuleSceneIntro::CreateLeftTriggers(int x, int z)
{
	Cube left(30, 1, 1);
	left.SetPos(x,0,z);
	left_triggers.add(left);

	left_triggers_phys.add(App->physics->AddBody(left, 100000.00F));

	left.Render();
}

void ModuleSceneIntro::CreateRightTriggers(int x, int z)
{
	Cube right(30, 1.2, 1);
	right.SetPos(x, 0, z);
	right_triggers.add(right);

	right_triggers_phys.add(App->physics->AddBody(right, 100000.00F));

	right.Render();
}

void ModuleSceneIntro::CreatePendul(int x, int z, int r, int s, int width, int height)
{
	Cube axis_door_one(40,2,2);
	Cube door_one(width, 50, height);

	Cube axis_door_two(40, 2, 2);
	Cube door_two(width, 50, height);


	axis_door_one.SetPos(x,50,z);
	door_one.SetPos(x,0,z);

	axis_door_two.SetPos(r, 50, s);
	door_two.SetPos(r, 0, s);

	
	hammer_list.add(door_one);
	hinge_list.add(axis_door_one);
	hammer_list.add(door_two);
	hinge_list.add(axis_door_two);

	hinge_phys.add(App->physics->AddBody(axis_door_one, 1000.0F));
	hammer.add(App->physics->AddBody(door_one,10.0f));

	hinge_phys.add(App->physics->AddBody(axis_door_two, 1000.0F));
	hammer.add(App->physics->AddBody(door_two, 10.0f));

	for (int i = 0; i <= hammer.count(); i++) {
		PhysBody3D* bodyA;
		PhysBody3D* bodyB;
		hinge_phys.at(i, bodyA);
		hammer.at(i, bodyB);
		
		App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, -22, 0), (0, 0, 0), (1, -22, 1), (1, -22, 1), true);
	
	
	}

	axis_door_one.Render();
	door_one.Render();
	axis_door_two.Render();
	door_two.Render();
}

void ModuleSceneIntro::CreateWin()
{
	Cube end_left(5, 30, 5);
	Cube end_right(5, 30, 5);
	Cube end_up(5, 5, 32);
	Cube end_wall(5, 30, 32);

	win.add(end_wall);
	win.add(end_left);
	win.add(end_right);
	win.add(end_up);

	end_wall.SetPos(-95, 0, 163);
	end_left.SetPos(-90, 0, 178);
	end_right.SetPos(-90, 0, 148);
	end_up.SetPos(-90, 30, 163);

	win_phys.add(App->physics->AddBody(end_wall, 100000.0F));
	win_phys.add(App->physics->AddBody(end_left, 10000.0F));
	win_phys.add(App->physics->AddBody(end_right, 10000.0F));
	win_phys.add(App->physics->AddBody(end_up, 10000.0F));

	end_wall.Render();
	end_left.Render();
	end_right.Render();
	end_up.Render();

}


void ModuleSceneIntro::CreateLimits()
{
	Cube limit_one(400, 10, 10);
	Cube limit_two(10, 10, 390);
	Cube limit_three(10, 10, 390);
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

			buildings.add(c);
			//cars.add(box);
		
			c.SetPos(x + b_x, 0, z + b_z);
			//box.SetPos(x + b_x, 0, z + b_z);
	
			buildings_phys.add(App->physics->AddBody(c, 100000.00F));
			//cars_phys.add(App->physics->AddBody(box, 100000.00F));
			
		}
	}
}
void ModuleSceneIntro::CreateRewards(int x, int z)
{
	Sphere r(2.0);
	r.SetPos(x, 0, z);


	rewards.add(r);
	rewards_phys.add(App->physics->AddBody(r, 100.0F));
	r.Render();
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

void ModuleSceneIntro::ResetGame()
{
	App->player->vehicle->score = 0;
	game_timer.Start();
	App->player->vehicle->SetPos(70, 0, -150);
	App->player->vehicle->Orientation(0);
	App->player->vehicle->SetLinearVelocity(0, 0, 0);
	App->audio->PlayMusic("Drift/Deja_Vu.ogg");

}

