#include <iostream>
#include <map>
#include <limits>

using namespace std;

template<typename K, typename V>
class interval_map {
private:
	std::map<K, V> m_map;
public:
	// constructor associates whole range of K with val by inserting (K_min, val)
	// into the map
	interval_map(V const& val) {
		m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
	}

	void assign(K const& keyBegin, K const& keyEnd, V const& val) {

		//empty interval
		if (!(keyBegin < keyEnd)) return;

		auto end_ub = m_map.upper_bound(keyEnd);
		auto end_lb = m_map.lower_bound(keyEnd);


		if ((operator[](keyEnd) != val) && (end_lb == end_ub)) {
			if (keyEnd != std::numeric_limits<K>::max()) m_map.insert(std::make_pair(keyEnd, operator[](keyEnd)));
		}
		else if ((operator[](keyEnd) == val) && (end_lb != end_ub)) {
			m_map.erase(keyEnd);
		}

		auto begin_ub = m_map.upper_bound(keyBegin);
		auto begin_lb = m_map.lower_bound(keyBegin);
		
		if ((begin_lb == m_map.begin()) || prev(begin_lb)->second != val) {
			m_map.insert_or_assign(keyBegin, val);
		}
		else {
			if (begin_lb != begin_ub) m_map.erase(begin_lb);
		}

		auto cleaning_it = begin_ub;
		auto clean_final = m_map.lower_bound(keyEnd);
		if (cleaning_it != m_map.end()) {
			m_map.erase(cleaning_it, clean_final);
		}
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		return (--m_map.upper_bound(key))->second;
	}
};

int main() {
	return 0;
}
