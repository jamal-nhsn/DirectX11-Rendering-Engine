#pragma once

#include <vector>
#include <type_traits>

#define INVALID_INDEX (-1)

template<typename KeyType, typename ValueType>
class SparseSet
{
public:
	SparseSet() = default;
	SparseSet(const SparseSet& other) = default;
	~SparseSet() = default;

	ValueType& operator[](KeyType key)
	{
		return m_data[m_indices[static_cast<unsigned int>(key)]];
	}

	template<typename... Args>
	void Emplace(const KeyType& key, Args&&... args)
	{
		unsigned int keyAsIndex = static_cast<unsigned int>(key);

		// Expand to hold key.
		if (static_cast<unsigned int>(m_indices.size()) < keyAsIndex + 1) {
			m_indices.resize(keyAsIndex + 1, INVALID_INDEX);
		}

		// Key did not already have data.
		if (m_indices[keyAsIndex] == INVALID_INDEX) {
			// Assign last key index.
			m_indices[keyAsIndex] = static_cast<unsigned int>(m_keys.size());

			// Emplace at back.
			m_keys.emplace_back(key);
			m_data.emplace_back(std::forward<Args>(args)...);
		}
		else {
			// Overwrite existing data.
			m_keys[m_indices[keyAsIndex]] = key;
			m_data[m_indices[keyAsIndex]] = ValueType(args...);
		}
	}

	void Remove(const KeyType& key)
	{
		// Nothing to remove.
		if (!Has(key)) {
			return;
		}

		unsigned int keyIndex = m_indices[static_cast<unsigned int>(key)];
		unsigned int lastKeyIndex = static_cast<unsigned int>(m_keys.size()) - 1;

		KeyType lastKey = m_keys[lastKeyIndex];

		// Move last entry to removed entries place.
		m_keys[keyIndex] = std::move(m_keys[lastKeyIndex]);
		m_data[keyIndex] = std::move(m_data[lastKeyIndex]);

		// Update indices.
		m_indices[static_cast<unsigned int>(lastKey)] = keyIndex;
		m_indices[static_cast<unsigned int>(key)] = INVALID_INDEX;

		// Remove the last entry.
		m_keys.pop_back();
		m_data.pop_back();
	}

	void Swap(const KeyType& key1, const KeyType& key2)
	{
		unsigned int key1AsIndex = static_cast<unsigned int>(key1);
		unsigned int key2AsIndex = static_cast<unsigned int>(key2);

		// No work to do.
		if (key1AsIndex == key2AsIndex) {
			return;
		}

		unsigned int key1Index = m_indices[key1AsIndex];
		unsigned int key2Index = m_indices[key2AsIndex];

		std::swap(m_indices[key1AsIndex], m_indices[key2AsIndex]);

		std::swap(m_keys[key1Index], m_keys[key2Index]);
		std::swap(m_data[key1Index], m_data[key2Index]);
	}

	bool Has(const KeyType& key)
	{
		return static_cast<unsigned int>(key) < static_cast<unsigned int>(m_indices.size()) && m_indices[static_cast<unsigned int>(key)] != -1;
	}

	std::vector<unsigned int>& Indices() {
		return m_indices;
	}

	std::vector<KeyType>& Keys() {
		return m_keys;
	}

	std::vector<ValueType>& Data() {
		return m_data;
	}

private:
	std::vector<unsigned int> m_indices;
	std::vector<KeyType> m_keys;
	std::vector<ValueType> m_data;
};

// Simplified verion of SparseSet which does not store the keys.

template<typename KeyType, typename ValueType>
class SimpleSparseSet
{
public:
	SimpleSparseSet() = default;
	SimpleSparseSet(const SimpleSparseSet& other) = default;
	~SimpleSparseSet() = default;

	ValueType& operator[](const KeyType& key)
	{
		return m_data[m_indices[static_cast<unsigned int>(key)]];
	}

	template<typename... Args>
	void Emplace(const KeyType& key, Args&&... args)
	{
		unsigned int keyAsIndex = static_cast<unsigned int>(key);

		// Expand to hold key.
		if (static_cast<unsigned int>(m_indices.size()) < keyAsIndex + 1) {
			m_indices.resize(keyAsIndex + 1, INVALID_INDEX);
		}

		// Key did not already have data.
		if (m_indices[keyAsIndex] == INVALID_INDEX) {
			// Assign last data index.
			m_indices[keyAsIndex] = static_cast<unsigned int>(m_data.size());

			// Emplace at back.
			m_data.emplace_back(std::forward<Args>(args)...);
		}
		else {
			// Overwrite existing data.
			m_data[m_indices[keyAsIndex]] = ValueType(args...);
		}
	}

	void Remove(const KeyType& key)
	{
		// Nothing to remove.
		if (!Has(key)) {
			return;
		}

		unsigned int keyIndex = m_indices[static_cast<unsigned int>(key)];
		unsigned int lastKeyIndex = static_cast<unsigned int>(m_data.size()) - 1;

		KeyType lastKey = m_data[lastKeyIndex];

		// Move last entry to removed entries place.
		m_data[keyIndex] = std::move(m_data[lastKeyIndex]);

		// Update indices.
		m_indices[static_cast<unsigned int>(lastKey)] = keyIndex;
		m_indices[static_cast<unsigned int>(key)] = INVALID_INDEX;

		// Remove the last entry.
		m_data.pop_back();
	}

	void Swap(const KeyType& key1, const KeyType& key2)
	{
		unsigned int key1AsIndex = static_cast<unsigned int>(key1);
		unsigned int key2AsIndex = static_cast<unsigned int>(key2);

		// No work to do.
		if (key1AsIndex == key2AsIndex) {
			return;
		}

		unsigned int key1Index = m_indices[key1AsIndex];
		unsigned int key2Index = m_indices[key2AsIndex];

		std::swap(m_indices[key1AsIndex], m_indices[key2AsIndex]);

		std::swap(m_data[key1Index], m_data[key2Index]);
	}

	bool Has(const KeyType& key)
	{
		return static_cast<unsigned int>(key) < static_cast<unsigned int>(m_indices.size()) && m_indices[static_cast<unsigned int>(key)] != -1;
	}

	std::vector<unsigned int>& Indices() {
		return m_indices;
	}

	std::vector<ValueType>& Data() {
		return m_data;
	}

private:
	std::vector<unsigned int> m_indices;
	std::vector<ValueType> m_data;
};

#undef INVALID_INDEX