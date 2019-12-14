#pragma once

// required by std::string
#include <string>

// required by BridgeId, MemberId
#include "common_defs.h"


class Hiker {
private:
	std::string m_name;
	//MemberId m_id;
	DistanceType m_speed;
	BridgeId m_bridge_id;
public:
	Hiker(const std::string & name,
			//MemberId id,
			decltype(m_speed) speed,
			BridgeId bridge_id
		) : 
		m_name(name),
		//m_id(id),
		m_speed(speed),
		m_bridge_id(bridge_id)
		{
	}

	//Hiker() = default;
	//Hiker(const Hiker &) = default;

	decltype(m_speed) get_speed() const {
		return m_speed;
	}
	
	//MemberId get_id() const {
	//	return m_id;
	//}

	//bool operator < (const Hiker & other) const {
	//	return m_id < other.m_id;
	//}

	//bool operator == (const Hiker & other) const {
	//	return m_id == other.m_id;
	//}

	std::string get_name() const {
		return m_name;
	}

	BridgeId get_bridge_id() const {
		return m_bridge_id;
	}

	void set_bridge_id(BridgeId bridge_id) {
		m_bridge_id = bridge_id;
	}
};
