#pragma once

#include "ecs.h"
#include <cassert>

namespace Engine
{
	template<typename OwnedPack, typename ObservedPack>
	class ComponentGroup;

	class ComponentGroupBase
	{
	};

	/*==================================================================================================
	-----------------------------------FULL AND PARTIAL OWNING GROUPS-----------------------------------
	==================================================================================================*/

	template<typename... OwnedTypes, typename... ObservedTypes>
	class ComponentGroup<std::tuple<OwnedTypes...>, Observe<ObservedTypes...>> : public ComponentGroupBase
	{
	private:
		using ComponentGroup_t = ComponentGroup<std::tuple<OwnedTypes...>, Observe<ObservedTypes...>>;

	private:
		ComponentGroup(
			std::tuple<ManagedSet<OwnedTypes>&...> ownedSets,
			std::tuple<ManagedSet<ObservedTypes>&...> observedSets
		)
			: m_last(0)
			, m_ownedSets(ownedSets)
			, m_observedSets(observedSets)
		{
			([this] {
				auto& ownedSet = std::get<ManagedSet<OwnedTypes>&>(m_ownedSets);
				// Do not allow owner to change.
				assert(("Error: Set already owned by another ComponentGroup!", !ownedSet.owningGroup.Exists()));
				ownedSet.owningGroup = GroupInvoker(this, &ComponentGroup_t::Add, &ComponentGroup_t::Remove);
			}(), ...);
			([this] {
				auto& observedSet = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
				observedSet.observingGroups.emplace_back(this, &ComponentGroup_t::Add, &ComponentGroup_t::Remove);
			}(), ...);

			Initialize();
		}

	public:
#ifndef NDEBUG
		ComponentGroup(const ComponentGroup& other)
			: m_last(other.m_last)
			, m_ownedSets(other.m_ownedSets)
			, m_observedSets(other.m_observedSets)
		{
			// Only increment the user counts here since the constructed Group is never returned directly.
			UsageSafety::IncrementUserCounts<OwnedTypes..., ObservedTypes...>();
		}

		~ComponentGroup()
		{
			UsageSafety::DecrementUserCounts<OwnedTypes..., ObservedTypes...>();
		}
#endif
		template<typename Action>
		void Each(Action&& action)
		{
			if constexpr (std::is_invocable_v<Action, EntityId, OwnedTypes&..., ObservedTypes&...>) {
				// Iterate through group subset.
				for (unsigned int i = 0; i < m_last; i++) {
					// Grab entity id from arbitary owned set.
					EntityId entityId = std::get<0>(m_ownedSets).entityIds[i];
					action(
						entityId,
						std::get<ManagedSet<OwnedTypes>&>(m_ownedSets).set.Data()[i]...,
						Get<ObservedTypes>(entityId)...
					);
				}
			}
			else if constexpr (std::is_invocable_v<Action, OwnedTypes&..., ObservedTypes&...>) {
				for (unsigned int i = 0; i < m_last; i++) {
					if constexpr (sizeof...(ObservedTypes) == 0) {
						// No need to grab entity id at all in this case.
						action(std::get<ManagedSet<OwnedTypes>&>(m_ownedSets).set.Data()[i]...);
					}
					else {
						EntityId entityId = std::get<0>(m_ownedSets).entityIds[i];
						action(
							std::get<ManagedSet<OwnedTypes>&>(m_ownedSets).set.Data()[i]...,
							Get<ObservedTypes>(entityId)...
						);
					}
				}
			}
			else {
				static_assert(false, "Error: Invalid arguments taken by function passed to ComponentGroup.Each()!");
			}
		}

		template<typename T>
		T& Get(EntityId entityId)
		{
			if constexpr ((std::is_same_v<T, OwnedTypes> || ...)) {
				return std::get<ManagedSet<T>&>(m_ownedSets).set[entityId];
			}
			else if constexpr ((std::is_same_v<T, ObservedTypes> || ...)) {
				return std::get<ManagedSet<T>&>(m_observedSets).set[entityId];
			}
			else {
				static_assert(false, "Error: Type T is not part of ComponentGroup!");
			}
		}

		bool Has(EntityId entityId)
		{
			return (std::get<ManagedSet<OwnedTypes>&>(m_ownedSets).set.Has(entityId) && ...) &&
				(std::get<ManagedSet<ObservedTypes>&>(m_observedSets).set.Has(entityId) && ...);
		}

		unsigned int Size()
		{
			return m_last;
		}

	private:
		void Initialize()
		{
			unsigned int smallest = static_cast<unsigned int>(std::get<0>(m_ownedSets).entityIds.size());
			bool inOwned = true;

			// Find the smallest set and where it is contained.
			([this, &smallest] {
				auto& set = std::get<ManagedSet<OwnedTypes>&>(m_ownedSets);
				smallest = static_cast<unsigned int>(set.entityIds.size()) < smallest ? static_cast<unsigned int>(set.entityIds.size()) : smallest;
			}(), ...);

			([this, &smallest, &inOwned] {
				auto& set = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
				if (static_cast<unsigned int>(set.entityIds.size()) < smallest) {
					smallest = static_cast<unsigned int>(set.entityIds.size());
					inOwned = false;
				}
			}(), ...);

			// Iterate through smallest set and try to add its entity ids to group.
			bool completed = false;
			if (inOwned) {
				([this, &smallest, &completed] {
					if (completed) {
						return;
					}
					auto& set = std::get<ManagedSet<OwnedTypes>&>(m_ownedSets);
					if (static_cast<unsigned int>(set.entityIds.size()) != smallest) {
						return;
					}
					for (EntityId entityId : set.entityIds) {
						Add(this, entityId);
					}
					completed = true;
				}(), ...);
			}
			else {
				([this, &smallest, &completed] {
					if (completed) {
						return;
					}
					auto& set = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
					if (static_cast<unsigned int>(set.entityIds.size()) != smallest) {
						return;
					}
					for (EntityId entityId : set.entityIds) {
						Add(this, entityId);
					}
					completed = true;
				}(), ...);
			}
		}

		static void Add(void* groupPtr, EntityId entityId)
		{
			ComponentGroup_t& group = *static_cast<ComponentGroup_t*>(groupPtr);

			// Check if all sets contain the entity id.
			if ((std::get<ManagedSet<OwnedTypes>&>(group.m_ownedSets).set.Has(entityId) && ...) &&
				(std::get<ManagedSet<ObservedTypes>&>(group.m_observedSets).set.Has(entityId) && ...)) {
				// Add entity id to back of subset and increase subset size.
				([&] {
					ManagedSet<OwnedTypes>& managedSet = std::get<ManagedSet<OwnedTypes>&>(group.m_ownedSets);
					EntityId lastEntityId = managedSet.entityIds[group.m_last];
					managedSet.set.Swap(entityId, lastEntityId);
				}(), ...);
				group.m_last++;
			}
		}

		static void Remove(void* groupPtr, EntityId entityId)
		{
			ComponentGroup_t& group = *static_cast<ComponentGroup_t*>(groupPtr);

			// Check if all sets contain the entity id.
			if ((std::get<ManagedSet<OwnedTypes>&>(group.m_ownedSets).set.Has(entityId) && ...) &&
				(std::get<ManagedSet<ObservedTypes>&>(group.m_observedSets).set.Has(entityId) && ...)) {
				// Decrease subset size and push entity id outside of subset.
				group.m_last--;
				([&] {
					ManagedSet<OwnedTypes>& managedSet = std::get<ManagedSet<OwnedTypes>&>(group.m_ownedSets);
					EntityId lastEntityId = managedSet.entityIds[group.m_last];
					managedSet.set.Swap(entityId, lastEntityId);
				}(), ...);
			}
		}

	public:
		class Iterator;

		Iterator begin()
		{
			return Iterator(*this);
		}

		Iterator end()
		{
			return Iterator(*this, m_last);
		}

	private:
		unsigned int m_last;
		std::tuple<ManagedSet<OwnedTypes>&...> m_ownedSets;
		std::tuple<ManagedSet<ObservedTypes>&...> m_observedSets;

		friend class Scene;
		friend Iterator;

	public:
		class Iterator
		{
		private:
			Iterator(ComponentGroup_t& group, unsigned int index = 0)
				: m_group(group), m_index(index)
			{
			}

		public:
			Iterator(const Iterator& other) = default;
			~Iterator() = default;

			bool operator!=(const Iterator& other)
			{
				return m_index != other.m_index;
			}
			Iterator& operator++()
			{
				m_index++;
				return *this;
			}
			auto operator*()
			{
				EntityId entityId = std::get<0>(m_group.m_ownedSets).entityIds[m_index];
				return std::tuple<EntityId, OwnedTypes&..., ObservedTypes&...>{
					entityId,
					std::get<ManagedSet<OwnedTypes>&>(m_group.m_ownedSets).set.Data()[m_index]...,
					m_group.Get<ObservedTypes>(entityId)...
				};
			}

		private:
			ComponentGroup_t& m_group;
			unsigned int m_index;

			friend ComponentGroup_t;
		};
	};

	/*==================================================================================================
	---------------------------------------FULL NON-OWNING GROUPS---------------------------------------
	==================================================================================================*/

	template<typename... ObservedTypes>
	class ComponentGroup<std::tuple<>, Observe<ObservedTypes...>> : public ComponentGroupBase
	{
	private:
		using ComponentGroup_t = ComponentGroup<std::tuple<>, Observe<ObservedTypes...>>;

	private:
		ComponentGroup(std::tuple<ManagedSet<ObservedTypes>&...> observedSets)
			: m_observedSets(observedSets)
		{
			([this] {
				auto& observedSet = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
				observedSet.observingGroups.emplace_back(
					this,
					&ComponentGroup_t::Add,
					&ComponentGroup_t::Remove
				);
			}(), ...);

			Initialize();
		}

	public:
#ifndef NDEBUG
		ComponentGroup(const ComponentGroup& other)
			: m_observedSets(other.m_observedSets), m_entityIds(other.m_entityIds)
		{
			UsageSafety::IncrementUserCounts<ObservedTypes...>();
		}

		~ComponentGroup()
		{
			UsageSafety::DecrementUserCounts<ObservedTypes...>();
		}
#endif
		template<typename Action>
		void Each(Action&& action)
		{
			if constexpr (std::is_invocable_v<Action, EntityId, ObservedTypes&...>) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(m_entityIds.Data().size()); i++) {
					EntityId entityId = m_entityIds.Data()[i];
					action(entityId, Get<ObservedTypes>(entityId)...);
				}
			}
			else if constexpr (std::is_invocable_v<Action, ObservedTypes&...>) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(m_entityIds.Data().size()); i++) {
					EntityId entityId = m_entityIds.Data()[i];
					action(Get<ObservedTypes>(entityId)...);
				}
			}
			else {
				static_assert(false, "Error: Invalid arguments taken by function passed to Group.Each()!");
			}
		}

		template<typename T>
		T& Get(EntityId entity)
		{
			if constexpr ((std::is_same_v<T, ObservedTypes> || ...)) {
				return std::get<ManagedSet<T>&>(m_observedSets).set[entity];
			}
			else {
				static_assert(false, "Error: Type T is not part of Group!");
			}
		}

		bool Has(EntityId entityId)
		{
			return (std::get<ManagedSet<ObservedTypes>&>(m_observedSets).set.Has(entityId) && ...);
		}

		unsigned int Size()
		{
			return static_cast<unsigned int>(m_entityIds.Data().size());
		}

	private:
		void Initialize()
		{
			// Find the smallest set.
			unsigned int smallest = static_cast<unsigned int>(std::get<0>(m_observedSets).entityIds.size());
			([this, &smallest] {
				auto& set = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
				if (static_cast<unsigned int>(set.entityIds.size()) < smallest) {
					smallest = static_cast<unsigned int>(set.entityIds.size());
				}
			}(), ...);

			// Try to add each entity of the smallest set to the group.
			bool completed = false;
			([this, &smallest, &completed] {
				if (completed) {
					return;
				}
				auto& set = std::get<ManagedSet<ObservedTypes>&>(m_observedSets);
				if (static_cast<unsigned int>(set.entityIds.size()) != smallest) {
					return;
				}
				for (EntityId entity : set.entityIds) {
					Add(this, entity);
				}
				completed = true;
			}(), ...);
		}

		static void Add(void* groupPtr, EntityId entityId)
		{
			ComponentGroup_t& group = *static_cast<ComponentGroup_t*>(groupPtr);
			if ((std::get<ManagedSet<ObservedTypes>&>(group.m_observedSets).set.Has(entityId) && ...)) {
				group.m_entityIds.Emplace(entityId, entityId);
			}
		}

		static void Remove(void* groupPtr, EntityId entityId)
		{
			ComponentGroup_t& group = *static_cast<ComponentGroup_t*>(groupPtr);
			if ((std::get<ManagedSet<ObservedTypes>&>(group.m_observedSets).set.Has(entityId) && ...)) {
				group.m_entityIds.Remove(entityId);
			}
		}

	public:
		class Iterator;

		Iterator begin()
		{
			return Iterator(*this);
		}

		Iterator end()
		{
			return Iterator(*this, static_cast<unsigned int>(m_entityIds.Data().size()));
		}

	private:
		std::tuple<ManagedSet<ObservedTypes>&...> m_observedSets;
		SimpleSparseSet<EntityId, EntityId> m_entityIds;

		friend class Scene;

	public:
		class Iterator
		{
		private:
			Iterator(ComponentGroup_t& group, unsigned int index = 0)
				: m_group(group), m_index(index)
			{
			}

		public:
			Iterator(const Iterator& other) = default;
			~Iterator() = default;

			bool operator!=(const Iterator& other)
			{
				return m_index != other.m_index;
			}
			Iterator& operator++()
			{
				m_index++;
				return *this;
			}
			auto operator*()
			{
				EntityId entity = m_group.m_entityIds.Data()[m_index];
				return std::tuple<EntityId, ObservedTypes&...>{ entity, m_group.Get<ObservedTypes>(entity)... };
			}

		private:
			ComponentGroup_t& m_group;
			unsigned int m_index;

			friend ComponentGroup_t;
		};
	};
}