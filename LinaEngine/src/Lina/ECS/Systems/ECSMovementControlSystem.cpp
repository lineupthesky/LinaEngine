/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: ECSMovementControlSystem
Timestamp: 4/9/2019 3:30:12 PM

*/

#include "LinaPch.hpp"
#include "ECSMovementControlSystem.hpp"  

namespace LinaEngine
{
	void ECSMovementControlSystem::UpdateComponents(float delta, BaseECSComponent ** components)
	{
		ECSTransformComponent* transform = (ECSTransformComponent*)components[0];
		ECSMovementControlComponent* movementControl = (ECSMovementControlComponent*)components[1];

		Vector3F newPos = transform->transform.GetPosition() + (movementControl->movement * movementControl->axis->GetAmount() * delta);
		transform->transform.SetPosition(newPos);
		
	}
}
