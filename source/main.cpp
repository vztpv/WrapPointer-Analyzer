
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

#include "MyGroupManager.h"
#include "readonly_parser.h"


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
void _AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	if (ut->GetUserTypeListSize() <= 0) { 
		return; 
	}
	std::string parent_name = ut->GetName();

	for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
		std::string x = GetPtrId(ut->GetUserTypeList(i)->GetName());
		if (analyzer.IsGroupExist(x)) {
			// chk memory leak
			return;
		}
		else {
			analyzer.RemoveGroupFromGroup("object" + x, "deleted");
			analyzer.NewGroup(x, 1, 1);
			analyzer.AddGroupToGroup(x, "not-inited");
			analyzer.AddGroupToGroup(x, "ptr-list");
			analyzer.AddGroupToGroup(x, parent_name);
			analyzer.NewItem(x, "object-nullptr");
			analyzer.AddItemToGroup(x, "relation");

		}
		//
		_AddNew(analyzer, ut->GetUserTypeList(i));
	}
}
void AddNew(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	if (ut->GetUserTypeListSize() <= 0) { 
		return; 
	}
	auto inner = ut->GetUserTypeList(0);
	// 
	std::string y = GetObjectId();
	std::string x = GetPtrId(inner->GetName()); // PtrId
	if (analyzer.IsGroupExist(x)) {
		// chk memory leak...
		return;
	}
	else {
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
		// chk error..?
		return;
	}
	else if (!analyzer.IsGroupExist(right)) {
		// chk memory leak...
		return;
	}
	else {
		analyzer.NewGroup(left, 1, 1);
		analyzer.AddGroupToGroup(left, "inited");
		analyzer.AddGroupToGroup(left, "ptr-list");
		bool pass = false;
		auto end = analyzer.groupItemEnd("relation");
		for (auto x = analyzer.groupItemBegin("relation"); x != end; ++x) {
			if (wiz::String::startsWith((*x)->getName(), right)) {
				analyzer.NewItem(left, (*x)->getValue());
				analyzer.AddItemToGroup(left, "relation");
				pass = true;
				break;
			}
		}
		if (!pass) {
			// error?
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
void Delete(wiz::MGM::GroupManager<std::string>& analyzer, wiz2::load_data::UserType* ut)
{
	std::string id = GetPtrId(ut->GetItemList(0).Get(0));
	bool pass = false;

	auto _end = analyzer.groupItemEnd("relation");
	for (auto x = analyzer.groupItemBegin("relation"); x != _end; ++x) {
		if ((*x)->getName() == id) {
			std::string object_name = (*x)->getValue();

			analyzer.RemoveGroupFromGroup(object_name, "object-list");
			analyzer.AddGroupToGroup("deleted" + object_name, "deleted");

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
		// error?
		return;
	}
}


int main(void)
{
	// load data using header only LoadData!
	wiz2::load_data::UserType global;
	wiz::MGM::GroupManager<std::string> analyzer("");

	wiz2::load_data::LoadData::LoadDataFromFile("../WrapPointer Logger/output.txt", global);

	// analyzer init
	analyzer.NewGroup("inited", 1, 1);
	analyzer.NewGroup("not-inited", 1, 1);
	analyzer.NewGroup("deleted", 1, 1);
	analyzer.NewGroup("dangling", 1, 1);
	analyzer.NewGroup("ptr-list", 1, 1);
	analyzer.NewGroup("object-list", 1, 1);
	analyzer.NewGroup("object-nullptr", 1, 1); //
	analyzer.AddGroupToGroup("object-nullptr", "object-list"); //
	analyzer.NewGroup("relation", 1, 1);

	// loop 
	for (int i = 0; i < global.GetUserTypeListSize(); ++i) {
		// read line?
		const std::string name = global.GetUserTypeList(i)->GetName();
		
		// chk not inited, memory leaks, double delete, new-delete macthing, and etc.. ( using my group manager )
		if (name == "New") {
			AddNew(analyzer, global.GetUserTypeList(i));
		}
		else if (name == "NewFromOther") {
			AddNewFromOther(analyzer, global.GetUserTypeList(i));
		}
		else if (name == "ReturnId") {
			ReturnPtrId(analyzer, global.GetUserTypeList(i));
		}
		else if (name == "access") {

		}
		else if (name == "assign") {

		}
		else if (name == "delete") {
			Delete(analyzer, global.GetUserTypeList(i));
		}
	}

	return 0;
}

