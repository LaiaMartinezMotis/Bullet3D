#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	Cube aleron(info.aleron_size.x, info.aleron_size.y, info.aleron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aleron.transform);
	Cube camera(info.camera_reference.x, info.camera_reference.y, info.camera_reference.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&camera.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	btVector3 aleron_offset(info.aleron_offset.x, info.aleron_offset.y, info.aleron_offset.z);
	aleron_offset = aleron_offset.rotate(q.getAxis(), q.getAngle());
	btVector3 camera_offset(info.camera_offset.x, info.camera_offset.y, info.camera_offset.z);
	camera_offset = camera_offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color = Pink;

	aleron.transform.M[12] += aleron_offset.getX();
	aleron.transform.M[13] += aleron_offset.getY();
	aleron.transform.M[14] += aleron_offset.getZ();
	aleron.color = Pink;

	camera.transform.M[12] += camera_offset.getX();
	camera.transform.M[13] += camera_offset.getY();
	camera.transform.M[14] += camera_offset.getZ();

	aleron.Render();
	chassis.Render();
	camera.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

//float PhysVehicle3D::Score() const
//{
//	return 0.0f;
//}

vec3 PhysVehicle3D::GetForwardVector()
{
	btVector3 viewdirection;
	viewdirection = vehicle->getForwardVector();
	vec3 cardirection;
	cardirection.Set(viewdirection.x(), viewdirection.y(), viewdirection.z());

	return vec3();
}

