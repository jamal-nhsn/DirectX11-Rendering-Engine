#pragma once

namespace Engine
{
	template<typename... Types>
	class ComponentView
	{
	private:
		ComponentView(std::tuple<ManagedSet<Types>&...> sets)
			: m_smallestSet(nullptr), m_sets(sets)
		{
#ifndef NDEBUG
			UsageSafety::IncrementUserCounts<Types...>();
#endif
			([this] {
				// Find the smallest set.
				auto& set = std::get<ManagedSet<Types>&>(m_sets);
				unsigned int size = static_cast<unsigned int>(set.entityIds.size());
				if (!m_smallestSet || set.entityIds.size() < m_smallestSet->entityIds.size()) {
					m_smallestSet = &set;
				}
			}(), ...);
		}

	public:
#ifndef NDEBUG
		ComponentView(const ComponentView& other)
			: m_sets(other.m_sets)
			, m_smallestSet(other.m_smallestSet)
		{
			UsageSafety::IncrementUserCounts<Types...>();
		}

		~ComponentView()
		{
			UsageSafety::DecrementUserCounts<Types...>();
		}
#endif
		template<typename Action>
		void Each(Action&& action)
		{
			// Iterate the smallest sets entity ids.
			for (auto entityId : m_smallestSet->entityIds) {
				if (Has(entityId)) {
					if constexpr (std::is_invocable_v<Action, EntityId, Types&...>) {
						action(entityId, Get<Types>(entityId)...);
					}
					// Don't provide entity id if it is not needed.
					else if constexpr (std::is_invocable_v<Action, Types&...>) {
						action(Get<Types>(entityId)...);
					}
					else {
						static_assert(false, "Error: Invalid arguments taken by function passed to View.Each()!");
					}
				}
			}
		}

		template<typename T>
		T& Get(EntityId entityId)
		{
			return std::get<ManagedSet<T>&>(m_sets).set[entityId];
		}

		bool Has(EntityId entityId)
		{
			return (std::get<ManagedSet<Types>&>(m_sets).set.Has(entityId) && ...);
		}

		unsigned int MaxSize()
		{
			// Return the max possible size.
			return static_cast<unsigned int>(m_smallestSet->entityIds.size());
		}

		class Iterator;

		Iterator begin()
		{
			unsigned int first = 0;
			std::vector<EntityId>& entityIds = m_smallestSet->entityIds;
			while (first < entityIds.size() && !Has(entityIds[first])) {
				first++;
			}
			return Iterator(*this, first);
		}

		Iterator end()
		{
			return Iterator(*this, static_cast<unsigned int>(m_smallestSet->entityIds.size()));
		}

	private:
		std::tuple<ManagedSet<Types>&...> m_sets;
		ManagedSetBase* m_smallestSet;

		friend class Scene;

	public:
		class Iterator
		{
		private:
			Iterator(ComponentView& view, unsigned int index = 0)
				: m_view(view), m_index(index)
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
				// Find next entity id that has all of the components.
				std::vector<EntityId>& entityIds = m_view.m_smallestSet->entityIds;
				while (m_index < entityIds.size() && !m_view.Has(entityIds[m_index])) {
					m_index++;
				}
				return *this;
			}
			auto operator*() 
			{
				EntityId entityId = m_view.m_smallestSet->entityIds[m_index];
				return std::tuple<EntityId, Types&...>{
					entityId, m_view.Get<Types>(entityId)...
				};
			}

		private:
			ComponentView& m_view;
			unsigned int m_index;

			friend class ComponentView;
		};
	};

	/*==================================================================================================
	---------------------------------------SINGLE COMPONENT VIEWS---------------------------------------
	==================================================================================================*/

	template<typename T>
	class ComponentView<T>
	{
	private:
		ComponentView(ManagedSet<T>& set)
			: m_set(set)
		{
#ifndef NDEBUG
			UsageSafety::IncrementUserCounts<T>();
#endif
		}

	public:
#ifndef NDEBUG
		ComponentView(const ComponentView& other)
			: m_set(other.m_set)
		{
			UsageSafety::IncrementUserCounts<T>();
		}

		~ComponentView()
		{
			UsageSafety::DecrementUserCounts<T>();
		}
#endif
		template<typename Action>
		void Each(Action&& action) 
		{
			if constexpr (std::is_invocable_v<Action, EntityId, T&>) {
				unsigned int index = 0;
				for (EntityId entityId : m_set.entityIds) {
					action(entityId, m_set.set.Data()[index]);
					index++;
				}
			}
			else if constexpr (std::is_invocable_v<Action, T&>) {
				for (T& component : m_set.set.Data()) {
					action(component);
				}
			}
			else {
				static_assert(false, "Error: Invalid arguments taken by function passed to View.Each()!");
			}
		}

		T& Get(EntityId entityId) 
		{
			return m_set.set[entityId];
		}

		bool Has(EntityId entityId)
		{
			return m_set.set.Has(entityId);
		}

		unsigned int Size()
		{
			return static_cast<unsigned int>(m_set.set.Data().size());
		}

		class Iterator;

		Iterator begin() 
		{
			return Iterator(*this);
		}

		Iterator end() 
		{
			return Iterator(*this, static_cast<unsigned int>(m_set.entityIds.size()));
		}

	private:
		ManagedSet<T>& m_set;

		friend class Scene;

	public:
		class Iterator
		{
		private:
			Iterator(ComponentView& view, unsigned int index = 0)
				: m_view(view), m_index(index)
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
				return std::tuple<EntityId, T&>{ m_view.m_set.entityIds[m_index], m_view.m_set.set.Data()[m_index] };
			}

		private:
			ComponentView& m_view;
			unsigned int m_index;

			friend ComponentView;
		};
	};
}