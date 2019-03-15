#ifndef MYGROUPMANAGER_H
#define MYGROUPMANAGER_H

#include <vector>
#include <string>
#include <algorithm>
#include "global.h"
#include "wizardError.h"
#include "wizSmartPointer.h" //

namespace wiz {
	namespace MGM // My Group Manager
	{
		template <class T>
		class Item;

		template <class T>
		class Group;

		template <class T>
		class Condition {
		public:
			virtual bool operator() (const Item<T>&, const Group<T>&) const = 0;
		};

		template <class T>
		class Func {
		public:
			virtual void operator() (Item<T>&) = 0;
		};

		template <class T>
		class ConditionWith {
		public:
			virtual bool operator() (const Item<T>&, const Item<T>&, const Group<T>&) const = 0;
		};

		template <class T>
		class FuncWith {
		public:
			virtual void operator() (Item<T>&, Item<T>&) = 0;
		};

		template <class T>
		class Item
		{
		private:
			std::string name; // key
			wiz::WizSmartPtr<T> value; // data
			size_t order; // 0, 1, 2, ...
		public:
			explicit Item() : name(""), order(0) { }
			explicit Item(const std::string& name, const T& value = T(), const size_t order = 0)
				: name(name), order(order)
			{
				wiz::WizSmartPtr<T> temp(new T());
				this->value = temp;
				*(this->value) = value;
			}
			explicit Item(const std::string& name, const wiz::WizSmartPtr<T>& value, const size_t order = 0)
				: name(name), order(order)
			{
				this->value = value;
			}
			virtual ~Item() { } // fixed! cout << "called" << endl; value.remove( true ); }
		public:
			bool empty() const { return value.isNULL(); }
			std::string getName()const { return name; }
			T getValue()const { return *value; }
			void setValue(const T& val) { *value = val; }

			bool operator==(const Item<T>& other) const
			{
				return this->name == other.name;
			}
			bool operator<(const Item<T>& other) const
			{
				return this->name < other.name;
			}
			// new comp<  by order.
		};
		// ItemArray! name, (index, value)+ ?

		template <class T>
		class Group
		{
		private:
			size_t order;
		public:
			bool hasNoItem() { renewalItem(); return 0 == itemMemberN; }
			bool hasNoGroup() { renewalGroup(); return 0 == groupMemberN; }
		public:
			typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator itemBegin() { return itemMember.begin(); }
			typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator itemEnd() { return itemMember.end(); }
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupBegin() { return groupMember.begin(); }
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupEnd() { return groupMember.end(); }
		private:
			std::string name;
			std::vector< wiz::WizSmartPtr< Item<T> > > itemMember;
			std::vector< wiz::WizSmartPtr< Group<T> > > groupMember;
			int itemMemberN; // for binary search!
			int groupMemberN; // for binary search!
		public:
			explicit Group() { }
			explicit Group(const std::string& name, const int itemMemberMax = 0, const int groupMemberMax = 0, const size_t order = 0)
				: name(name), itemMemberN(0), groupMemberN(0), order(0)
			{
				// chk if itemMemberN > 0 ?
				itemMember.reserve(itemMemberMax);
				groupMember.reserve(groupMemberMax);
			}
			virtual ~Group() /// chk!!!
			{ }
			bool operator==(const Group& group) const
			{
				return this->name == group.name;
			}
		public:
			std::string getName()const { return name; }
			int getItemMemberN() { renewalItem();  return itemMemberN; }
			int getGroupMemberN() { renewalGroup(); return groupMemberN; }

			bool isItemExist(const std::string& itemName)
			{
				int index = searchItem(itemName);
				return index > -1;// fixed 2012.5.4 && !itemMember[index].isNULL();
			}
			bool isGroupExist(const std::string& groupName)
			{
				int index = searchGroup(groupName);
				return index > -1;// fixed 2012.5.4 && !groupMember[index].isNULL();
			}
			// bool isItemExist( const int index );
			// bool isGroupExist( const int index );
			bool getItem(const std::string& itemName, wiz::WizSmartPtr< Item<T> >& ptr)
			{
				bool isSuccess = isItemExist(itemName);

				if (isSuccess) /// chk!!
				{
					const int index = searchItem(itemName);
					ptr = itemMember[index];
				}

				return isSuccess;
			}

			// void Do( void(*f)( Item<T>& ), bool (*condition)( Item<T>&, Group<T>& ) ); - chk!
			void Do(Func<T>& f, const Condition<T>& condition, bool isOnlyItem) // int or enum! ( ALL, RANDOM- 00.00% )
			{
				/// todo- op, op_offmultiset set!, random!
				// for item,
				for (int i = 0; i < itemMemberN; i++)
				{
					if (!itemMember[i].isNULL() && condition(*itemMember[i], *this))
					{
						f(*itemMember[i]);
					}
				}

				if (!isOnlyItem)
				{
					for (int i = 0; i < groupMemberN; i++)
					{
						if (!groupMember[i].isNULL())
						{
							groupMember[i]->Do(f, condition, isOnlyItem);
						}
					}
				}
			}

			void Do(void(*f)(const Item<T>&, const Item<T>&), bool(*condition)(const Item<T>&, const Item<T>&, const Group<T>&), bool isOnlyItem) // int or enum! ( ALL, RANDOM- 00.00% )
			{
				/// todo- op, op_offmultiset set!, random!
				// for item,
				for (int i = 0; i < itemMemberN - 1; i++)
				{
					for (int j = i + 1; j < itemMemberN; j++)
					{
						if (!itemMember[i].isNULL() && condition(*itemMember[i], *itemMember[j], *this))
						{
							f(*itemMember[i], *itemMember[j]);
						}
					}
				}

				if (!isOnlyItem)
				{
					for (int i = 0; i < groupMemberN; i++)
					{
						if (!groupMember[i].isNULL())
						{
							groupMember[i]->Do(f, condition, isOnlyItem);
						}
					}
				}
			}

			bool addItem(std::vector< wiz::WizSmartPtr< Item<T> > >& items)
			{
				for (int i = 0; i < items.size(); ++i)
				{
					addItem(items[i]);
				}
				return true;
			}

			bool addItem(wiz::WizSmartPtr< Item<T> >& item)
			{
				{
					itemMember.push_back(item);

					itemMemberN++;

					// sort O(n) + O(n) !
					{
						renewalItem();

						int start = 0; int end = itemMemberN - 1;

						for (int pick = start + 1; pick <= end; pick++)
						{
							int current = pick;
							wiz::WizSmartPtr< Item<T> > temp = itemMember[pick]; //
							for (; (current > 0) && (itemMember[current - 1]->getName() > temp->getName()); current--)
							{
								itemMember[current] = std::move(itemMember[current - 1]);
							}
							itemMember[current] = temp;
						}
					}
				}

				return true;
			}

			bool addGroup(std::vector< wiz::WizSmartPtr< Group<T> > > & groups)
			{
				for (int i = 0; i < groups.size(); ++i)
				{
					addGroup(groups[i]);
				}
				return true;
			}
			bool addGroup(wiz::WizSmartPtr< Group<T> > & group) // empty group!
			{
				groupMember.push_back(group);

				groupMemberN++;

				// sort
				{
					renewalGroup();

					int start = 0; int end = groupMemberN - 1;

					for (int pick = start + 1; pick <= end; pick++)
					{
						int current = pick;
						wiz::WizSmartPtr< Group<T> > temp = groupMember[pick]; //
						for (; (current > 0) && (groupMember[current - 1]->getName() > temp->getName()); current--)
						{
							groupMember[current] = std::move(groupMember[current - 1]);
						}
						groupMember[current] = temp;
					}
				}

				return true;
			}

			// bool getItemValue( const std::string itemName ); // todo : + condition?
			bool setItemValue(const std::string & itemName, const T & value) //, int option?, Condition?
			{
				bool isSuccess = isItemExist(itemName);

				if (isSuccess) /// chk!!
				{
					int index = searchItem(itemName);
					itemMember[index]->setValue(value);
				}

				return isSuccess;
			}
			bool getGroup(const std::string & groupName, wiz::WizSmartPtr< Group<T> > & ptr)
			{
				bool isSuccess = isGroupExist(groupName);

				if (isSuccess) /// chk!!
				{
					int index = searchGroup(groupName);
					(ptr) = groupMember[index];
				}

				return isSuccess;
			}
			bool removeItem(const std::string & itemName, const bool bremove) // bremove - remove? delete?
			{
				bool isSuccess = isItemExist(itemName);

				if (isSuccess)
				{
					int index = -1;

					while (-1 != (index = searchItem(itemName)))
					{				// cf) expand <> shrink?
						itemMember[index].remove(bremove);

						renewalItem(index);

						itemMember.pop_back();
					}
				}

				return isSuccess;
			}

			bool removeGroup(const std::string & groupName, const bool bremove)
			{
				bool isSuccess = isGroupExist(groupName);

				if (isSuccess)
				{
					int index = searchGroup(groupName);

					// cf) expand <> shrink?

					groupMember[index].remove(bremove);

					renewalGroup(index);

					groupMember.pop_back();
				}

				return isSuccess;
			}
		private:
			void renewalItem(const int start = 0) /// isNeedRenewal? <- make?? todo!!
			{
				int count = start;
				for (int i = start; i < itemMember.size(); i++)
				{
					if (!itemMember[i].isNULL())
					{
						itemMember[count] = std::move(itemMember[i]);
						count++;
					}
				}
				// null initial!
				for (int i = count; i < itemMemberN; i++)
				{
					wiz::WizSmartPtr< Item<T> > temp(NULL);
					itemMember[i] = temp;
				}

				itemMemberN = count;

			}
			void renewalGroup(const int start = 0)
			{
				int count = start;
				for (int i = start; i < groupMember.size(); i++)
				{
					if (!groupMember[i].isNULL())
					{
						groupMember[count] = std::move(groupMember[i]);
						count++;
					}
				}
				// null initial!
				for (int i = count; i < groupMemberN; i++)
				{
					wiz::WizSmartPtr< Group<T> > temp(NULL);
					groupMember[i] = temp; /// bug fixed!! 2012.5.2 7.43PM - why?
				}

				groupMemberN = count;

			}


			int searchItem(const std::string & itemName) // return index! if index == -1 -> not exist
			{
				int index = -1;
				int left = 0; int right = itemMemberN - 1;
				int middle; std::string middleKey;

				while (left <= right)
				{
					middle = (left + right) >> 1; // more fast than ( left + right )/2!
					if (!(itemMember[middle].isNULL()))
					{
						middleKey = itemMember[middle]->getName();
						if (itemName == middleKey)
						{
							index = middle; break;
						}
						else if (itemName < middleKey)
						{
							right = middle - 1;
						}
						else
						{
							left = middle + 1;
						}
					}
					else
					{
						renewalItem();
						left = 0; right = itemMemberN - 1; // restart!
					}
				}

				return index;
			}
			int searchGroup(const std::string& groupName)
			{
				int index = -1;
				int left = 0; int right = groupMemberN - 1;
				int middle; std::string middleKey;

				while (left <= right)
				{
					middle = (left + right) >> 1; // more fast than ( left + right )/2!
					if (!groupMember[middle].isNULL())
					{
						middleKey = groupMember[middle]->getName();
						if (groupName == middleKey)
						{
							index = middle; break;
						}
						else if (groupName < middleKey)
						{
							right = middle - 1;
						}
						else
						{
							left = middle + 1;
						}
					}
					else
					{
						renewalGroup();
						left = 0; right = groupMemberN - 1; /// -1����! index�̹Ƿ� -1�� �ʿ���!
					}
				}

				return index;
			}

		};

		template <class T>
		class GroupManager /// has functionObject table.
		{
		public:
			/// typename or class??
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupBegin() { return rootGroup->groupBegin(); }
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupEnd() { return rootGroup->groupEnd(); }

			typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemBegin(const std::string& groupName)
			{
				wiz::WizSmartPtr< Group<T> > temp;
				if (rootGroup->getGroup(groupName, temp)) { if (temp->hasNoItem()) { return temp->itemEnd(); } else { return temp->itemBegin(); } }
				throw wiz::Error(__FILE__, __LINE__, "groupItemBegin err");
			}
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupBegin(const std::string& groupName)
			{
				wiz::WizSmartPtr< Group<T> > temp;
				if (rootGroup->getGroup(groupName, temp)) { if (temp->hasNoGroup()) { return temp->groupEnd(); } else { return temp->groupBegin(); } }
				throw wiz::Error(__FILE__, __LINE__, "groupGroupBegin err");
			}

			typename std::vector< wiz::WizSmartPtr< Item<T> > >::iterator groupItemEnd(const std::string& groupName)
			{
				wiz::WizSmartPtr< Group<T> > temp;
				if (rootGroup->getGroup(groupName, temp)) { return temp->itemEnd(); }
				throw wiz::Error(__FILE__, __LINE__, "groupItemEnd err");
			}
			typename std::vector< wiz::WizSmartPtr< Group<T> > >::iterator groupGroupEnd(const std::string& groupName)
			{
				wiz::WizSmartPtr< Group<T> > temp;
				if (rootGroup->getGroup(groupName, temp)) { return temp->groupEnd(); }
				throw wiz::Error(__FILE__, __LINE__, "groupGroupEnd err");
			}

		private:
			wiz::WizSmartPtr< Group<T> > rootGroup;
			std::string coreName;

		public:
			GroupManager(const std::string& coreName)
				: coreName(coreName)
			{
				wiz::WizSmartPtr< Group<T> > temp(new Group<T>(coreName, 1, 1)); // 1 or 0?
				rootGroup = temp;
			}
			virtual ~GroupManager() { rootGroup.remove(true); } //rootGroup.remove(); }
		public:
			std::string GetCoreName()const { return rootGroup->getName(); }

			bool IsItemExist(const std::string& itemName)
			{
				return rootGroup->isItemExist(itemName);
			}
			bool IsItemExist(const std::string& groupName, const std::string& place)
			{
				wiz::WizSmartPtr<Group<T>> x;
				if (rootGroup->getGroup(place, x)) {
					return x->isItemExist(groupName);
				}
				else {
					return false;
				}
			}
			bool IsGroupExist(const std::string& groupName)
			{
				return rootGroup->isGroupExist(groupName);
			}
			bool IsGroupExist(const std::string& groupName, const std::string& place)
			{
				wiz::WizSmartPtr<Group<T>> x;
				if (rootGroup->getGroup(place, x)) {
					return x->isGroupExist(groupName);
				}
				else {
					return false;
				}
			}

			bool NewItem(const std::string& itemName, const T& value)
			{
				wiz::WizSmartPtr< Item<T> > itemTemp(new Item<T>(itemName, value));

				/// add item to group
				if (rootGroup->addItem(itemTemp))
				{
					return true;
				}
				else
				{
					itemTemp.remove();
					return false;
				}
			}

			bool NewGroup(const std::string& groupName, const int itemMemberMax, const int groupMemberMax)
			{
				wiz::WizSmartPtr< Group<T> > temp(new Group<T>(groupName, itemMemberMax, groupMemberMax));

				if (coreName != groupName && rootGroup->addGroup(temp))
				{
					return true;
				}
				else
				{
					temp.remove();
					return false;
				}
			}
			bool AddItem(const std::string& itemName, wiz::WizSmartPtr< T >& ptr)
			{
				wiz::WizSmartPtr< Item<T> > itemTemp(new Item<T>(itemName, ptr));

				/// add item to group
				if (rootGroup->addItem(itemTemp))
				{
					return true;
				}
				else
				{
					itemTemp.remove();
					return false;
				}
			}
			bool AddItemToGroup(const std::string& itemName, const std::string& groupName)
			{
				/// find Group - name is groupName
				wiz::WizSmartPtr< Group<T> > groupTemp;
				wiz::WizSmartPtr< Item<T> > itemTemp;
				// get Group Ptr
				if (rootGroup->getGroup(groupName, groupTemp)
					&& rootGroup->getItem(itemName, itemTemp))
				{
					//for (int i = 0; i < groupTemp.size(); ++i) {
					groupTemp->addItem(itemTemp);
					//}
					return true;
				}
				else
				{
					return false;
				}
			}
			bool AddGroupToGroup(const std::string& groupName1, const std::string& groupName2) // from 1, to 2
			{
				/// find Group - name is groupName
				wiz::WizSmartPtr< Group<T> > groupTemp1;
				wiz::WizSmartPtr< Group<T> > groupTemp2;
				// get Group Ptr
				if (rootGroup->getGroup(groupName1, groupTemp1)
					&& rootGroup->getGroup(groupName2, groupTemp2))
				{
					return groupTemp2->addGroup(groupTemp1);
				}
				else
				{
					return false;
				}
			}
			bool GetItem(const std::string& itemName, Item<T>& itemPtr)
			{
				wiz::WizSmartPtr< Item<T> > ptr;

				if (rootGroup->getItem(itemName, ptr))
				{
					itemPtr = *ptr;
					return true;
				}
				else
				{
					return false;
				}
			}
			bool GetValue(const std::string& itemName, T& value)
			{
				Item<T> itemPtr;
				if (GetItem(itemName, itemPtr))
				{
					value = itemPtr.getValue();
					return true;
				}
				else
				{
					return false;
				}
			}
			bool GetGroup(const std::string& groupName, wiz::WizSmartPtr<Group<T>>& value)
			{
				return rootGroup->getGroup(groupName, value);
			}

			bool SetValue(const std::string& itemName, const T value)
			{
				return rootGroup->setItemValue(itemName, value);
			}

			bool RemoveItem(const std::string& itemName) // must true!
			{
				return rootGroup->removeItem(itemName, true);
			}
			bool RemoveGroup(const std::string& groupName) // must true!
			{
				return rootGroup->removeGroup(groupName, true);
			}
			bool RemoveItemFromGroup(const std::string& itemName, const std::string& groupName, const bool bRemove = false) // maybe false
			{
				/// find Group - name is groupName
				wiz::WizSmartPtr< Group<T> > groupTemp;
				// get Group Ptr
				if (rootGroup->getGroup(groupName, groupTemp))
				{
					return groupTemp->removeItem(itemName, bRemove);
				}
				else
				{
					return false;
				}
			}
			bool RemoveGroupFromGroup(const std::string& groupName1, const std::string& groupName2, const bool bRemove = false) // maybe false
			{
				/// find Group - name is groupName
				wiz::WizSmartPtr< Group<T> > groupTemp;
				// get Group Ptr
				if (rootGroup->getGroup(groupName2, groupTemp))
				{
					return groupTemp->removeGroup(groupName1, bRemove);
				}
				else
				{
					return false;
				}
			}

			/// int functionNo ??
			// functionObject table??
			//
			bool Do2(const std::string& groupName, Func<T>& f, const Condition<T>& condition) // int or enum! ( ALL, RANDOM- 00.00% )
			{
				wiz::WizSmartPtr< Group<T> > ptr;

				if (rootGroup->getGroup(groupName, ptr))
				{
					ptr->Do(f, condition, false);
					return true;
				}
				else
				{
					return false;
				}
			}


			bool Do(const std::string& groupName, FuncWith<T>& f, const ConditionWith<T>& condition)
			{
				wiz::WizSmartPtr< Group<T> > ptr;

				if (rootGroup->getGroup(groupName, ptr))
				{
					ptr->Do(f, condition, false);
					return true;
				}
				else
				{
					return false;
				}
			}

			void DoAll(Func<T>& f, const Condition<T>& condition)//DoAll( ~, groupName ); 
			{
				rootGroup->Do(f, condition, true);
			}
			void DoAll(FuncWith<T>& f, const ConditionWith<T>& condition)
			{
				rootGroup->Do(f, condition, true);
			}
		};

	}

}

#endif