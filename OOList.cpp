#include "OOList.h"

//Helpers

//Function that will compare ID pointer for record to be inserted with one already in the list, if a match is found, returns 1,
//otherwise, returns 0
int compareID(char* newRec, char* listRec);

//Function that will make both char arrays lowercase and then compare them in order to determine alphabetical order,
//if the new lastname comes first, will return a negative, if same lastname, returns 0, if new lastname comes second, returns a positive
int compareLast(char* newLast, char* listLast);


//////////Function definitions


/////////////////////Rec class

//Assigns parameters to the member variables, allocates memory for the id field before copying data over
void rec::SetData(char* id_in, char* fn, char* ln) {
	if (id) {
		delete[] id;
		id = NULL;
	}
	//Set the fields of the rec with new data
	strcpy_s(firstname, strlen(fn) + 1, fn);
	strcpy_s(lastname, strlen(ln) + 1, ln);
	if (id_in) {
		id = new char[strlen(id_in) + 1];
		strcpy_s(id, strlen(id_in) + 1, id_in);
	}
}

//Public constructor
rec::rec(char* i, char* fn, char* ln) {
	id = NULL;
	//Initialize the fields of the new rec with data
	strcpy_s(firstname, strlen(fn) + 1, fn);
	strcpy_s(lastname, strlen(ln) + 1, ln);
	if (i) {
		id = new char[strlen(i) + 1];
		strcpy_s(id, strlen(i) + 1, i);
	}
	previous = NULL;
	next = NULL;
}

//Null public contructor
rec::rec(void) {
	id = NULL;
	firstname[0] = NULL;
	lastname[0] = NULL;
	previous = NULL;
	next = NULL;
}

//Deep copy a rec
void rec::operator=(const rec& r) {
	if (this->id) {
		delete[] this->id;
		id = NULL;
	}
	//Initialize the fields of the new rec with data from r
	strcpy_s(firstname, strlen(r.firstname) + 1, r.firstname);
	strcpy_s(lastname, strlen(r.lastname) + 1, r.lastname);
	if (r.id) {
		id = new char[strlen(r.id) + 1];
		strcpy_s(id, strlen(r.id) + 1, r.id);
	}
	next = r.next;
	previous = r.previous;
	return *this;
}

//Public copy constructor function
rec::rec(const rec& r) {
	id = NULL;
	//Initialize the fields of the new rec with data from r
	strcpy_s(firstname, strlen(r.firstname) + 1, r.firstname);
	strcpy_s(lastname, strlen(r.lastname) + 1, r.lastname);
	if (r.id) {
		id = new char[strlen(r.id) + 1];
		strcpy_s(id, strlen(r.id) + 1, r.id);
	}
	next = r.next;
	previous = r.previous;
}

//Displays a single record
void rec::Print() {
	std::cout << id << " ";
	std::cout << firstname << " ";
	std::cout << lastname << " ";
	std::cout << previous << " ";  //  address of previous item
	std::cout << this << " ";   // address of item
	std::cout << next << " " << std::endl;  //  address of next item
}

//Destructor that frees memory in record only if the id is in use
rec::~rec() {
	if (id != NULL) {
		delete[] id;
		id = NULL;
	}
}

/////////////////////List class

//Add a new record to the list, calls the copy constructor for rec, returns 0 if add failed, returns 1 if add successful
int list::AddItem(rec r) {
	//Make sure memory is available, call copy constructor
	rec* recPtr = new rec(r);
	if (recPtr == NULL) {
		std::cout << "Memory full, add failed\n";
		return 0;
	}
	//If list is empty
	if (size == 0) {
		recPtr->next = NULL;
		recPtr->previous = NULL;
		first = recPtr;
		last = recPtr;
		size++;
		return 1;
	}

	//Initialize index pointer to first list element
	current = first;

	//Iterate through list
	while (current != NULL) {
		//If matching ID found, new record is bad, delete
		if (compareID(recPtr->id, current->id) == 1) {
			delete recPtr;
			recPtr = NULL;
			std::cout << "Failed to add the new record (duplicate ID number)\n";
			return 0;
		}
		//Keep iterating
		else {
			current = current->next;
		}
	}


	//If it gets to here, no duplicate ID, reset position to start
	current = first;

	//Will check for alphabetical order based on lastname
	while (current) {
		//Enter if block when new record comes before current record alphabetically
		if (compareLast(recPtr->lastname, current->lastname) <= 0) {
			//If new record comes before the current first element in the list
			if (current == first) {
				first = recPtr;
				recPtr->previous = NULL;
				recPtr->next = current;
				current->previous = recPtr;
				size++;
				return 1;
			}
			//If inserting somewhere in the middle of the list
			else {
				recPtr->previous = current->previous;
				recPtr->next = current;
				current->previous->next = recPtr;
				current->previous = recPtr;
				size++;
				return 1;
			}
		}
		//Keep iterating
		current = current->next;
	}
	//At end of the list, new rec becomes the last element
	current = last;
	last = recPtr;
	recPtr->next = NULL;
	recPtr->previous = current;
	current->next = recPtr;
	size++;
	return 1;
}

//Takes in a character pointer ID, searches list and if found, removes record from list, also deallocates the memory, returns 1 if successfully
//deleted, returns 0 if delete failed or record not in list
int list::DeleteItem(char* delid) {
	//Try to delete from empty list
	if (size == 0) {
		std::cout << "Delete failed, list is empty\n";
		return 0;
	}

	//Initialize iterator
	current = first;

	//Iterator through list
	while (current != NULL) {
		//If a match found, enter if block
		if (compareID(delid, current->id) == 1) {
			//If deleting the only item in list
			if (size == 1) {
				first = NULL;
				last = NULL;
			}
			//Deleting first element
			else if (current == first) {
				first = current->next;
				first->previous = NULL;
			}
			//Deleting last element
			else if (current == last) {
				last = current->previous;
				last->next = NULL;
			}
			//Deleting somewhere in the middle
			else {
				current->next->previous = current->previous;
				current->previous->next = current->next;
			}
			//Physically delete
			delete current;
			size--;
			std::cout << "Delete successful\n";
			current = NULL;
			return 1;
		}
		//Keep iterating
		else {
			current = current->next;
		}
	}

	//Makes it to end of list without finding ID
	std::cout << "Delete failed, ID not in list\n";
	return 0;
}

//If 0 entered, prints list in ascending order, if 1 entered, prints list in descending order
//One record printed per line, space between each field, prints ID, Firstname, Lastname, address of previous record, 
//address of current record, and address of next record
void list::PrintList(int order) {
	//Ascending order
	if (order == 0) {
		//Initialize iterator
		current = first;
		//Iterate through until end reached
		while (current) {
			current->Print();
			current = current->next;
		}
	}
	//Descending order
	else {
		//Iterate through until end reached
		current = last;
		while (current) {
			current->Print();
			current = current->previous;
		}
	}
	std::cout << std::endl;
}

//Write a rec to the list by calling AddItem
int list::Write(rec r) {
	return AddItem(r);
}

//Read the list by calling PrintList
void list::Read(int order) {
	PrintList(order);
}

//Deep copy one list to another by first deleting all records in this list and then copying each record one by one into this list
void list::operator=(list l) {
	while (this->first) {
		if (this->first->next) {
			this->first = this->first->next;
			delete this->first->previous;
		}
		else {
			delete this->first;
			this->first = NULL;
		}
	}

	this->first = NULL;
	this->last = NULL;
	this->current = NULL;

	size = 0;

	if (l.first != NULL) {
		this->first = new rec(*(l.first));
		this->last = this->first;
		this->current = this->first;
		l.current = l.first;
		while (l.current->next) {
			l.current = l.current->next;
			this->current->next = new rec(*(l.current));
			this->current->next->previous = this->current;
			this->current = this->current->next;
			this->last = this->current;
		}
		l.current = NULL;
	}
	size = l.size;
	
	this->current = NULL;
}

//Default constructor, assigns the first and last member variables to NULL
list::list(void) {
	first = NULL;
	last = NULL;
	current = NULL;
	size = 0;
}


////////////Helper definitions
int compareID(char* newRec, char* listRec) {
	if (strcmp(newRec, listRec) != 0)
		return 0;//no match
	else
		return 1;//matching ID found
}

int compareLast(char* newLast, char* listLast) {
	char newLastBuff[15];
	char listLastBuff[15];
	strcpy_s(newLastBuff, strlen(newLast) + 1, newLast);
	strcpy_s(listLastBuff, strlen(listLast) + 1, listLast);
	//Convert copies of both char arrays to lowercase, then compare
	for (int i = 0; newLastBuff[i]; i++) {
		newLastBuff[i] = tolower(newLastBuff[i]);
	}
	for (int j = 0; listLastBuff[j]; j++) {
		listLastBuff[j] = tolower(listLastBuff[j]);
	}
	return strcmp(newLastBuff, listLastBuff);
}