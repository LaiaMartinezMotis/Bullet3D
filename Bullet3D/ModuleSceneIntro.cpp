#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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

	//for (int i = 0; i < MAX_BUILDINGS; i++) {
	//	buildings[i] = Cube((50, 70, 50);

	//}
	/*Cube c(50, 70, 50);
	c.color.Set(0.50, 0.0,0.6,1.0);
	c.SetPos(50, 0, 0);*/

	/*Cube limit_one(400,10,10);
	Cube limit_two(10, 10, 370);
	Cube limit_three(10, 10, 370);
	Cube limit_four(400, 10, 10);

	Cube block_one(50,70,20);

	block_one.SetPos(50, 0, 0);
	limit_one.SetPos(0,0,195);
	limit_two.SetPos(-195, 0, 0);
	limit_three.SetPos(195, 0, 0);
	limit_four.SetPos(0, 0, -195);
	

	
	App->physics->AddBody(block_one, 10000.0F)->Push (0.0, 0.0, 0.0);
	App->physics->AddBody(limit_one, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_two, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_three, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_four, 100000.0F)->Push(0.0, 0.0, 0.0);


	limit_one.Render();
	limit_two.Render();
	limit_three.Render();
	limit_four.Render();
	block_one.Render();*/

	CreateMap(-30, 0, 200, 200, 70, 40, 30);
	CreateLimits();

	

	return ret;
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
	p.axis = true;
	
	p2List_item<Cube>* item = buildings.getFirst();
	p2List_item<PhysBody3D*>* item_b= buildings_phys.getFirst();
	
	while (item && item_b)
	{
		item_b->data->GetTransform(&item->data.transform);
		item->data.color.Set(0,1,0,1);

		item->data.Render();
		item_b = item_b->next;
		item = item->next;
	}


	


	p.Render();
	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateLimits()
{
	Cube limit_one(400, 10, 10);
	Cube limit_two(10, 10, 370);
	Cube limit_three(10, 10, 370);
	Cube limit_four(400, 10, 10);

	limit_one.SetPos(0, 0, 195);
	limit_two.SetPos(-195, 0, 0);
	limit_three.SetPos(195, 0, 0);
	limit_four.SetPos(0, 0, -195);

	App->physics->AddBody(limit_one, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_two, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_three, 100000.0F)->Push(0.0, 0.0, 0.0);
	App->physics->AddBody(limit_four, 100000.0F)->Push(0.0, 0.0, 0.0);

	limit_one.Render();
	limit_two.Render();
	limit_three.Render();
	limit_four.Render();
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
			/*Cube cars(10,0,10);*/
			buildings.add(c);
			/*buildings.add(cars);*/
			/*c.color.Set(1, 0, 0, 1);*/
			c.SetPos(x + b_x, 0, z + b_z);
			/*cars.SetPos(x, 0, z);*/
			buildings_phys.add(App->physics->AddBody(c, 100000.00F));
			
		}
	}
}

