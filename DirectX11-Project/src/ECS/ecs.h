#pragma once

#include "sparseset.h"

namespace Engine
{
	using EntityId = unsigned int;

	template<typename... ObservedTypes>
	struct Observe
	{
	};

	class GroupInvoker
	{
	public:
		GroupInvoker(
			void* group,
			void(*add)(void* groupPtr,EntityId entityId),
			void(*remove)(void* groupPtr, EntityId entityId)
		)
			: m_groupPtr(group), m_add(add), m_remove(remove)
		{
		}
		GroupInvoker(const GroupInvoker& other) = default;
		~GroupInvoker() = default;

		void Add(EntityId entityId)
		{
			m_add(m_groupPtr, entityId);
		}
		void Remove(EntityId entityId)
		{
			m_remove(m_groupPtr, entityId);
		}
		bool Exists()
		{
			return m_groupPtr;
		}

	private:
		void* m_groupPtr;
		void(*m_add)(void* groupPtr, EntityId entityId);
		void(*m_remove)(void* groupPtr, EntityId entityId);
	};

	struct ManagedSetBase
	{
	public:
		ManagedSetBase(
			void(*baseRemove)(ManagedSetBase* set, EntityId entityId),
			std::vector<unsigned int>& indices,
			std::vector<EntityId>& entityIds
		)
			: baseRemove(baseRemove)
			, owningGroup(nullptr, nullptr, nullptr)
			, indices(indices)
			, entityIds(entityIds)
		{
		}

	public:
		void(*baseRemove)(ManagedSetBase* set, EntityId entityId);
		GroupInvoker owningGroup;
		std::vector<GroupInvoker> observingGroups;

		std::vector<unsigned int>& indices;
		std::vector<EntityId>& entityIds;
	};

	template<typename T>
	struct ManagedSet : public ManagedSetBase
	{
	public:
		ManagedSet()
			: ManagedSetBase(&ManagedSet::BaseRemove, set.Indices(), set.Keys())
		{
		}

		static void BaseRemove(ManagedSetBase* set, EntityId entityId)
		{
			static_cast<ManagedSet<T>*>(set)->set.Remove(entityId);
		}

	public:
		SparseSet<EntityId, T> set;
	};


	// Statically distributes a unique index to each Component and Group type.

	class TypeCounter
	{
	private:
		static unsigned int NextTypeIndex()
		{
			static unsigned int typeIndexCounter = 0;
			return typeIndexCounter++;
		}

		static unsigned int NextGroupIndex()
		{
			static unsigned int groupIndexCounter = 0;
			return groupIndexCounter++;
		}

		friend class TypeIndexer;
	};

	class TypeIndexer
	{
	private:
		// Get the assigned index of Type and Group.
		template<typename T>
		static unsigned int ComponentIndex()
		{
			static unsigned int componentIndex = TypeCounter::NextTypeIndex();
			return componentIndex;
		}

		template<typename T>
		static unsigned int GroupIndex()
		{
			static unsigned int groupIndex = TypeCounter::NextGroupIndex();
			return groupIndex;
		}

		friend class Scene;

#ifndef NDEBUG
		friend class UsageSafety;
#endif
	};

#ifndef NDEBUG
	// Used to prevent modification of sparse sets while a group or view is active.
	class UsageSafety
	{
	private:
		template<typename T>
		static bool CanBeModified()
		{
			return s_setUserCounts.size() <= TypeIndexer::ComponentIndex<T>() ||
				s_setUserCounts[TypeIndexer::ComponentIndex<T>()] == 0;
		}

		static bool CanAllBeModified()
		{
			for (unsigned int i = 0; i < s_setUserCounts.size(); i++) {
				if (s_setUserCounts[i] != 0) {
					return false;
				}
			}
			return true;
		}

		template<typename... Types>
		static void IncrementUserCounts()
		{
			([] {
				unsigned int typeIndex = TypeIndexer::ComponentIndex<Types>();
				if (s_setUserCounts.size() < typeIndex + 1) {
					s_setUserCounts.resize(typeIndex + 1);
				}
				s_setUserCounts[typeIndex]++;
			}(), ...);
		}

		template<typename... Types>
		static void DecrementUserCounts() 
		{
			([] {
				unsigned int typeIndex = TypeIndexer::ComponentIndex<Types>();
				if (s_setUserCounts.size() < typeIndex + 1) {
					s_setUserCounts.resize(typeIndex + 1);
				}
				s_setUserCounts[typeIndex]--;
			}(), ...);
		}

	private:
		inline static std::vector<unsigned int> s_setUserCounts;

		friend class Scene;

		template<typename OwnedPack, typename ObservedPack>
		friend class ComponentGroup;

		template<typename... Types>
		friend class ComponentView;
	};
#endif
};