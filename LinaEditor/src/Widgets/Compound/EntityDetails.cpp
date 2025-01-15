/*
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Editor/Widgets/Compound/EntityDetails.hpp"
#include "Editor/Widgets/CommonWidgets.hpp"
#include "Editor/EditorLocale.hpp"
#include "Editor/Editor.hpp"
#include "Editor/Actions/EditorActionEntity.hpp"
#include "Editor/Actions/EditorActionComponent.hpp"
#include "Common/Math/Math.hpp"
#include "Core/GUI/Widgets/Layout/DirectionalLayout.hpp"
#include "Core/GUI/Widgets/Layout/FoldLayout.hpp"
#include "Core/GUI/Widgets/Primitives/InputField.hpp"
#include "Core/GUI/Widgets/Primitives/Text.hpp"
#include "Core/GUI/Widgets/WidgetManager.hpp"
#include "Core/World/Entity.hpp"
#include "Core/World/EntityWorld.hpp"
#include "Core/World/Component.hpp"
#include "Core/Application.hpp"

namespace Lina::Editor
{
	void EntityDetails::Construct()
	{
		m_editor = Editor::Get();

		m_layout = m_manager->Allocate<DirectionalLayout>("Vertical");
		m_layout->GetFlags().Set(WF_POS_ALIGN_X | WF_POS_ALIGN_Y | WF_SIZE_ALIGN_X | WF_SIZE_ALIGN_Y);
		m_layout->SetAlignedPos(Vector2::Zero);
		m_layout->SetAlignedSize(Vector2::One);
		m_layout->GetProps().direction				   = DirectionOrientation::Vertical;
		m_layout->GetWidgetProps().childPadding		   = Theme::GetDef().baseIndent;
		m_layout->GetWidgetProps().childMargins.top	   = Theme::GetDef().baseIndent;
		m_layout->GetWidgetProps().childMargins.bottom = Theme::GetDef().baseIndent;
		AddChild(m_layout);

		m_noDetailsText = m_manager->Allocate<Text>("NoEntityDetails");
		m_noDetailsText->GetFlags().Set(WF_POS_ALIGN_X | WF_POS_ALIGN_Y);
		m_noDetailsText->SetAlignedPos(Vector2(0.5f, 0.5f));
		m_noDetailsText->SetAnchorX(Anchor::Center);
		m_noDetailsText->SetAnchorY(Anchor::Center);
		m_noDetailsText->GetProps().font = Theme::GetDef().altFont;
		m_noDetailsText->UpdateTextAndCalcSize(Locale::GetStr(LocaleStr::NoEntityDetails));
		AddChild(m_noDetailsText);
	}

	void EntityDetails::Destruct()
	{
		for (OStream& stream : m_editingComponentsBuffer)
			stream.Destroy();
		m_editingComponentsBuffer.clear();
	}

	void EntityDetails::PreTick()
	{
		if (m_selectedEntities.size() != 1)
			return;

		UpdateDetails();
	}

	void EntityDetails::SetWorld(EntityWorld* w)
	{
		m_world = w;
		RefreshDetails();
	}

	void EntityDetails::OnEntitySelectionChanged(const Vector<Entity*>& entities)
	{
		m_selectedEntities = entities;
		RefreshDetails();
	}

	void EntityDetails::RefreshDetails()
	{
		m_noDetailsText->GetFlags().Set(WF_HIDE, !m_selectedEntities.empty());

		m_layout->DeallocAllChildren();
		m_layout->RemoveAllChildren();

		if (m_selectedEntities.empty())
			return;

		UpdateDetails();

		Widget* name					   = CommonWidgets::BuildField(m_layout,
												   Locale::GetStr(LocaleStr::Name),
												   &m_dummyDetails.name,
																	   {
																		   .type = FieldType::String,
												   });
		name->GetCallbacks().onEditStarted = [this]() { StartEditingName(); };
		name->GetCallbacks().onEditEnded   = [this]() { StopEditingName(); };
		m_layout->AddChild(name);

		Widget* pos						  = CommonWidgets::BuildField(m_layout,
												  Locale::GetStr(LocaleStr::Position),
												  &m_dummyDetails.pos,
																	  {
																		  .type = FieldType::Vector3,
												  });
		pos->GetCallbacks().onEditStarted = [this]() { StartEditingTransform(); };
		pos->GetCallbacks().onEdited	  = [this]() { UpdateEntities(); };
		pos->GetCallbacks().onEditEnded	  = [this]() { StopEditingTransform(); };
		m_layout->AddChild(pos);

		Widget* rotation					   = CommonWidgets::BuildField(m_layout,
													   Locale::GetStr(LocaleStr::Rotation),
													   &m_dummyDetails.rot,
																		   {
																			   .type = FieldType::Vector3,
													   });
		rotation->GetCallbacks().onEditStarted = [this]() { StartEditingTransform(); };
		rotation->GetCallbacks().onEdited	   = [this]() { UpdateEntities(); };
		rotation->GetCallbacks().onEditEnded   = [this]() { StopEditingTransform(); };
		m_layout->AddChild(rotation);

		Widget* scale						= CommonWidgets::BuildField(m_layout,
													Locale::GetStr(LocaleStr::Scale),
													&m_dummyDetails.scale,
																		{
																			.type = FieldType::Vector3,
													});
		scale->GetCallbacks().onEditStarted = [this]() { StartEditingTransform(); };
		scale->GetCallbacks().onEdited		= [this]() { UpdateEntities(); };
		scale->GetCallbacks().onEditEnded	= [this]() { StopEditingTransform(); };
		m_layout->AddChild(scale);

		m_layout->AddChild(CommonWidgets::BuildSeperator(m_layout));

		DirectionalLayout* physicsSettingsWrapper = m_manager->Allocate<DirectionalLayout>("PhySettingsWrapper");
		physicsSettingsWrapper->GetFlags().Set(WF_POS_ALIGN_X | WF_SIZE_ALIGN_X | WF_SIZE_Y_TOTAL_CHILDREN);
		physicsSettingsWrapper->SetAlignedPosX(0.0f);
		physicsSettingsWrapper->SetAlignedSize(Vector2::One);
		physicsSettingsWrapper->GetProps().direction		  = DirectionOrientation::Vertical;
		physicsSettingsWrapper->GetWidgetProps().childPadding = m_layout->GetWidgetProps().childPadding;
		physicsSettingsWrapper->GetCallbacks().onEditStarted  = [this]() { StartEditingEntityPhysicsSettings(); };
		physicsSettingsWrapper->GetCallbacks().onEditEnded	  = [this]() { StopEditingEntityPhyiscsSettings(); };
		m_layout->AddChild(physicsSettingsWrapper);

		EntityPhysicsSettings& phySettings = m_selectedEntities.at(0)->GetPhysicsSettings();
		CommonWidgets::BuildClassReflection(physicsSettingsWrapper, &phySettings, ReflectionSystem::Get().Resolve<EntityPhysicsSettings>());

		m_layout->AddChild(CommonWidgets::BuildSeperator(m_layout));

		Vector<Component*> comps;
		m_world->GetComponents(m_selectedEntities.at(0), comps);

		for (Component* c : comps)
		{
			MetaType*	  meta	= ReflectionSystem::Get().Resolve(c->GetTID());
			const String& title = meta->GetProperty<String>("Title"_hs);

			Widget* foldTitle = CommonWidgets::BuildFoldTitle(m_layout, title, meta->GetFoldValuePtr());
			m_layout->AddChild(foldTitle);
			CommonWidgets::BuildClassReflection(foldTitle, c, meta);

			foldTitle->GetCallbacks().onEditStarted = [this, c]() {
				m_editingComponents = {c};
				StartEditingComponents();
			};

			foldTitle->GetCallbacks().onEditEnded = [this, c]() { StopEditingComponents(); };
		}
	}

	void EntityDetails::UpdateDetails()
	{
		if (m_isEditing)
			return;

		Entity* e			 = m_selectedEntities.at(0);
		m_dummyDetails.pos	 = e->GetLocalPosition();
		m_dummyDetails.rot	 = e->GetLocalRotation().GetPitchYawRoll();
		m_dummyDetails.scale = e->GetLocalScale();
		m_dummyDetails.name	 = e->GetName();
	}

	void EntityDetails::UpdateEntities()
	{
		if (Math::Equals(m_dummyDetails.scale.x, 0.0f, 0.001f))
			m_dummyDetails.scale.x = 0.001f;

		if (Math::Equals(m_dummyDetails.scale.y, 0.0f, 0.001f))
			m_dummyDetails.scale.y = 0.001f;

		if (Math::Equals(m_dummyDetails.scale.z, 0.0f, 0.001f))
			m_dummyDetails.scale.z = 0.001f;

		for (Entity* e : m_selectedEntities)
		{
			e->SetLocalPosition(m_dummyDetails.pos);
			e->SetLocalRotationAngles(m_dummyDetails.rot);
			e->SetLocalScale(m_dummyDetails.scale);
		}
	}
	void EntityDetails::StartEditingTransform()
	{
		m_isEditing = true;

		m_storedTransformations.resize(0);
		for (Entity* e : m_selectedEntities)
			m_storedTransformations.push_back(e->GetTransform());
	}

	void EntityDetails::StopEditingTransform()
	{
		m_isEditing = false;
		EditorActionEntityTransform::Create(m_editor, m_world->GetID(), m_selectedEntities, m_storedTransformations);
	}

	void EntityDetails::StartEditingName()
	{
		m_storedNames.resize(0);
		for (Entity* e : m_selectedEntities)
			m_storedNames.push_back(e->GetName());
	}

	void EntityDetails::StopEditingName()
	{
		for (Entity* e : m_selectedEntities)
			e->SetName(m_dummyDetails.name);
		EditorActionEntityNames::Create(m_editor, m_world->GetID(), m_selectedEntities, m_storedNames);
	}

	void EntityDetails::StartEditingComponents()
	{
		for (OStream& stream : m_editingComponentsBuffer)
			stream.Destroy();
		m_editingComponentsBuffer.resize(0);

		for (Component* c : m_editingComponents)
		{
			OStream stream;
			c->SaveToStream(stream);
			m_editingComponentsBuffer.push_back(stream);
		}
	}

	void EntityDetails::StopEditingComponents()
	{
		EditorActionComponentChanged::Create(m_editor, m_world->GetID(), {m_selectedEntities.at(0)}, {m_editingComponents.at(0)->GetTID()}, m_editingComponentsBuffer);
	}

	void EntityDetails::StartEditingEntityPhysicsSettings()
	{
		m_storedPhysicsSettings = {m_selectedEntities.at(0)->GetPhysicsSettings()};
	}

	void EntityDetails::StopEditingEntityPhyiscsSettings()
	{
		EditorActionEntityPhysicsSettingsChanged::Create(m_editor, m_world, {m_selectedEntities.at(0)}, m_storedPhysicsSettings);
	}

} // namespace Lina::Editor