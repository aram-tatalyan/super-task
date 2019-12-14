#pragma once

// required by std::set
#include <set>

// required by MemberId, DistanceType
#include "common_defs.h"


class Bridge {
private:
	DistanceType m_length;
	std::set<MemberId> m_hikers;

public:
	explicit Bridge(decltype(m_length) length) : m_length(length) {
	}

	decltype(m_length) get_length() const {
		return m_length;
	}

	void add_hiker(MemberId id) {
		m_hikers.insert(id);
	}

	void remove_hiker(MemberId id) {
		m_hikers.erase(id);
	}

	const decltype(m_hikers) & get_hikers() const {
		return m_hikers;
	}
};

