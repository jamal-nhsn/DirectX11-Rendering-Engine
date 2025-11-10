#pragma once

#include "ecs.h"
#include "componentgroup.h"
#include "componentview.h"
#include "freelist.h"

#include <memory>

#define INVALID_ID (-1)

namespace Engine 
{
	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene& other) = default;
		~Scene() = default;

		EntityId CreateEntity()
		{
			// Just need the index, it doesn't matter what we insert.
			return m_entityIds.Emplace();
		}

		template<typename T, typename... Args>
		void Emplace(EntityId entityId, Args&&... args)
		{
			assert(("Error: Component set cannot be modified as it is being utilized!", UsageSafety::CanBeModified<T>()));

			ManagedSet<T>& managedSet = GetOrCreateSet<T>();
			managedSet.set.Emplace(entityId, std::forward<Args>(args)...);

			// Perform group callbacks.
			if (managedSet.owningGroup.Exists()) {
				managedSet.owningGroup.Add(entityId);
			}
			for (GroupInvoker& observingGroup : managedSet.observingGroups) {
				observingGroup.Add(entityId);
			}
		}

		template<typename T>
		void Remove(EntityId entityId)
		{
			assert(("Error: Component set cannot be modified as it is being utilized!", UsageSafety::CanBeModified<T>()));

			ManagedSet<T>& managedSet = GetOrCreateSet<T>();

			// Perform group callbacks.
			if (managedSet.owningGroup.Exists()) {
				managedSet.owningGroup.Remove(entityId);
			}
			for (GroupInvoker& observingGroup : managedSet.observingGroups) {
				observingGroup.Remove(entityId);
			}

			managedSet.set.Remove(entityId);
		}

		template<typename T>
		T& Get(EntityId entityId)
		{
			return GetOrCreateSet<T>().set[entityId];
		}

		template<typename T>
		bool Has(EntityId entityId)
		{
			return GetOrCreateSet<T>().set.Has(entityId);
		}

		void RemoveAll(EntityId entityId) 
		{
			assert(("Error: Component sets cannot be modified as it is being utilized!", UsageSafety::CanAllBeModified()));
			assert(("Error: Trying to modify a deleted entity!", entityId != INVALID_ID));

			for (auto& setPtr : m_sets) {
				ManagedSetBase* set = setPtr.get();

				// Perform group callbacks.
				if (set->owningGroup.Exists()) {
					set->owningGroup.Remove(entityId);
				}
				for (GroupInvoker& observingGroup : set->observingGroups) {
					observingGroup.Remove(entityId);
				}

				set->baseRemove(set, entityId);
			}
		}

		void DestroyEntity(EntityId& entityId)
		{
			RemoveAll(entityId);
			m_entityIds.Remove(entityId);
			entityId = INVALID_ID;
		}

		template<typename... Types>
		ComponentView<Types...> View()
		{
			if constexpr (1 < sizeof...(Types)) {
				return ComponentView<Types...>(std::forward_as_tuple(GetOrCreateSet<Types>()...));
			}
			else {
				return ComponentView<Types...>(GetOrCreateSet<Types...>());
			}
		}

		template<typename... OwnedTypes, typename... ObservedTypes>
		auto Group(Observe<ObservedTypes...> = {})
		{
			using Group_t = ComponentGroup<std::tuple<OwnedTypes...>, Observe<ObservedTypes...>>;

			unsigned int groupIndex = TypeIndexer::GroupIndex<Group_t>();

			if (m_groups.size() < groupIndex + 1) {
				m_groups.resize(groupIndex + 1);
			}
			if (!m_groups[groupIndex]) {
				// Create unique group only once.
				// Full Non-Owning group.
				if constexpr (sizeof...(OwnedTypes) == 0) {
					m_groups[groupIndex] = std::unique_ptr<Group_t>(
						new Group_t(std::forward_as_tuple(GetOrCreateSet<ObservedTypes>()...))
					);
				}
				// Full/Partial owning group.
				else {
					m_groups[groupIndex] = std::unique_ptr<Group_t>(
						new Group_t(
							std::forward_as_tuple(GetOrCreateSet<OwnedTypes>()...),
							std::forward_as_tuple(GetOrCreateSet<ObservedTypes>()...)
						)
					);
				}
			}

			// Return a copy of the group to the caller.
			return *(static_cast<Group_t*>(m_groups[groupIndex].get()));
		}

	private:
		template<typename T>
		ManagedSet<T>& GetOrCreateSet()
		{
			unsigned int typeIndex = TypeIndexer::ComponentIndex<T>();
			if (m_sets.size() < typeIndex + 1) {
				m_sets.resize(typeIndex + 1);
			}
			if (!m_sets[typeIndex]) {
				m_sets[typeIndex] = std::make_unique<ManagedSet<T>>();
			}
			return *(static_cast<ManagedSet<T>*>(m_sets[typeIndex].get()));
		}


	private:
		std::vector<std::unique_ptr<ManagedSetBase>> m_sets;
		std::vector<std::unique_ptr<ComponentGroupBase>> m_groups;
		FreeList<EntityId> m_entityIds;
	};
}

#undef INVALID_ID