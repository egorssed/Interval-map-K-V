#include <iostream>
#include "UnitTests.h"
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

template<typename K, typename V>
class interval_map {
public:
	std::map<K, V> m_map;
public:
	// constructor associates whole range of K with val by inserting (K_min, val)
	// into the map
	interval_map(V const& val) {
		m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
	}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
		
		//empty interval
		if (!(keyBegin < keyEnd)) return;

		auto end_ub = m_map.upper_bound(keyEnd);
		auto end_lb = m_map.lower_bound(keyEnd);

		
		if ((operator[](keyEnd) != val) && (end_lb == end_ub)) {//другое значение в точке
			//keyEnd не попал в точку 
			//keyEnd не попал в конец диапазона
			if(keyEnd!= std::numeric_limits<K>::max()) m_map.insert(std::make_pair(keyEnd, operator[](keyEnd)));
		}
		else if ((operator[](keyEnd) == val) && (end_lb != end_ub)) {//то же значение
			//keyEnd попал в точку
			m_map.erase(keyEnd);
		}

		auto begin_ub = m_map.upper_bound(keyBegin);
		auto begin_lb = m_map.lower_bound(keyBegin);
		//интересно значение в записи если попали в нее
		//или значение перед keyBegin если нет
		if ((begin_lb == m_map.begin()) || prev(begin_lb)->second != val) {
			m_map.insert_or_assign(keyBegin, val);
		}
		else {
			//если попали в запись , то ее надо удалить
			if (begin_lb != begin_ub) m_map.erase(begin_lb);
		}

		//очистка записей в (keyBegin,keyEnd)
		auto cleaning_it = begin_ub;
		//чтобы получить интервал до которого надо удалять записи
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

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of unsigned int intervals to char.

using interval = interval_map<unsigned int, char>;

//тесты
bool operator==(const interval& lhs, const interval& rhs) {
	return (lhs.m_map == rhs.m_map);
}

std::map<unsigned int, char> testBeginEnd() {
	interval t('A');
	t.assign(1, 1, 'c');
	return t.m_map;
}

std::map<unsigned int, char> testBeginLowest() {
	interval t('A');
	t.assign(numeric_limits<unsigned int>::lowest(), 1, 'c');
	return t.m_map;
}

std::map<unsigned int, char> testBeginMax() {
	interval t('A');
	t.assign(numeric_limits<unsigned int>::max(), 1, 'c');
	return t.m_map;
}

std::map<unsigned int, char> testEndMax() {
	interval t('A');
	t.assign(1, numeric_limits<unsigned int>::max(), 'c');
	return t.m_map;
}

std::map<unsigned int, char> testBeginInValueA() {
	interval t('A');
	t.assign(1, 2, 'c');
	t.assign(2, 3, 'q');
	t.assign(2, 5, 'q');
	return t.m_map;
}

std::map<unsigned int, char> testBeginInValueB() {
	interval t('A');
	t.assign(1, 2, 'c');
	t.assign(2, 3, 'q');
	t.assign(2, 5, 'c');
	return t.m_map;
}

std::map<unsigned int, char> testBeginInValueC() {
	interval t('A');
	t.assign(1, 2, 'c');
	t.assign(2, 3, 'q');
	t.assign(2, 5, 'd');
	return t.m_map;
}

std::map<unsigned int, char> testEndInValueA() {
	interval t('A');
	t.assign(1, 3, 'c');
	t.assign(3, 4, 'q');
	t.assign(2, 3, 'q');
	return t.m_map;
}

std::map<unsigned int, char> testEndInValueB() {
	interval t('A');
	t.assign(1, 3, 'c');
	t.assign(3, 4, 'q');
	t.assign(2, 3, 'd');
	return t.m_map;
}

std::map<unsigned int, char> testNotInValue() {
	interval t('A');
	t.assign(1, 3, 'c');
	t.assign(3, 5, 'd');
	t.assign(5, 7, 'p');
	t.assign(4, 6, 'q');
	return t.m_map;
}

std::map<unsigned int, char> testBeginNotInValueA() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(3, 7, 'c');
	return t.m_map;
}
std::map<unsigned int, char> testBeginNotInValueB() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(3, 7, 'd');
	return t.m_map;
}

std::map<unsigned int, char> testEndNotInValueA() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(3, 7, 'd');
	return t.m_map;
}

std::map<unsigned int, char> testEndNotInValueB() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(3, 7, 'c');
	return t.m_map;
}

std::map<unsigned int, char> testFromMinToMax() {
	interval t('A');
	t.assign(numeric_limits<unsigned int>::lowest(), numeric_limits<unsigned int>::max(), 'c');
	return t.m_map;
}

std::map<unsigned int, char> testEndLowest() {
	interval t('A');
	t.assign(1, numeric_limits<unsigned int>::lowest(), 'c');
	return t.m_map;
}

std::map<unsigned int, char> testBothNotInValue() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(5, 7, 'd');
	t.assign(3, 6, 'q');
	return t.m_map;
}

std::map<unsigned int, char> testBothInValue() {
	interval t('A');
	t.assign(1, 5, 'c');
	t.assign(8, 10, 'd');
	t.assign(5, 8, 'q');
	return t.m_map;
}


void TestWorks() {
	std::map<unsigned int, char> m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} };
	AssertEqual(testBeginEnd(), m_map, "Begin=End");

	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} };
	AssertEqual(testBeginMax(), m_map, "Begin=Max");

	m_map = { {std::numeric_limits<unsigned int>::lowest(),'c'},{1,'A'} };
	AssertEqual(testBeginLowest(), m_map, "Begin=Lowest");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'c'} };
	AssertEqual(testFromMinToMax(), m_map, "from min to max");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{2, 'q'},{5,'A'} };
	AssertEqual(testBeginInValueA(), m_map, "BeginInValueA");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{5,'A'} };
	AssertEqual(testBeginInValueB(), m_map, "BeginInValueB");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{2, 'd'},{5,'A'} };
	AssertEqual(testBeginInValueC(), m_map, "BeginInValueC");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{7,'A'} };
	AssertEqual(testBeginNotInValueA(), m_map, "BeginNotInValueA");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{3, 'd'},{7,'A'} };
	AssertEqual(testBeginNotInValueB(), m_map, "BeginNotInValueB");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} };
	AssertEqual(testEndLowest(), m_map, "End=lowest");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'} };
	AssertEqual(testEndMax(), m_map, "End=Max");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{2, 'q'},{4,'A'} };
	AssertEqual(testEndInValueA(), m_map, "EndInValueA");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{2, 'd'},{3, 'q'},{4,'A'} };
	AssertEqual(testEndInValueB(), m_map, "EndInValueB");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{3, 'd'},{7,'A'} };
	AssertEqual(testEndNotInValueA(), m_map, "EndNotInValueA");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{7,'A'} };
	AssertEqual(testEndNotInValueB(), m_map, "EndNotInValueB");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{3,'q'},{6,'d'},{7,'A'} };
	AssertEqual(testBothNotInValue(), m_map, "BothNotInValue");
	
	m_map = { {std::numeric_limits<unsigned int>::lowest(),'A'} ,{1, 'c'},{5,'q'},{8,'d'},{10,'A'} };
	AssertEqual(testBothInValue(), m_map, "BothInValue");
}




int main() {

	//тесты
	TestRunner tr;
	tr.RunTest(TestWorks, "TestWorks");
	int z;
	cin >> z;
	return 0;
}

