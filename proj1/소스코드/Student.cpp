#include <iostream>
#include <iomanip>
#include "Student.h"
ostream& operator<< (ostream& os, Student st) {
	os << left << setw(17) << st.name << setw(12) << st.student_id << setw(20) << st.department << setw(12) << st.birth_year << setw(13) << st.tel <<
		setw(16) << st.total_credit << setw(16) << st.total_avg;
	return os;
}

bool Student::operator <(Student& st) {
	switch (get_sort_type()) {
	case 1:	return this->get_name() < st.get_name(); break;
	case 2: return this->get_student_id() < st.get_student_id(); break;
	case 3: return this->get_birth_year() < st.get_birth_year(); break;
	case 4: return this->get_department() < st.get_department(); break;
	case 5: return this->get_total_credit() < st.get_total_credit(); break;
	case 6: return this->get_total_avg() < st.get_total_avg(); break;
	}
}