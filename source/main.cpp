
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
	}
}

void _AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	if (ut->GetUserTypeListSize() <= 0) { 
		return; 
	}
	std::string parent_name = ut->GetName();

	for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
		AddNewEmpty(analyzer, ut->GetUserTypeList(i));
		
		_AddNew(analyzer, ut->GetUserTypeList(i));
	}
}
void AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	if (ut->GetUserTypeListSize() <= 0) {
		std::cout << "intenal error2\n";
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
 		analyzer.NewGroup(x, 1, 1);
		analyzer.AddGroupToGroup(x, "inited");
		analyzer.AddGroupToGroup(x, "ptr-list");
		
		analyzer.NewGroup(y, 1, 1);
		analyzer.AddGroupToGroup(y, "object-list");

		analyzer.NewItem(x, y);
		analyzer.AddItemToGroup(x, "relation");

		
	}
	//
	_AddNew(analyzer, inner);
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
	else if (!analyzer.IsGroupExist(right)) {
		std::cout << "internal error6\n";
		return;
	}
	else {
		analyzer.NewGroup(left, 1, 1);
		if (bool inited = analyzer.IsGroupExist(right, "inited")) {
			analyzer.AddGroupToGroup(left, "inited");
		}
		else {
			analyzer.AddGroupToGroup(left, "not-inited");
		}
		analyzer.AddGroupToGroup(left, "ptr-list");
		bool pass = false;
		
		for (auto end = analyzer.groupItemEnd("relation"), x = analyzer.groupItemBegin("relation"); x != end; ++x) {
			if ((*x)->getName() == right) {
				analyzer.NewItem(left, (*x)->getValue());
				analyzer.AddItemToGroup(left, "relation");
				pass = true;
				break;
			}
		}
		if (!pass) {
			std::cout << "internal error7\n";
			return;
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
}
void Access(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string pid = GetPtrId(ut->GetItemList(0).Get(0));

	if (analyzer.IsGroupExist(pid, "not-inited")) {
		std::cout << "Accessed invalid pointer\n";
		return;
	}
}
void Assign(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	// pid <- pointer id
	std::string left_pid = GetPtrId(ut->GetItemList(0).GetName());
	std::string right_pid = GetPtrId(ut->GetItemList(0).Get(0));

	// wheter left is deleted..
	if (std::string left_obj_id; analyzer.IsItemExist(left_pid, "relation") &&
		analyzer.GetValue(left_pid, left_obj_id) // if success return true.
		) {
		if (left_obj_id != "object-nullptr") {
			// memory leaks...
			std::cout << "Memory leak.. in Assign\n";
			return;
		}
	}

	// left quit relation, (not-inited)
	analyzer.RemoveItemFromGroup(left_pid, "relation");
	analyzer.RemoveItem(left_pid);

	bool inited = false;
	if (inited = analyzer.IsGroupExist(right_pid, "inited")) {
		if (analyzer.IsGroupExist(left_pid, "not-inited")) {
			analyzer.RemoveGroupFromGroup(left_pid, "not-inited");
			analyzer.AddGroupToGroup(left_pid, "inited");
		}
	}
	else {
		if (analyzer.IsGroupExist(left_pid, "inited")) {
			analyzer.RemoveGroupFromGroup(left_pid, "inited");
			analyzer.AddGroupToGroup(left_pid, "not-inited");
		}
	}

	// left enter right relation? (inited?) - if right is not inited?
	if (std::string value; analyzer.GetValue(right_pid, value)) {
		analyzer.NewItem(left_pid, value);
	}
	else {
		std::cout << "internal error8\n";
		return; // or throw?
	}
	analyzer.AddItemToGroup(left_pid, "relation");
}
void Delete(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string id = GetPtrId(ut->GetItemList(0).Get(0));
	std::string obj_id;

	if (analyzer.GetValue(id, obj_id)) {
		//
	}
	else {
		std::cout << "tried delete already deleted thing1\n";
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

	bool pass = false;

	auto _end = analyzer.groupItemEnd("relation");
	for (auto x = analyzer.groupItemBegin("relation"); x != _end; ++x) {
		if ((*x)->getName() == id) {
			std::string object_name = (*x)->getValue();

			analyzer.RemoveGroupFromGroup(object_name, "object-list");
			analyzer.NewGroup("deleted" + object_name, 1, 1);
			analyzer.AddGroupToGroup("deleted" + object_name, "deleted");

			analyzer.RemoveGroupFromGroup(object_name, "object-list");

			analyzer.RemoveGroup(object_name);
			analyzer.RemoveItemFromGroup(id, "relation");
			analyzer.RemoveItem(id);
			
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

// todo?
// if(testfunction(analyzer, ut)) { } else { stop! }

int main(void)
{
	try {
		// load data using header only LoadData!
		wiz2::load_data::UserType global;
		wiz::MGM::GroupManager<std::string> analyzer("");

		wiz2::load_data::LoadData::LoadDataFromFile("../WrapPointer Logger/output.txt", global);

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

		// loop 
		for (int i = 0; i < global.GetUserTypeListSize(); ++i) {
			// read line?
			const std::string name = global.GetUserTypeList(i)->GetName();

			// chk not inited, memory leaks, double delete, new-delete macthing, and etc.. ( using my group manager )
			if (name == "NewEmpty") {
				AddNewEmpty(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "New") { 
				AddNew(analyzer, global.GetUserTypeList(i)); // object
			}
			else if (name == "NewArray") {
				// todo..
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
			else if (name == "assign") { // chk array or object?
				Assign(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "delete") { // chk array or object?
				Delete(analyzer, global.GetUserTypeList(i));
			}
			else if (name == "delete[]") { // chk array or object?
				// todo..
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

