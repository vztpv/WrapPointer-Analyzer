
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

#include "MyGroupManager.h"
#include "readonly_parser.h"


class FileManager
{
public:
	inline static std::string fileName;
	static void ClearFile() {
		std::ofstream outFile;
		outFile.open(fileName);
		outFile.close();
	}
	static void Write(const std::string& str)
	{
		std::ofstream outFile;
		outFile.open(fileName, std::ios::app);
		outFile << str;
		outFile.close();
	}
	static void WriteLine(const std::string& str)
	{
		std::ofstream outFile;
		outFile.open(fileName, std::ios::app);
		outFile << str << "\n";
		outFile.close();
	}
};

std::set<long long> removed_tids;
std::set<long long> tids;
long long tid = 0; // total

std::string GetObjectId() {
	if (!removed_tids.empty()) {
		long long x = *removed_tids.begin();
		removed_tids.erase(x);
		tids.insert(x);
		return "object" + wiz::toStr(x);
	}
	tids.insert(tid);
	tid++;
	return "object" + wiz::toStr(tid);
}

template<class T>
class ALWAYS : public wiz::MGM::Condition<T>
{
public:
	virtual bool operator() (const wiz::MGM::Item<T>& item, const wiz::MGM::Group<T>& group) const {
		return true;
	}
};
template <class T>
class Print : public wiz::MGM::Func<T>
{
public:
	virtual void operator()(wiz::MGM::Item<T>& item)
	{
		std::cout << item.getName() << " " << item.getValue() << std::endl;
	}
};

std::string GetPtrId(std::string str)
{
	auto x = str.find('\"');
	if (std::string::npos != x) {
		return str.substr(0, x);
	}
	return str;
}

void _AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut);
void AddNewEmpty(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string x = GetPtrId(ut->GetItemList(0).Get(0));

	if (analyzer.IsGroupExist(x)) {
		std::cout << "internal error1\n";
		return;
	}
	else {
		analyzer.NewGroup(x, 1, 1);
		analyzer.AddGroupToGroup(x, "not-inited");
		analyzer.AddGroupToGroup(x, "ptr-list");
		analyzer.NewItem(x, "object-nullptr");
		analyzer.AddItemToGroup(x, "relation");
		analyzer.AddGroupToGroup(x, "object-nullptr");


		if (analyzer.IsGroupExist("deleted-pid" + x, "deleted-pid")) {
			analyzer.RemoveGroupFromGroup("deleted-pid" + x, "deleted-pid");
			analyzer.RemoveGroup("deleted-pid" + x);
		}
	}

	_AddNew(analyzer, ut);
}

void AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut, bool is_local);

void AddNewArray(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut, bool is_local);

void AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut, bool is_local)
{
	if (ut->GetUserTypeListSize() <= 0) {
		//std::cout << "intenal error2\n";
		return; 
	}
	auto inner = ut->GetUserTypeList(0);
	// 
	std::string y = GetObjectId();
	std::string x = GetPtrId(inner->GetName()); // PtrId
	if (analyzer.IsGroupExist(x)) {
		std::cout << "intenal error3\n";
		return;
	}
	else if (analyzer.IsGroupExist(y)) {
		std::cout << "internal error4\n";
		return;
	}
	else {
		if (analyzer.IsGroupExist("deleted" + y, "deleted")) {
			analyzer.RemoveGroupFromGroup("deleted" + y, "deleted");
			analyzer.RemoveGroup("deleted" + y);
		}
		if (analyzer.IsGroupExist("deleted-pid" + x, "deleted-pid")) {
			analyzer.RemoveGroupFromGroup("deleted-pid" + x, "deleted-pid");
			analyzer.RemoveGroup("deleted-pid" + x);
		}


 		analyzer.NewGroup(x, 1, 1);
		analyzer.AddGroupToGroup(x, "inited");
		analyzer.AddGroupToGroup(x, "ptr-list");
	
	
		analyzer.NewGroup(y, 1, 1);
		analyzer.AddGroupToGroup(y, "object-list");
		analyzer.AddGroupToGroup(y, "not-array");

		analyzer.NewItem(x, y);
		analyzer.AddItemToGroup(x, "relation");
	
		analyzer.AddGroupToGroup(x, y); //


		// offset
		analyzer.NewItem("offset" + x, "0");
		analyzer.AddItemToGroup("offset" + x, "offset");

	}
	//
	_AddNew(analyzer, inner);
}

void AddNewArray(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut, bool is_local)
{
	// 
	std::string y = GetObjectId();
	std::string x = GetPtrId(ut->GetItemList(0).Get(0)); // PtrId
	std::string num = ut->GetItemList(1).Get(0);

	if (analyzer.IsGroupExist(x)) {
		std::cout << "intenal error3\n";
		return;
	}
	else if (analyzer.IsGroupExist(y)) {
		std::cout << "internal error4\n";
		return;
	}
	else {
		if (analyzer.IsGroupExist("deleted" + y, "deleted")) {
			analyzer.RemoveGroupFromGroup("deleted" + y, "deleted");
			analyzer.RemoveGroup("deleted" + y);
		}
		if (analyzer.IsGroupExist("deleted-pid" + x, "deleted-pid")) {
			analyzer.RemoveGroupFromGroup("deleted-pid" + x, "deleted-pid");
			analyzer.RemoveGroup("deleted-pid" + x);
		}
		

		analyzer.NewGroup(x, 1, 1);
		analyzer.AddGroupToGroup(x, "inited");
		analyzer.AddGroupToGroup(x, "ptr-list");

		analyzer.NewGroup(y, 1, 1);
		analyzer.AddGroupToGroup(y, "object-list");
		analyzer.AddGroupToGroup(y, "array");
		analyzer.NewItem("array-size" + y, num);
		analyzer.AddItemToGroup("array-size" + y, "array-size");
		analyzer.NewItem(x, y);
		analyzer.AddItemToGroup(x, "relation");

		analyzer.AddGroupToGroup(x, y); //

		// offset
		analyzer.NewItem("offset" + x, "0");
		analyzer.AddItemToGroup("offset" + x, "offset");

		//
		
	}
	//
	_AddNew(analyzer, ut);
}

void Access(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut);
// ptr + integer.
void AddNewPlus(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	// todo!
	// ptr <- is deleted?? or nullptr? => Error
	std::string x = GetPtrId(ut->GetItemList(0).Get(0));
	
	if (std::string::size_type i; std::string::npos != (i = x.find('%'))) {
		std::string pid = x.substr(0, i);
		x = x.substr(i + 1, x.size() - i - 1);
		if (std::string::npos != (i = x.find('%'))) {
			std::string ppid = x.substr(0, i);
			std::string offset = x.substr(i + 1, x.size() - i - 1);

			if (analyzer.IsGroupExist(ppid, "not-inited")) { // nullptr..
				std::cout << "Accessed invalid pointer\n";
				return;
			}

			if (analyzer.IsGroupExist("deleted-pid" + ppid, "deleted-pid")) {
				std::cout << "Accessed deleted pointer\n";
				return;
			}

			std::string pobject;
			if (analyzer.GetValue(ppid, pobject)) {
				if (pobject == "object-nullptr") {
					std::cout << "Accessed deleted pointer\n";
					return;
				}
			}

			if (wiz::MGM::Item<std::string> x; analyzer.GetItem("offset" + ppid, x) && analyzer.IsGroupExist(pobject, "not-array")) {
				if (stoll(offset) != 0) { // stoll(x.getValue()) != 0) {
					std::cout << "Accessed maybe wrong pointer\n";
					return;
				}
			}
			if (wiz::MGM::Item<std::string> x; analyzer.GetItem("offset" + ppid, x) && analyzer.IsGroupExist(pobject, "array")) {
				if (wiz::MGM::Item<std::string> y; analyzer.GetItem(ppid, y)) {
					if (wiz::MGM::Item<std::string> z; analyzer.GetItem("array-size" + y.getValue(), z)) {
						if (long long a = stoll(x.getValue()) + stoll(offset); a < 0 || a >= stoll(z.getValue())) {
							std::cout << "Accessed maybe wrong index\n";
							return;
						}
					}
					// internal error
				}
				// internal error
			}

			analyzer.NewGroup(pid, 1, 1);
			analyzer.AddGroupToGroup(pid, "inited"); // has object?
			analyzer.AddGroupToGroup(pid, "ptr-list");

			if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a;  analyzer.GetGroup("relation", a)) {
				wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
				if (a->getItem(ppid, b)) {
					analyzer.NewItem(pid, b->getValue());

					analyzer.AddGroupToGroup(pid, b->getValue()); // 
				}
			}
			analyzer.AddItemToGroup(pid, "relation");

			if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a;  analyzer.GetGroup("offset", a)) {
				wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
				if (a->getItem("offset" + ppid, b)) {
					long long value = stoll(b->getValue());
					if (value != 0) {
						offset += value;
					}
				}
			}

			// offset
			analyzer.NewItem("offset" + pid, offset);
			analyzer.AddItemToGroup("offset" + pid, "offset");

			// array-size
			{
				wiz::MGM::Item<std::string> x; analyzer.GetItem(pid, x);
				if (wiz::MGM::Item<std::string> y; analyzer.GetItem(ppid, y)) {
					if (wiz::MGM::Item<std::string> z; analyzer.GetItem("array-size" + y.getValue(), z)) {
						analyzer.NewItem("array-size" + x.getValue(), std::to_string(stoll(z.getValue())));
						analyzer.AddItemToGroup("array-size" + x.getValue(), "array-size");
					}
				}
			}
		}
	}

}
void AddNewFromOther(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	auto inner = ut;
	std::string left = GetPtrId(inner->GetItemList(0).GetName());
	std::string right = GetPtrId(inner->GetItemList(0).Get(0));

	if (analyzer.IsGroupExist(left)) {
		std::cout << "internal error5\n";
		return;
	}
	// itemexist(left) ? - internal error..
	else if (!analyzer.IsGroupExist(right)) {
		std::cout << "internal error6\n";
		return;
	}
	else {
		analyzer.NewGroup(left, 1, 1);
		if (bool inited = analyzer.IsGroupExist(right, "inited")) {
			analyzer.AddGroupToGroup(left, "inited");
		}
		else if(analyzer.IsGroupExist(right, "not-inited")) {
			analyzer.AddGroupToGroup(left, "not-inited");
		}
		else {
			std::cout << "right is not exist.\n"; //
			return;
		}
		analyzer.AddGroupToGroup(left, "ptr-list");
		bool pass = false;
		

		if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a; analyzer.GetGroup("relation", a)) {
			wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
			if (a->getItem(right, b)) {
				analyzer.NewItem(left, b->getValue());
				analyzer.AddItemToGroup(left, "relation");

				analyzer.AddGroupToGroup(left, b->getValue()); // 

				if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a; analyzer.GetGroup("offset", a)) {
					wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
					if (a->getItem("offset" + right, b)) {
						analyzer.NewItem("offset" + left, b->getValue());
						analyzer.AddItemToGroup("offset" + left, "offset");
						pass = true;
					}
				}
			}
		}

		if (!pass) {
			//std::cout << "internal error7\n";
			//return;
		}
	}
}
void ReturnPtrId(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string id = GetPtrId(ut->GetItemList(0).Get(0));

	analyzer.RemoveGroupFromGroup(id, "inited", false);
	analyzer.RemoveGroupFromGroup(id, "not-inited", false);
	analyzer.RemoveGroupFromGroup(id, "ptr-list", false);

	analyzer.RemoveItemFromGroup(id, "relation");
	analyzer.RemoveItem(id);

	analyzer.RemoveGroup(id);

	// offset
	analyzer.RemoveItemFromGroup("offset" + id, "offset");
	analyzer.RemoveItem("offset" + id);
}
void Access(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string pid = GetPtrId(ut->GetItemList(0).Get(0));

	if (analyzer.IsGroupExist(pid, "not-inited")) {
		std::cout << "Accessed invalid pointer\n";
		return;
	}	
	std::string object_id;
	if (wiz::MGM::Item<std::string> object; analyzer.GetItem(pid, object)) {
		object_id = object.getValue();

		if (object_id == "object-nullptr") {
			std::cout << "Accessd not inited or null or deleted thing\n";
			return;
		}
	}
	if (analyzer.IsGroupExist("deleted-pid" + pid, "deleted-pid")) {
		std::cout << "Accessed deleted pointer\n";
		return;
	}
	if (wiz::MGM::Item<std::string> x; analyzer.GetItem("offset" + pid, x) && analyzer.IsGroupExist(object_id, "not-array")) {
		if (stoll(x.getValue()) != 0) {
			std::cout << "Accessed maybe wrong pointer\n";
			return;
		}
	}
	if (wiz::MGM::Item<std::string> x; analyzer.GetItem("offset" + pid, x) && analyzer.IsGroupExist(object_id, "array")) {
		if (wiz::MGM::Item<std::string> z; analyzer.GetItem("array-size" + object_id, z)) {
			if (long long a = stoll(x.getValue()); a < 0 || a >= stoll(z.getValue())) {
				std::cout << "Accessed maybe wrong index\n";
				return;
			}
		}
		// internal error
	}
}
void AccessArray(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string pid = GetPtrId(ut->GetItemList(0).Get(0));
	std::string _idx = ut->GetItemList(1).Get(0);
	size_t idx = stoll(_idx);

	if (analyzer.IsGroupExist(pid, "not-inited")) {
		std::cout << "Accessed invalid pointer\n";
		return;
	}
	if (analyzer.IsGroupExist("deleted-pid" + pid, "deleted-pid")) {
		std::cout << "Accessed deleted pointer\n";
		return;
	}

	std::string object_id;
	if (wiz::MGM::Item<std::string> object; analyzer.GetItem(pid, object)) {
		object_id = object.getValue();
		
		if (object_id == "object-nullptr") {
			std::cout << "Accessd not inited or null or deleted thing\n";
			return;
		}
	}

	wiz::MGM::Item<std::string> offset;
	if (analyzer.GetItem("offset" + pid, offset)) {
		idx += stoll(offset.getValue());
	}

	if (analyzer.IsGroupExist(object_id, "not-array")) {
		if (idx != 0) {
			std::cout << "Accessed wrong index\n";
			return;
		}
	}
	if (analyzer.IsGroupExist(object_id, "array")) {
		if (wiz::MGM::Item<std::string> z; analyzer.GetItem("array-size" + object_id, z)) {
			if (long long a = idx; a < 0 || a >= stoll(z.getValue())) {
				std::cout << "Accessed wrong index\n";
				return;
			}
		}
		// internal error
	}
}
void Assign(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	// pid <- pointer id
	std::string left_pid = GetPtrId(ut->GetItemList(0).GetName());
	std::string right_pid = GetPtrId(ut->GetItemList(0).Get(0));

	// self assign -> pass
	if (left_pid == right_pid) { return; } 

	// whether left is deleted..
	if (std::string left_obj_id; analyzer.IsItemExist(left_pid, "relation") &&
		analyzer.GetValue(left_pid, left_obj_id) // if success return true.
		) {
		if ( wiz::WizSmartPtr<wiz::MGM::Group<std::string>> x; 
			left_obj_id != "object-nullptr" && analyzer.GetGroup(left_obj_id, x))
		{	
			if (x->getGroupMemberN() == 1) {
				// memory leaks...
				std::cout << "Memory leak.. in Assign\n";
				return;
			}
		}
	}

	// left quit relation, (not-inited)
	analyzer.RemoveItemFromGroup(left_pid, "relation");
	
	if (std::string left_object; analyzer.GetValue(left_pid, left_object)) {
		analyzer.RemoveGroupFromGroup(left_pid, left_object);
	}
	
	analyzer.RemoveItem(left_pid);

	
	if (bool inited = false; inited = analyzer.IsGroupExist(right_pid, "inited")) {
		if (analyzer.IsGroupExist(left_pid, "not-inited")) {
			analyzer.RemoveGroupFromGroup(left_pid, "not-inited");
			analyzer.AddGroupToGroup(left_pid, "inited");
		}
	}
	else { // not inited?
		if (analyzer.IsGroupExist(left_pid, "inited")) {
			analyzer.RemoveGroupFromGroup(left_pid, "inited");
			analyzer.AddGroupToGroup(left_pid, "not-inited");
		}
	}


	// check deleted pid
	if (analyzer.IsGroupExist("deleted-pid" + left_pid)) {
		analyzer.RemoveGroupFromGroup("deleted-pid" + left_pid, "deleted-pid");
		analyzer.RemoveGroup("deleted-pid" + left_pid);
	}
	if (analyzer.IsGroupExist("deleted-pid" + right_pid)) {
		analyzer.NewGroup("deleted-pid" + left_pid, 1, 1);
		analyzer.AddGroupToGroup("deleted-pid" + left_pid, "deleted-pid");
	}

	// left enter right relation? (inited?) - if right is not inited?
	if (std::string value; analyzer.GetValue(right_pid, value)) {
		analyzer.NewItem(left_pid, value);

		analyzer.AddGroupToGroup(left_pid, value); // 
	}
	else {
		std::cout << "internal error8\n";
		return; // or throw?
	}
	analyzer.AddItemToGroup(left_pid, "relation");

	bool pass = false;
	if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a; analyzer.GetGroup("offset", a)) {
		wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
		if (a->getItem("offset" + right_pid, b)) {
			pass = analyzer.SetValue("offset" + left_pid, b->getValue());
		}
	}
}

void Delete(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string id = GetPtrId(ut->GetItemList(0).Get(0));
	std::string obj_id;

	if (analyzer.GetValue(id, obj_id)) {
		if (obj_id == "object-nullptr") {
			std::cout << "delete already deleted or not inited or null thing\n";
			return;
		}
	}
	else {
		std::cout << "tried delete already deleted thing1\n";
		return;
	}

	if (analyzer.IsGroupExist(obj_id, "array")) {
		std::cout << "tried delete not-array but it is array!\n";
		return;
	}

	if (analyzer.IsGroupExist("deleted" + obj_id, "deleted")) {
		std::cout << "tried delete already deleted thing2\n";
		return;
	}
	else if (analyzer.IsGroupExist(id, "not-inited")) {
		std::cout << "tried delete not inited thing\n";
		return;
	}

	if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a; analyzer.GetGroup("offset", a)) {
		wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
		if (a->getItem("offset" + id, b)) {
			std::string offset;
			analyzer.GetValue("offset" + id, offset);
			long long value = stoll(offset);

			if (analyzer.IsGroupExist(obj_id, "not-array")) {
				if (value != 0) {
					std::cout << "Deleted maybe wrong pointer\n";
					return;
				}
			}
			if (analyzer.IsGroupExist(obj_id, "array")) {
				if (wiz::MGM::Item<std::string> z; analyzer.GetItem("array-size" + obj_id, z)) {
					if (long long a = value; a < 0 || a >= stoll(z.getValue())) {
						std::cout << "Deleted maybe wrong index\n";
						return;
					}
				}
				// internal error
			}
		}
	}


	bool pass = false;

	auto _end = analyzer.groupItemEnd("relation");
	for (auto x = analyzer.groupItemBegin("relation"); x != _end; ++x) {
		if ((*x).isNULL()) { continue; }

		if ((*x)->getName() == id) {
			std::string object_name = (*x)->getValue();

			wiz::WizSmartPtr<wiz::MGM::Group<std::string>> temp;
			analyzer.GetGroup(object_name, temp);
			for (auto iter = temp->groupBegin(); iter != temp->groupEnd(); ++iter) {
				if (!iter->isNULL()) {
					std::string id = (*iter)->getName();

					analyzer.SetValue(id, "object-nullptr");
					analyzer.AddGroupToGroup(id, "object-nullptr");
					
					analyzer.NewGroup("deleted-pid" + id, 1, 1);
					analyzer.AddGroupToGroup("deleted-pid" + id, "deleted-pid");
				}
			}


			analyzer.NewGroup("deleted" + object_name, 1, 1);
			analyzer.AddGroupToGroup("deleted" + object_name, "deleted");

			
			
			analyzer.RemoveGroupFromGroup(object_name, "not-array"); //
			


			analyzer.RemoveGroupFromGroup(object_name, "object-list");
			analyzer.RemoveGroup(object_name);

			//
			long long object_no = stoll(object_name.substr(6, object_name.size() - 6));
			removed_tids.insert(object_no); // object
			tids.erase(object_no);


			pass = true;
			break;
		}
	}
	
	if (!pass) {
		std::cout << "internal error9\n";
		return;
	}
}

void DeleteArray(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string id = GetPtrId(ut->GetItemList(0).Get(0));
	std::string obj_id;

	if (analyzer.GetValue(id, obj_id)) {
		if (obj_id == "object-nullptr") {
			std::cout << "delete already deleted or not inited or null thing\n";
			return;
		}
	}
	else {
		std::cout << "tried delete already deleted thing1\n";
		return;
	}

	if (analyzer.IsGroupExist(obj_id, "not-array")) {
		std::cout << "tried delete array but it is not array!\n";
		return;
	}

	if (analyzer.IsGroupExist("deleted" + obj_id, "deleted")) {
		std::cout << "tried delete already deleted thing2\n";
		return;
	}
	else if (analyzer.IsGroupExist(id, "not-inited")) {
		std::cout << "tried delete not inited thing\n";
		return;
	}

	if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> a; analyzer.GetGroup("offset", a)) {
		wiz::WizSmartPtr<wiz::MGM::Item<std::string>> b;
		if (a->getItem("offset" + id, b)) {
			std::string offset;
			analyzer.GetValue("offset" + id, offset);
			long long value = stoll(offset);

			if (analyzer.IsGroupExist(obj_id, "array")) {
				if (value != 0) {
					std::cout << "Deleted maybe wrong index\n";
					return;
				}
				// internal error
			}
		}
	}


	bool pass = false;

	auto _end = analyzer.groupItemEnd("relation");
	for (auto x = analyzer.groupItemBegin("relation"); x != _end; ++x) {
		if ((*x)->getName() == id) {
			std::string object_name = (*x)->getValue();

			wiz::WizSmartPtr<wiz::MGM::Group<std::string>> temp;
			analyzer.GetGroup(object_name, temp);
			for (auto iter = temp->groupBegin(); iter != temp->groupEnd(); ++iter) {
				if (!iter->isNULL()) {
					std::string id = (*iter)->getName();

					analyzer.SetValue(id, "object-nullptr");
					analyzer.AddGroupToGroup(id, "object-nullptr"); 
					

					analyzer.NewGroup("deleted-pid" + id, 1, 1);
					analyzer.AddGroupToGroup("deleted-pid" + id, "deleted-pid");
				}
			}


			analyzer.NewGroup("deleted" + object_name, 1, 1);
			analyzer.AddGroupToGroup("deleted" + object_name, "deleted");

			

			analyzer.RemoveGroupFromGroup(object_name, "array"); // 
			analyzer.RemoveItemFromGroup("array-size" + object_name, "array-size");
			analyzer.RemoveItem("array-size" + object_name);


			analyzer.RemoveGroupFromGroup(object_name, "object-list");

			analyzer.RemoveGroup(object_name);



			long long object_no = stoll(object_name.substr(6, object_name.size() - 6));
			removed_tids.insert(object_no); // object
			tids.erase(object_no);


			pass = true;
			break;
		}
	}

	if (!pass) {
		std::cout << "internal error9\n";
		return;
	}
}


void _AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
		std::string name = ut->GetUserTypeList(i)->GetName();
		if (ut->GetUserTypeList(i)->GetIListSize() == 0) {
			continue;
		}

		if (name == "NewEmpty") {
			AddNewEmpty(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "New") {
			AddNew(analyzer, ut->GetUserTypeList(i), false); // object
		}
		else if (name == "NewLocal") {
			AddNew(analyzer, ut->GetUserTypeList(i), true); // object
		}
		else if (name == "NewArray") {
			AddNewArray(analyzer, ut->GetUserTypeList(i), false);
		}
		else if (name == "NewLocalArray") {
			AddNewArray(analyzer, ut->GetUserTypeList(i), true);
		}
		else if (name == "NewPlus") { // ptr + integer..
			AddNewPlus(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "NewFromOther") { // chk array? or object?
			AddNewFromOther(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "ReturnId") {
			ReturnPtrId(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "access") {
			Access(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "access_array") {
			AccessArray(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "assign") { // chk array or object?
			Assign(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "delete") { // chk array or object?
			Delete(analyzer, ut->GetUserTypeList(i));
		}
		else if (name == "delete[]") { // chk array or object?
			DeleteArray(analyzer, ut->GetUserTypeList(i));
		}
	}
}

// todo?
// if(testfunction(analyzer, ut)) { } else { stop! }

int main(void)
{
	try {
		// load data using header only LoadData!
		wiz2::load_data::UserType global;
		wiz::MGM::GroupManager<std::string> analyzer("");

		wiz2::load_data::LoadData::LoadDataFromFile("../Memory Test/output.txt", global);

		// analyzer init
		analyzer.NewGroup("inited", 1, 1);
		analyzer.NewGroup("not-inited", 1, 1);
		analyzer.NewGroup("deleted", 1, 1);
		analyzer.NewGroup("ptr-list", 1, 1);
		analyzer.NewGroup("object-list", 1, 1);
		analyzer.NewGroup("object-nullptr", 1, 1); //
		analyzer.AddGroupToGroup("object-nullptr", "object-list"); //
		analyzer.NewGroup("relation", 1, 1);
		
		analyzer.NewGroup("array", 1, 1); // linked object! not pointer
		analyzer.NewGroup("not-array", 1, 1); // linked object! not pointer
		analyzer.NewGroup("array-size", 1, 1); // linked object! not pointer.

		analyzer.NewGroup("offset", 1, 1);
		analyzer.NewGroup("deleted-pid", 1, 1); // linked pointer.

		// loop 
		for (int i = 0; i < global.GetUserTypeListSize(); ++i) {
			// read line?
			const std::string name = global.GetUserTypeList(i)->GetName();

			// chk not inited, memory leaks, double delete, new-delete macthing, and etc.. ( using my group manager )
			if (name == "NewEmpty") {
				AddNewEmpty(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "New") { 
				AddNew(analyzer, global.GetUserTypeList(i), false); // object
			}
			else if (name == "NewLocal") {
				AddNew(analyzer, global.GetUserTypeList(i), true); // object
			}
			else if (name == "NewArray") {
				AddNewArray(analyzer, global.GetUserTypeList(i), false);
			}
			else if (name == "NewLocalArray") {
				AddNewArray(analyzer, global.GetUserTypeList(i), true);
			}
			else if (name == "NewPlus") { // ptr + integer..
				AddNewPlus(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "NewFromOther") { // chk array? or object?
				AddNewFromOther(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "ReturnId") { 
				ReturnPtrId(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "access") {
				Access(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "access_array") {
				AccessArray(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "assign") { // chk array or object?
				Assign(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "delete") { // chk array or object?
				Delete(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "delete[]") { // chk array or object?
				DeleteArray(analyzer, global.GetUserTypeList(i));
			}
		}

		if (wiz::WizSmartPtr<wiz::MGM::Group<std::string>> x; analyzer.GetGroup("object-list", x)) {
			if (x->getGroupMemberN() > 1) { // include object-nullptr
				std::cout << "memory leaks when program ends.\n";
			}
		}
	}
	catch (std::exception & e) {
		std::cout << e.what() << "\n";
	}
	catch (...) {
		std::cout << "internal error" << "\n";
	}

	return 0;
}

