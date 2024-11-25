#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <memory>
#include <format>
#include <format>

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleGame::CreateRandomCircles()
{
	int randomNumber = 1 + (std::rand() % (int)(100 - 1 + 1));
	int randomNumberX = 1 + (std::rand() % (int)(SCREEN_WIDTH - 1 + 1));
	int randomNumberY = 1 + (std::rand() % (int)(SCREEN_HEIGHT - 1 + 1));
	int randomMass = 1 + (std::rand() % (int)(100 - 1 + 1));

	if (timer.ReadSec() >= 1.0)
	{
		if (randomNumber % 2 == 0)
		{
			std::shared_ptr<PhysBody> circle = std::shared_ptr<PhysBody>(App->physics->CreateCircle(0, randomNumberY, randomNumber, randomMass));
			circles.emplace_back(circle);
			timer.Start();
		}

	}

	
	float normalForce = 2;
	
	float frictionForceX = normalForce * mu;

	for (std::shared_ptr<PhysBody> circle : circles)
	{
		float forceX = 10;
		float forceY = 0;

		if (circle->body->GetLinearVelocity().LengthSquared() > 0.001f)
		{
			float mass = circle->body->GetMass();
			float N = GRAVITY_Z * mass;
			float dynamicFriction = N * mu;
			forceX = std::max(0.0f, forceX - dynamicFriction);
			//friction application
			circle->body->ApplyForce(b2Vec2(forceX, forceY), b2Vec2_zero, true);

		}
		else
		{
			float mass = circle->body->GetMass();
			float N = GRAVITY_Z * mass;
			float dynamicFriction = N * dynamicMu;
			forceX = std::max(0.0f, forceX - dynamicFriction);
			//friction application
			circle->body->ApplyForce(b2Vec2(forceX, forceY), b2Vec2_zero, true);
		}

	}
	
	if (IsMouseButtonPressed(0))
	{
		mu += 0.1;
		if (mu > 1)
		{
			mu = 0;
		}
	}
	if (IsMouseButtonPressed(1))
	{
		dynamicMu += 0.1;
		if (dynamicMu > 1)
		{
			dynamicMu = 0;
		}
	}

	DrawText(std::format("{}/ {}", mu, dynamicMu).c_str(), 10, 600, 30, BLACK);
}

// Update: draw background
update_status ModuleGame::Update()
{
	CreateRandomCircles();
	
	return UPDATE_CONTINUE;

}
