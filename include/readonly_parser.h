
#ifndef READONLY_PARSER_H
#define READONLY_PARSER_H

//#define _CRT_SECURE_NO_WARNINGS // in main?

#define VECTOR std::vector
#define ARRAY_QUEUE std::deque


#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cstring>
#include <ctime>

#include <list>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>
#include <type_traits> /// for is_class, 
#include <regex>

#include <thread>

#include <variant>  // C++17 Standard.
///
/// GLOBAL_DEBUG
///

namespace wiz2 {
	// global.h
	template <class T>
	class ASC {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 < t2; }
	};
	template <class T>
	class DSC // DESC
	{
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 > t2; }
	};
	template <class T>
	class EQ { // EQ -> EQ!
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 == t2; }
	};
	/// TO DO
	/// ASC_EQ, DSC_EQ, NOT_EQ, EQ_SAME_VALUE, NOT_EQ_SAME_VALUE , chk red-black tree!!
	template <class T>
	class ASC_EQ {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 <= t2; }
	};
	template <class T>
	class DSC_EQ {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 >= t2; }
	};
	template <class T>
	class NOT_EQ {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1 != t2; }
	};
	template <class T>
	class EQ_SAME_VALUE {
	public:
		bool operator() (const T& t1, const T& t2) const { return t1.isSameValue(t2); }
	};
	template <class T>
	class NOT_EQ_SAME_VALUE { /// chk..
	public:
		bool operator() (const T& t1, const T& t2) const { return !t1.isSameValue(t2); }
	};


	template <class T> /// T is pointer type..
	class PASC {
	public:
		bool operator() (const T t1, const T t2) const { return *t1 < *t2; }
	};
	template <class T> /// T is pointer type..
	class PDSC // PDESC
	{
	public:
		bool operator() (const T t1, const T t2) const
		{
			return *t1 > * t2;
		}
	};
	template <class T> /// T is pointer type..
	class PEQ {
	public:
		bool operator() (const T t1, const T t2) const { return *t1 == *t2; }
	};

	template <typename T> /// x is 10ÏßÑÏàò..
	inline T pos_1(const T x, const int base = 10) // 1êÎ¶¨ Í∞Í≥ÑÏÇ∞
	{
		if (x >= 0) { return x % base; }// x - ( x / 10 ) * 10; }
		else { return (x / base) * base - x; }
		// -( x - ( (x/10) * 10 ) )
	}
	template <typename T> /// T <- char, int, long, long long...
	std::string toStr(const T x) /// chk!!
	{
		const int base = 10;
		if (base < 2 || base > 16) { return "base is not valid"; }
		T i = x;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 NULL, 1 minus
		std::string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE + 1] = '\0'; ///Î¨∏ÏûêúÏãú..

		for (k = INT_SIZE; k >= 1; k--) {
			T val = pos_1<T>(i, base); /// 0 ~ base-1
									   /// number to ['0'~'9'] or ['A'~'F']
			if (val < 10) { temp[k] = val + '0'; }
			else { temp[k] = val - 10 + 'A'; }

			i /= base;

			if (0 == i) { // ´Ïûê.
				k--;
				break;
			}
		}

		if (isMinus) {
			temp[k] = '-';
			tempString = std::string(temp + k);//
		}
		else {
			tempString = std::string(temp + k + 1); //
		}
		delete[] temp;

		return tempString;
	}

	/// chk.... need more thinking..., ToDo...
	template <typename T> /// T <- char, int, long, long long...
	std::string toStr2(const T x, const int str_space) /// chk!!
	{
		const int base = 10;
		if (base < 2 || base > 16) { return "base is not valid"; }
		T i = x;
		T k2 = 0;

		const int INT_SIZE = sizeof(T) << 3; ///*8
		char* temp = new char[INT_SIZE + 1 + 1]; /// 1 NULL, 1 minus
		for (int i = 0; i < INT_SIZE + 2; ++i) { temp[i] = '0'; }//
		std::string tempString;
		int k;
		bool isMinus = (i < 0);
		temp[INT_SIZE + 1] = '\0'; ///Î¨∏ÏûêúÏãú..

		for (k = INT_SIZE; k >= 1; k--) {
			T val = pos_1<T>(i, base); /// 0 ~ base-1
									   /// number to ['0'~'9'] or ['A'~'F']
			if (val < 10) { temp[k] = val + '0'; }
			else { temp[k] = val - 10 + 'A'; }

			i /= base;

			if (0 == i) { // ´Ïûê.
				k--;
				break;
			}
		}

		if (isMinus) {
			temp[k] = '-';
			tempString = std::string(temp + k);//
		}
		else {
			if (INT_SIZE + 1 - (k + 1) + 1 < str_space + 1)
			{
				k2 = str_space + 1 - (INT_SIZE + 1 - (k + 1) + 1);
			}
			else
			{
				k2 = 0;
			}
			tempString = std::string(temp + k + 1 - k2); //
		}
		delete[] temp;

		return tempString;
	}

	inline std::string str(const int x)
	{
		return toStr<int>(x);
	}

	template <typename T> /// ∏Ï∂ú†Îïå åÏïÑÏ≤¥ÌÅ¨úÎã§
	inline std::string _toString(const T x)
	{
		std::stringstream strs;
		strs << x;
		return strs.str();
	}
	template <>
	inline std::string _toString(const long double x)
	{
		std::stringstream strs;
		strs << std::fixed << x;
		std::string temp = strs.str();
		std::size_t idx = temp.find('.');
		if (idx == temp.size() - 1) {
			temp.push_back('0');
		}
		else if (idx == std::string::npos) {
			temp.push_back('.');
			temp.push_back('0');
		}
		return temp;
	}

	template <>
	inline std::string _toString(const long long x)
	{
		return toStr(x);
	}
	template <>
	inline std::string _toString(const int x)
	{
		return str(x);
	}
	template <>
	inline std::string _toString(const bool x)
	{
		if (x) { return "true"; }
		return "false";
	}

	// following remove
	inline bool isWhitespace(const char ch)
	{
		switch (ch)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
		case '\v':
		case '\f':
			return true;
			break;
		}
		return false;
	}

	// STRINGBUILDER.H
	class StringBuilder //
	{
	private:
		char* buffer_first;
		char* buffer;
		int len;
		int capacity;
	private:
		char* end() const
		{
			return buffer_first + capacity;
		}
	public:
		StringBuilder(const int buffer_size, const char* cstr = "", int len = 0) {
			buffer = (char*)malloc(sizeof(char) * (buffer_size + 1)); // 1 for '\0'
			this->len = len;
			capacity = buffer_size;
			memcpy(buffer, cstr, this->len);
			buffer[this->len] = '\0';
			buffer_first = buffer;
		}
		StringBuilder(const StringBuilder & other)
		{
			buffer = (char*)malloc(sizeof(char) * (other.capacity + 1)); // 1 for '\0'
			this->len = other.len;
			capacity = other.capacity;
			memcpy(buffer, other.buffer, other.capacity);
			buffer[this->len] = '\0';
			buffer_first = buffer;
		}

		StringBuilder & operator=(const StringBuilder & other)
		{
			if (buffer_first != nullptr) {
				free(buffer_first);
			}
			len = other.len;
			capacity = other.capacity;
			buffer_first = (char*)malloc(sizeof(char) * (len + 1));
			buffer = other.buffer;

			for (int i = 0; i <= len; ++i) {
				buffer_first[i] = other.buffer_first[i];
			}

			return *this;
		}
		StringBuilder& operator=(StringBuilder && other)
		{
			buffer_first = other.buffer_first;
			buffer = other.buffer;
			len = other.len;
			capacity = other.capacity;

			other.buffer_first = nullptr;
			other.buffer = nullptr;
			other.len = 0;

			return *this;
		}

		virtual ~StringBuilder()
		{
			free(buffer_first);
		}

		StringBuilder& AppendChar(const char ch)
		{
			char temp[2];
			temp[0] = ch; temp[1] = '\0';
			return Append(temp, 1);
		}

		StringBuilder& Append(const char* cstr, const int len)
		{
			if (buffer + this->len + len < end())
			{
				memcpy(buffer + this->len, cstr, len);
				buffer[this->len + len] = '\0';
				this->len = this->len + len;
			}
			else {
				if (buffer_first + this->len + len < end()) {
					memmove(buffer_first, buffer, this->len);
					memcpy(buffer_first + this->len, cstr, len);
					buffer_first[this->len + len] = '\0';
					buffer = buffer_first;
					this->len = this->len + len;
				}
				else {
					char* new_buffer = (char*)malloc(sizeof(char) * (2 * (this->len + len) + 1));
					memcpy(new_buffer, buffer, this->len);
					memcpy(new_buffer + this->len, cstr, len);
					new_buffer[this->len + len] = '\0';
					free(buffer_first);
					buffer = new_buffer;
					buffer_first = buffer;
					this->capacity = 2 * (this->len + len);
					this->len = this->len + len;
				}
			}
			return *this;
		}
		const char* Divide(const int idx) // need to rename!l, chk idx range!
		{
			buffer[idx] = '\0';
			return buffer;
		}
		const char* Str(int* size = nullptr) const {
			if (size) { *size = len; }
			return buffer;
		}
		char* Str(int* size = nullptr) {
			if (size) { *size = len; }
			return buffer;
		}
		void Clear()
		{
			len = 0;
			buffer = buffer_first;
			buffer[0] = '\0';
		}
		int Size() const { return len; }
		StringBuilder& LeftShift(const int offset = 1)
		{
			if (offset < 1) { return *this; }

			if (buffer + offset < end()) {
				buffer = buffer + offset;
			}
			else {
				memmove(buffer_first, buffer + offset, len - offset);
				buffer = buffer_first;
				buffer[len - offset] = '\0';
			}
			len = len - offset;
			return *this;
		}

		void SetLength(const int len)
		{
			this->len = len;
		}

		void SetBuffer(const int idx, const char val)
		{
			this->buffer[idx] = val;
		}

		char& operator[](const int idx)
		{
			return this->buffer[idx];
		}
		const char& operator[](const int idx) const
		{
			return this->buffer[idx];
		}
	};

	// cpp_string.h
	class String {
	public:
		static int find(const std::string& str, const std::string& dilim, const int start_index)
		{
			if (str.empty() || dilim.empty()) { return -1; }

			int k = 0;

			for (int i = start_index; i < str.size(); i++) {
				if (str[i] == dilim[k]) { k++; if (dilim.size() == k) { return i - dilim.size() + 1; } }
				else {
					k = 0;
					if (str[i] == dilim[k]) { k++; if (dilim.size() == k) { return i - dilim.size() + 1; } }
				}
			}

			return -1;
		}
		static bool Comp(const char* cstr1, const char* cstr2, const int n) /// isSameData
		{
			for (int i = 0; i < n; i++)
			{
				if (cstr1[i] != cstr2[i]) { return false; }
			}
			return true;
		}
		static bool Comp(const std::string& str1, const std::string& str2, const int n) /// isSameData
		{
			for (int i = 0; i < n; i++)
			{
				if (str1[i] != str2[i]) { return false; }
			}
			return true;
		}
		// fixed more fast!
		static std::string replace(const std::string& str, const std::string& str1, const std::string& str2)
		{
			if (str1.empty()) { return str; }

			std::string temp = str;

			for (std::string::size_type i = temp.size() - str1.size(); i >= 0;) {
				i = temp.rfind(str1.c_str(), i);

				if (i == std::string::npos) {
					break;
				}
				else {
					temp.replace(temp.begin() + i, temp.begin() + i + str1.size(), str2.c_str());
					--i;
				}
			}

			return temp;
		}
		static std::string remove(const std::string& str, const std::string& str1)
		{
			return replace(str, str1, std::string(""));
		}
		//start index
		static int indexOf(const std::string& str, const char ch)
		{
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == ch) { return i; }
			}
			return -1;
		}
		// chk..
		static std::pair< bool, size_t > indexOf(const std::string& str, const std::string& str1, const int fromIndex)
		{
			if (str1.empty()) { return{ false, 0 }; }
			auto idx = str.find(str1, fromIndex);
			if (idx == std::string::npos) { return{ false, 0 }; }
			return{ true, idx };
		}
		static auto indexOf(const std::string& str, const std::string& str1)
		{
			return indexOf(str, str1, 0);
		}
		/// chk....
		static std::vector<std::string> split(const std::string& str, const std::string& filter)
		{
			if (filter.empty()) {
				std::vector<std::string> result;
				if (!str.empty()) {
					result.push_back(str);
				}
				return result;
			}
			/// example...
			/// www.naver.com -> www, naver, com
			/// .www.naver.com. -> www, naver, com!!
			const char* pStr = str.c_str();
			char* pTemp = new char[str.size() + 1];
			std::vector<std::string> result;

			strcpy(pTemp, str.c_str());

			for (int i = 0; i <= str.size() - filter.size(); i++) {
				if (strlen(pStr + i) >= filter.size()
					&& Comp(pStr + i, filter.c_str(), filter.size()))
				{ // finded..
					pTemp[i] = '\0';
					for (int j = i + 1; j < i + filter.size(); j++)
					{
						pTemp[j] = '\0';
					}
					i = i + filter.size() - 1;
				}
			}
			for (int i = 0; i < str.size(); i++)
			{
				if (pTemp[i] != '\0')
				{
					result.push_back(std::string(pTemp + i));
					i = i + strlen(pTemp + i) - 1;
				}
			}

			// free
			delete[] pTemp;
			return result;
		}
		static std::string substring(const std::string & str, const int start, const int last)
		{
			return str.substr(start, last - start + 1);
		}
		static std::string substring(const std::string & str, const int start)
		{
			return substring(str, start, str.size() - 1);
		}
		static bool startsWith(const std::string & str, const std::string & start)
		{
			if (str.size() < start.size()) { return false; }
			return Comp(str.data(), start.c_str(), start.size());
		}
		static bool endsWith(const std::string & str, const std::string & last)
		{
			if (str.size() < last.size()) { return false; }
			if (last.empty()) { return true; } /// chk... return false; 
			return Comp(str.data() + (str.size() - last.size()), last.c_str(), last.size());
		}
	};

	class StringTokenizer
	{
	private:
		std::vector<std::string> _m_str;
		int count;
		bool exist;
		inline static const std::vector<std::string> whitespaces = { " ", "\t", "\r", "\n" };
		int option;
	private:
		void Init(const std::string& str, const std::vector<std::string>& separator, StringBuilder* builder) // assumtion : separators are sorted by length?, long -> short
		{
			const int sep_size = separator.size();
			const int str_size = str.size();
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
																//
			builder->Clear();
			builder->Append(str.c_str(), str.size());

			_m_str.reserve(str.size() / 2);
			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;


				if (1 == option && 0 == state && '\'' == str[i]) {
					if (i == 0) {
						state = 2;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init 1 on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 2;
						continue;
					}
				}
				else if (1 == option && 2 == state && '\'' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
						if (i == str.size() - 1) {
							if (right - left + 1 > 0) {
								_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
								builder->LeftShift(right - left + 2);
							}
						}
						continue;
					}
					continue;
				}
				else if (1 == option && 2 == state) {
					continue;
				}
				else if (1 == option && 0 == state && '\"' == str[i]) {
					if (i == 0) {
						state = 1;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init 2 on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 1;
						continue;
					}
				}
				else if (1 == option && 1 == state && '\"' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
						if (i == str.size() - 1) {
							if (right - left + 1 > 0) {
								_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
								builder->LeftShift(right - left + 2);
							}
						}
						continue;
					}
				}
				else if (1 == option && 1 == state) {
					continue;
				}


				for (int j = 0; j < sep_size; ++j) {
					const int sep_j_size = separator[j].size();
					for (int k = 0; k < sep_j_size && i + k < str_size; ++k) {
						if (str[i + k] == separator[j][k]) {
							pass = true;
						}
						else {
							pass = false;
							break;
						}
					}
					if (pass) { _select = j; break; }
				}

				if (pass) {
					this->exist = true;

					if (right - left >= 0) {
						const char* temp = builder->Divide(right - left);
						if (right - left > 0) {
							_m_str.emplace_back(temp, right - left);
							builder->LeftShift(right - left + 1);
						}
						else {
							builder->LeftShift(1);
						}
					}

					i = i + separator[_select].size() - 1;

					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
						builder->LeftShift(right - left + 2);
					}
					else {
						//GETCH();
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
		void Init(std::string && str, const std::vector<std::string> & separator, StringBuilder * builder) // assumtion : separators are sorted by length?, long -> short
		{
			const int sep_size = separator.size();
			const int str_size = str.size();
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
																//
			builder->Clear();
			builder->Append(str.c_str(), str.size());

			_m_str.reserve(str.size() / 2);
			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;


				if (1 == option && 0 == state && '\'' == str[i]) {
					state = 2;
					continue;
				}
				else if (1 == option && 2 == state && '\'' == str[i]) {
					state = 0;
					if (i == str.size() - 1) {
						if (right - left + 1 > 0) {
							_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
							builder->LeftShift(right - left + 2);
						}
					}
					continue;
				}
				else if (1 == option && 2 == state) {
					continue;
				}
				else if (1 == option && 0 == state && '\"' == str[i]) {
					if (i == 0) {
						state = 1;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 1;
						continue;
					}
				}
				else if (1 == option && 1 == state && '\"' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
						if (i == str.size() - 1) {
							if (right - left + 1 > 0) {
								_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
								builder->LeftShift(right - left + 2);
							}
						}
						continue;
					}
				}
				else if (1 == option && 1 == state) {
					continue;
				}


				for (int j = 0; j < sep_size; ++j) {
					const int sep_j_size = separator[j].size();
					for (int k = 0; k < sep_j_size && i + k < str_size; ++k) {
						if (str[i + k] == separator[j][k]) {
							pass = true;
						}
						else {
							pass = false;
							break;
						}
					}
					if (pass) { _select = j; break; }
				}

				if (pass) {
					this->exist = true;

					if (right - left >= 0) {
						const char* temp = builder->Divide(right - left);
						if (right - left > 0) {
							_m_str.emplace_back(temp, right - left);
							builder->LeftShift(right - left + 1);
						}
						else {
							builder->LeftShift(1);
						}
					}

					i = i + separator[_select].size() - 1;

					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
						builder->LeftShift(right - left + 2);
					}
					else {
						//GETCH();
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
	public:
		explicit StringTokenizer() : count(0), exist(false), option(0) { }
		explicit StringTokenizer(const std::string & str, const std::string & separator, StringBuilder * builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			std::vector<std::string> vec; vec.push_back(separator);
			Init(str, vec, builder);
		}
		explicit StringTokenizer(std::string && str, const std::string & separator, StringBuilder * builder, int option = 0)
		{
			std::vector<std::string> vec; vec.push_back(separator);
			Init(std::move(str), vec, builder);
		}
		explicit StringTokenizer(const std::string & str, const std::vector<std::string> & separator, StringBuilder * builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, separator, builder);
		}
		explicit StringTokenizer(std::string && str, const std::vector<std::string> & separator, StringBuilder * builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), separator, builder);
		}

		explicit StringTokenizer(const std::string & str, StringBuilder * builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, whitespaces, builder);
		}
		explicit StringTokenizer(std::string && str, StringBuilder * builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), whitespaces, builder);
		}
		int countTokens()const
		{
			return _m_str.size();
		}
		std::string nextToken()
		{
			if (hasMoreTokens())
				return _m_str[count++];
			else
				throw "error in nextToken!";
		}
		bool hasMoreTokens()const
		{
			return count < _m_str.size();
		}

		bool isFindExist()const
		{
			return exist;
		}

	};

	// load_data.h
	class Token2
	{
	public:
		char* str = nullptr;
		int len = 0;
		bool isComment = false;
	public:
		Token2(char* str, int len, bool isComment) :
			str(str), len(len), isComment(isComment) { }

		Token2() { }

		void clear()
		{
			str = nullptr;
			len = 0;
			isComment = false;
		}
		bool operator==(const char* cstr) const {
			int cstr_len = strlen(cstr);

			if (len == cstr_len) {
				for (int i = 0; i < len; ++i) {
					if (str[i] != cstr[i]) {
						return false;
					}
				}
				return true;
			}
			return false;
		}
	};

	class Token
	{
	public:
		std::string str; // cf) && ?
		bool isComment;
	public:
		Token& operator=(const Token& token) {
			str = token.str;
			isComment = token.isComment;
			return *this;
		}
		void operator=(Token&& token) {
			str = std::move(token.str);
			isComment = token.isComment;
		}
		virtual ~Token() {

		}
		Token(Token&& token) : str(std::move(token.str)), isComment(token.isComment) { }
		Token(const Token& token) : str(token.str), isComment(token.isComment) { }
		explicit Token() : isComment(false) { }
		explicit Token(std::string&& str, bool isComment = false) : str(std::move(str)), isComment(isComment) { }
		explicit Token(const std::string& str, bool isComment = false) : str(str), isComment(isComment) { }
	};

	class LoadDataOption
	{
	public:
		std::vector<std::string> LineComment;	// # 
		std::vector<std::string> MuitipleLineCommentStart; // ###  // ?
		std::vector<std::string> MuitipleLineCommentEnd;   // ### // ?
		std::vector<char> Left, Right;	// { } , [ ] <- json
		std::vector<char> Assignment;	// = , :
		std::vector<char> Removal;		// ',', empty. 
	};

	inline int Equal(const std::vector<char> & option, const char ch)
	{
		for (int i = 0; i < option.size(); ++i) {
			if (ch == option[i]) {
				return i;
			}
		}
		return -1;
	}

	// load_data_utility.h
	class Utility {
	public:
		class DoThread // need to rename!
		{
		private:
			StringBuilder* strVec;
		public:
			ARRAY_QUEUE<Token>* aq;
			const wiz2::LoadDataOption* option;
			//int strVecStart;
			//int strVecEnd;
		public:
			DoThread(StringBuilder* strVec, ARRAY_QUEUE<Token>* aq, const wiz2::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
				: strVec(strVec), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
			{
				//
			}
		private:
			// slow?
			int checkDelimiter(const StringBuilder& statement, const int start, const std::vector<std::string>& delimiter)
			{
				int sum = 0;
				const int _size = delimiter.size();

				for (int delim_num = 0; delim_num < _size; ++delim_num) {
					// size check
					if (start + delimiter[delim_num].size() - 1 > statement.Size() - 1) {
						continue;
					}

					const int last = start + delimiter[delim_num].size() - 1;
					for (int i = start; last >= start && i <= last; ++i) {
						if (statement[i] == delimiter[delim_num][i - start]) {

						}
						else {
							sum--;
							break;
						}
					}
					sum++;

					if (sum > 0) {
						return delim_num;
					}
				}

				return -1;
			}
		public:
			void operator() () {
				//std::string* strVecTemp = strVec; // enterkey ±‚¡ÿ¿∏∑Œ ≥™¥µæÓ¡Æ¿÷¥Ÿ∞Ì ∞°¡§«—¥Ÿ.
				//for (int x = 0; x <= 0; ++x)
				{
					//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
					//while (tokenizer.hasMoreTokens()) {
					//	aq.push(tokenizer.nextToken());
					//}
					StringBuilder& statement = *strVec;
					int token_first = 0, token_last = 0; // idx of token in statement.
					int state = 0;
					bool isMultipleLineComment = false;
					std::string token;


					for (int i = 0; i < statement.Size(); ++i) {
						int idx;

						if (isMultipleLineComment && -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentEnd)))
						{
							isMultipleLineComment = false;

							for (int j = 0; j < option->MuitipleLineCommentEnd[idx].size(); ++j)
							{
								token.push_back(statement[i + j]);
							}

							aq->emplace_back(std::move(token), true);

							i = i + option->MuitipleLineCommentEnd[idx].size() - 1;

							statement.Divide(i);
							statement.LeftShift(i + 1);

							token.clear();

							token_first = 0;
							token_last = 0;

							i = -1;
						}
						else if (isMultipleLineComment) {
							token_last = i;

							token.push_back(statement[i]);
						}
						else if (0 == state && '\'' == statement[i]) {
							//token_last = i - 1;
							//if (token_last >= 0 && token_last - token_first + 1 > 0) {
							//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
							//}
							state = 2;
							//token_first = i; 
							token_last = i;

							token.push_back(statement[i]);
						}
						else if (2 == state && '\\' == statement[i - 1] && '\'' == statement[i]) {
							token_last = i;
							token.push_back(statement[i]);
						}
						else if (2 == state && '\'' == statement[i]) {
							state = 0; token_last = i;
							token.push_back(statement[i]);
						}
						else if (0 == state && '\"' == statement[i]) {
							//token_last = i - 1;
							//if (token_last >= 0 && token_last - token_first + 1 > 0) {
							//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
							//}
							state = 1;
							//token_first = i; 
							token_last = i;
							token.push_back(statement[i]);
						}
						else if (1 == state && '\\' == statement[i - 1] && '\"' == statement[i]) {
							token_last = i;
							token.push_back(statement[i]);
						}
						else if (1 == state && '\"' == statement[i]) {
							state = 0; token_last = i;
							token.push_back(statement[i]);
						}
						else if (0 == state && -1 != (idx = Equal(option->Removal, statement[i])))
						{
							token_last = i - 1;

							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								statement.Divide(i);

								aq->emplace_back(std::move(token), false);
								token.clear();

								statement.LeftShift(i + 1);

								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else {
								statement.LeftShift(1);
								token.clear();
								token_first = 0;
								token_last = 0;
								i = -1;
							}
							continue;
						}
						else if (0 == state && -1 != (idx = Equal(option->Assignment, statement[i]))) {
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {

								statement.Divide(i);

								aq->emplace_back(std::move(token), false);

								token.clear();

								statement.LeftShift(i + 1);

								aq->emplace_back(std::string("") + option->Assignment[idx], false);
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else {
								aq->emplace_back(std::string("") + option->Assignment[idx], false);
								statement.LeftShift(1);

								token.clear();
								token_first = 0;
								token_last = 0;
								i = -1;
							}
						}
						else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								statement.Divide(i);

								aq->emplace_back(std::move(token), false);

								statement.LeftShift(i + 1);
								token.clear();
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else
							{
								statement.LeftShift(1);
								token.clear();
								token_first = 0;
								token_last = 0;
								i = -1;
							}
						}
						else if (0 == state && -1 != (idx = Equal(option->Left, statement[i]))) {
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								statement.Divide(i);

								aq->emplace_back(std::move(token), false);
								statement.LeftShift(i + 1);

								aq->emplace_back(std::string("") + option->Left[idx], false);
								token.clear();
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else {
								aq->emplace_back(std::string("") + option->Left[idx], false);
								statement.LeftShift(1);
								token.clear();
								token_first = 0;
								token_last = 0;
								i = -1;
							}
						}
						else if (0 == state && -1 != (idx = Equal(option->Right, statement[i]))) {
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								statement.Divide(i);


								aq->emplace_back(std::move(token), false);
								statement.LeftShift(i + 1);

								aq->emplace_back(std::string("") + option->Right[idx], false);

								token.clear();
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else {
								aq->emplace_back(std::string("") + option->Right[idx], false);

								statement.LeftShift(1);

								token.clear();
								token_first = 0;
								token_last = 0;
								i = -1;
							}
						}
						else if (0 == state && option->MuitipleLineCommentStart.empty() == false
							&& -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentStart))) { // different from load_data_from_file
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								statement.Divide(i);
								aq->emplace_back(std::move(token), false);

								statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
								i = -1;

								token_first = 0;
								token_last = 0;
							}
							else {
								statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
								i = -1;
							}

							token = option->MuitipleLineCommentStart[idx];

							isMultipleLineComment = true;
						}
						else if (0 == state && option->LineComment.empty() == false &&
							-1 != checkDelimiter(statement, i, option->LineComment)) { // different from load_data_from_file
							token_last = i - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								char temp = statement[i];

								statement.Divide(i);
								aq->emplace_back(std::move(token), false);
								token.clear();

								statement[i] = temp;
								statement.LeftShift(i);
								i = 0;
							}
							int j = 0;
							for (j = i; j < statement.Size(); ++j) {
								if (statement[j] == '\n') // cf) '\r' ? '\0'?
								{
									break;
								}
							}
							--j; // "before enter key" or "before end"

							if (j - i + 1 > 0) {
								statement.Divide(j + 1);

								aq->emplace_back(std::string(statement.Str(), j - i + 1), true);
								statement.LeftShift(j + 2);

								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else {
								statement.LeftShift(j + 2);
								token_first = 0;
								token_last = 0;

								i = -1;
							}
						}
						else {
							token.push_back(statement[i]);
						}
					}

					if (token.empty() == false)
					{
						aq->emplace_back(std::move(token), false);
					}
				}
			}
			//
			//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
			//{
			//
			//}
			//void join(DoThread& other) 
			//{
			//	aqpush(std::move(other.aq));
			//}
		};

		class DoThread3 // need to rename!
		{
		private:
			char* start;
			char* last;
		public:
			VECTOR<Token2>* aq;
			const wiz2::LoadDataOption* option;
			//int strVecStart;
			//int strVecEnd;
		public:
			DoThread3(char* start, char* last, VECTOR<Token2>* aq, const wiz2::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
				: start(start), last(last), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
			{
				//
			}
		private:
			int checkDelimiter(const char* start, const char* last, const std::vector<std::string>& delimiter)
			{

				int sum = 0;
				for (int delim_num = 0; delim_num < delimiter.size(); ++delim_num) {
					// size check
					if (start + delimiter[delim_num].size() - 1 > last) {
						continue;
					}

					for (const char* x = start; x <= start + delimiter[delim_num].size() - 1; ++x) {
						if (*x == delimiter[delim_num][x - start]) {

						}
						else {
							sum--;
							break;
						}
					}
					sum++;

					if (sum > 0) {
						return delim_num;
					}
				}

				return -1;
			}
		public:
			void operator() () {
				{
					char* token_first = start;
					char* token_last = start; // idx of token in statement.
					int state = 0;
					std::string token;

					for (char* x = start; x < last; ++x) {
						int offset = 0;
						int idx;

						if (0 == state && '\'' == *x) {
							//token_last = x - 1;
							//if (token_last >= 0 && token_last - token_first + 1 > 0) {
							//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
							//}
							state = 2;
							//token_first = i; 
							token_last = x;

							token.push_back(*x);
						}
						else if (2 == state && '\\' == *(x - 1) && '\'' == *x) {
							token_last = x;
							token.push_back(*x);
						}
						else if (2 == state && '\'' == *x) {
							state = 0; token_last = x;
							token.push_back(*x);
						}
						else if (0 == state && '\"' == *x) {
							//token_last = x - 1;
							//if (token_last >= 0 && token_last - token_first + 1 > 0) {
							//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
							//}

							state = 1;
							//token_first = i; 
							token_last = x;
							token.push_back(*x);
						}
						else if (1 == state && '\\' == *(x - 1) && '\"' == *x) {
							token_last = x;
							token.push_back(*x);
						}
						else if (1 == state && '\"' == *x) {
							state = 0; token_last = x;
							token.push_back(*x);
						}
						else if (0 == state && -1 != (idx = Equal(option->Removal, *x)))
						{
							token_last = x - 1;

							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								aq->emplace_back(token_first, token_last - token_first + 1, false);
								token.clear();

								token_first = x + 1;
							}
							else {
								token.clear();

								token_first = token_first + 1;
							}
							continue;
						}
						else if (0 == state && -1 != (idx = Equal(option->Assignment, *x))) {
							token_last = x - 1;

							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								aq->emplace_back(token_first, token_last - token_first + 1, false);

								token.clear();

								token_first = x + 1;

								aq->emplace_back(x, 1, false);
							}
							else {
								aq->emplace_back(x, 1, false);
								token_first = token_first + 1;

								token.clear();
							}
						}
						else if (0 == state && isWhitespace(*x)) { // isspace ' ' \t \r \n , etc... ?
							token_last = x - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {

								aq->emplace_back(token_first, token_last - token_first + 1, false);

								token_first = x + 1;
								token.clear();
							}
							else
							{
								token_first = token_first + 1;
								token.clear();
							}
						}
						else if (0 == state && -1 != (idx = Equal(option->Left, *x))) {
							token_last = x - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {

								aq->emplace_back(token_first, token_last - token_first + 1, false);
								token_first = x + 1;

								aq->emplace_back(x, 1, false);
								token.clear();
							}
							else {
								aq->emplace_back(x, 1, false);
								token_first = token_first + 1;
								token.clear();
							}
						}
						else if (0 == state && -1 != (idx = Equal(option->Right, *x))) {
							token_last = x - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {

								aq->emplace_back(token_first, token_last - token_first + 1, false);
								token_first = x + 1;

								aq->emplace_back(x, 1, false);

								token.clear();
							}
							else {
								aq->emplace_back(x, 1, false);

								token_first = token_first + 1;

								token.clear();
							}
						}
						else if (0 == state && option->LineComment.empty() == false &&
							-1 != checkDelimiter(x, last, option->LineComment)) { // different from load_data_from_file
							token_last = x - 1;
							if (token_last >= 0 && token_last - token_first + 1 > 0) {
								aq->emplace_back(token_first, token_last - token_first + 1, false);
								token.clear();
								x = token_last + 1;
								token_first = token_last + 1;
								token_last = token_last + 1;
							}
							for (; x < last; ++x) {
								token_last++;
								if (*x == '\n' || *x == '\0') // cf) '\r' ? '\0'?
								{
									break;
								}
							}
							//aq->emplace_back(token_first, token_last - token_first + 1, true); // cancel?
							token_first = x + 1;
							offset = 1;
						}
						else {
							//
						}

						token_last = x + offset;
					}

					if (token_first < last)
					{
						aq->emplace_back(token_first, last - 1 - token_first + 1, false);
					}
				}
			}
		};
		// no enter strings? " abc \n def " and problem - one line!?
		static std::pair<bool, int> Reserve2_3(std::ifstream & inFile, VECTOR<Token2> * aq, const int num, bool* isFirst, const wiz2::LoadDataOption & option, int thr_num, char*& _buffer)
		{
			if (inFile.eof()) {
				return { false, 0 };
			}

			//int a = clock();

			//int count = 0;
			std::string temp;
			char* buffer = nullptr;// = new char[length + 1]; // 
			std::vector<long long> start(thr_num + 1, 0);
			std::vector<long long> last(thr_num + 1, 0);

			if (thr_num > 0) {
				inFile.seekg(0, inFile.end);
				const unsigned long long length = inFile.tellg();
				inFile.seekg(0, inFile.beg);

				buffer = new char[length + 1]; // 

											   // read data as a block:
				inFile.read(buffer, length);
				inFile.seekg(0, inFile.end);
				char temp;
				inFile >> temp;

				buffer[length] = '\0';

				start[0] = 0;


				// todo - linear check?
				for (int i = 1; i < thr_num; ++i) {
					start[i] = length / thr_num * i;
					for (int x = start[i]; x <= length; ++x) {
						// here bug is..  " check "
						if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
							start[i] = x;
							//	std::cout << "start " << start[i] << std::endl;
							break;
						}
					}
				}
				for (int i = 0; i < thr_num - 1; ++i) {
					last[i] = start[i + 1] - 1;
					for (int x = last[i]; x <= length; ++x) {
						if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
							last[i] = x;
							//	std::cout << "start " << start[i] << std::endl;
							break;
						}
					}
				}
				last[thr_num - 1] = length;
				//	std::cout << last[thr_num - 1] << std::endl;
			}
			else {
				inFile.seekg(0, inFile.end);
				const unsigned long long length = inFile.tellg();
				inFile.seekg(0, inFile.beg);

				buffer = new char[length + 1]; // 

											   // read data as a block:
				inFile.read(buffer, length);
				buffer[length] = '\0';

				inFile.seekg(0, inFile.end);
				char temp;
				inFile >> temp;

				start[0] = 0;
				last[0] = length;
			}

			//int a = clock();
			//debug

			if (thr_num > 0) {
				//// in string, there are '\r' or '\n' etc.. - no '\r' or '\n'?
				for (int i = thr_num - 1; i > 0; --i) {
					int last_enter_idx = -1;
					int first_enter_idx = -1;

					// find last_enter_idx, first_enter_idx -
					//// has bug - " abc
					////				def"
					for (int j = last[i - 1]; j <= last[i]; ++j) {
						if (buffer[j] == '\r' || buffer[j] == '\n' || j == last[thr_num - 1]) {
							last_enter_idx = j;
							break;
						}
					}

					{
						int state = 0;
						int j = last[i - 1] - 1;
						for (; j >= 0; --j) {
							if (state == 0 && (buffer[j] == '\r' || buffer[j] == '\n')) {
								first_enter_idx = j;
								break;
							}
							else if (j == start[i - 1]) {
								--i;
							}
						}

						if (-1 == first_enter_idx) {
							first_enter_idx = 0;
						}
					}

					// exchange with whitespace
					{
						int state = 0;
						int sharp_start = -1;

						for (int j = first_enter_idx + 1; j < last_enter_idx; ++j) {
							if (0 == state && buffer[j] == '\'') {
								state = 1;
							}
							else if (1 == state && buffer[j - 1] == '\\' && buffer[j] == '\'') {

							}
							else if (1 == state && buffer[j] == '\'') {
								state = 0;
							}
							else if (0 == state && buffer[j] == '\"') {
								state = 3;
							}
							else if (3 == state && buffer[j - 1] == '\\' && buffer[j] == '\"') {

							}
							else if (3 == state && buffer[j] == '\"') {
								state = 0;
							}
							else if (0 == state && buffer[j] == '#') {
								sharp_start = j;
							}
						}

						if (-1 != sharp_start) {
							for (int k = sharp_start; k < last_enter_idx; ++k) {
								buffer[k] = ' ';
							}
						}
					}
				}

				std::vector<VECTOR<Token2>> partial_list(thr_num, VECTOR<Token2>());
				std::vector<std::thread> thr(thr_num);

				for (int i = 0; i < thr_num; ++i) {
					//	std::cout << last[i] - start[i] << std::endl;
					partial_list[i].reserve((last[i] - start[i]) / 10);
					thr[i] = std::thread(DoThread3(buffer + start[i], buffer + last[i], &partial_list[i], &option));
				}

				for (int i = 0; i < thr_num; ++i) {
					thr[i].join();
				}

				int new_size = aq->size() + 2;
				for (int i = 0; i < thr_num; ++i) {
					new_size = new_size + partial_list[i].size();
				}

				aq->reserve(new_size);

				for (int i = 0; i < thr_num; ++i) {
					//for (int j = 0; j < partial_list[i].size(); ++j) {
					//	aq->push_back(std::move(partial_list[i][j]));
					//}
					aq->insert(aq->end(), make_move_iterator(partial_list[i].begin()), make_move_iterator(partial_list[i].end()));
				}
			}
			else {
				VECTOR<Token2> temp;

				temp.reserve((last[0] - start[0]) / 10);

				DoThread3 dothr(buffer + start[0], buffer + last[0], &temp, &option);

				dothr();

				aq->insert(aq->end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
			}

			//delete[] buffer;

			//				log_result = log_result + clock() - a;
			//std::cout << "lexing " << clock() - a << "ms" << std::endl;

			_buffer = buffer;

			return{ true, 1 };
		}
	};

	// file reserver
	class InFileReserver3
	{
	private:
		std::ifstream* pInFile;
		bool isFirst;
	public:
		int Num;
	public:
		explicit InFileReserver3(std::ifstream& inFile)
		{
			pInFile = &inFile;
			Num = 1;
			isFirst = true;
		}
		bool end()const { return pInFile->eof(); } //
	public:
		bool operator() (VECTOR<Token2>* strVec, const wiz2::LoadDataOption& option, int thr_num, char*& buffer)
		{
			bool x = Utility::Reserve2_3(*pInFile, strVec, Num, &isFirst, option, thr_num, buffer).second > 0;
			return x;
		}
	};
	class NoneReserver
	{
	private:
		int count;
	public:
		explicit NoneReserver() : count(0) { }
		bool operator() (ARRAY_QUEUE<Token>&, const wiz2::LoadDataOption&)
		{
			count = 1;
			return false;
		}
		bool operator() (VECTOR<Token2>*, const wiz2::LoadDataOption&, int, char*&)
		{
			count = 1;
			return false;
		}
		bool end()const { return 1 == count; }
	};

	// load_data_types.h
	namespace load_data {
		class Type {
		private:
			std::string name;

			void chk() {
				/*
				if ("_" == name || (String::startsWith(name, "$ut") && name.size() > 3) || (String::startsWith(name, "$it") && name.size() > 3)) {
				std::cout << "name is " << name << std::endl;
				std::cout << "in funciton chk() in Type" << std::endl;
				GETCH();
				throw "ERROR for name in Type";
				}
				*/
			}
		public:
			explicit Type(const std::string& name = "", const bool valid = true) : name(name) { }//chk();  }
			explicit Type(std::string&& name, const bool valid = true) : name(move(name)) { }//chk(); }
			Type(const Type& type)
				: name(type.name)
			{
				//chk();
			}
			virtual ~Type() { }
			bool IsFail() const { // change body
				return "" == name;
			}
			const std::string& GetName()const {
				return name;
			}
			void SetName(const std::string& name)
			{
				this->name = name;

				//chk();
			}
			void SetName(std::string&& name)
			{
				this->name = name;

				//chk();
			}
			bool operator==(const Type& t) const {
				return name == t.name;
			}
			bool operator<(const Type& t) const {
				return name < t.name;
			}
			Type& operator=(const Type& type)
			{
				name = type.name;
				return *this;
			}
			void operator=(Type&& type)
			{
				name = std::move(type.name);
			}
		};

		template < class T >
		class ItemType : public Type {
		public:
			typedef T item_type; //
		private:
			//std::vector<T> arr;
			T data;
			bool inited;
		public:
			ItemType(const ItemType<T>& ta) : Type(ta), data(ta.data), inited(ta.inited)
			{

			}
			ItemType(ItemType<T>&& ta) : Type(std::move(ta))
			{
				data = std::move(ta.data);
				inited = ta.inited;
			}
		public:
			explicit ItemType()
				: Type("", true), inited(false) { }
			explicit ItemType(const std::string& name, const T& value, const bool valid = true)
				:Type(name, valid), data(value), inited(true)
			{

			}
			explicit ItemType(std::string&& name, T&& value, const bool valid = true)
				:Type(move(name), valid), data(move(value)), inited(true)
			{

			}
			virtual ~ItemType() { }
		public:
			void Remove(const int idx = 0)
			{
				data = T();
				inited = false;
			}
			bool Push(const T& val) { /// do not change..!!
				if (inited) { throw "ItemType already inited"; }
				data = val;
				inited = true;

				return true;
			}
			bool Push(T&& val) {
				if (inited) { throw "ItemType already inited"; }
				data = std::move(val);
				inited = true;

				return true;
			}
			T& Get(const int index) {
				if (!inited) {
					throw "ItemType, not inited";
				}
				return data;
			}
			const T& Get(const int index) const {
				if (!inited) {
					throw "ItemType, not inited";
				}
				return data;
			}
			void Set(const int index, const T& val) {
				if (!inited) {
					throw "ItemType, not inited";
				} // removal?
				data = val;
			}
			void Set(const int index, T&& val) {
				if (!inited) {
					throw "ItemType, not inited";
				} // removal?
				data = std::move(val);
			}
			int size()const {
				return inited ? 1 : 0;
			}
			bool empty()const { return !inited; }
			std::string ToString()const
			{
				if (Type::GetName().empty()) {
					return Get(0);
				}
				return Type::GetName() + " = " + Get(0);
			}
		public:
			ItemType<T>& operator=(const ItemType<T>& ta)
			{
				Type::operator=(ta);
				ItemType<T> temp = ta;

				data = std::move(temp.data);
				inited = temp.inited;
				return *this;
			}
			ItemType<T>& operator=(ItemType<T>&& ta)
			{
				Type::operator=(ta);
				if (data == ta.data) { return *this; }

				data = std::move(ta.data);
				inited = ta.inited;
				return *this;
			}
		};

		class UserType : public Type {
		private:
			class UserTypeCompare
			{
			public:
				bool operator() (const UserType* x, const UserType* y) const {
					return x->GetName() < y->GetName();
				}
			};
			class ItemTypeStringPtrCompare {
			public:
				bool operator() (const ItemType<std::string>* x, const ItemType<std::string>* y) const {
					return x->GetName() < y->GetName();
				}
			};
			int binary_find_ut(const std::vector<UserType*>& arr, const UserType& x) const
			{
				if (arr.empty()) { return -1; }

				int left = 0, right = arr.size() - 1;
				int middle = (left + right) / 2;

				while (left <= right) {
					if (arr[middle]->GetName() == x.GetName()) {
						return middle;
					}
					else if (arr[middle]->GetName() < x.GetName()) {
						left = middle + 1;
					}
					else {
						right = middle - 1;
					}

					middle = (left + right) / 2;
				}
				return -1;
			}
			int binary_find_it(const std::vector<ItemType<std::string>*> & arr, const ItemType<std::string> & x) const {
				if (arr.empty()) { return -1; }

				int left = 0, right = arr.size() - 1;
				int middle = (left + right) / 2;

				while (left <= right) {
					if (arr[middle]->GetName() == x.GetName()) {
						return middle;
					}
					else if (arr[middle]->GetName() < x.GetName()) {
						left = middle + 1;
					}
					else {
						right = middle - 1;
					}

					middle = (left + right) / 2;
				}
				return -1;
			}
		public:
			void PushComment(const std::string & comment)
			{
				commentList.push_back(comment);
			}
			void PushComment(std::string && comment)
			{
				commentList.push_back(move(comment));
			}
			int GetCommentListSize()const { return commentList.size(); }
			const std::string& GetCommentList(const int idx) const { return commentList[idx]; }
			std::string& GetCommentList(const int idx) {
				return commentList[idx];
			}
		public:
			int GetIListSize()const { return ilist.size(); }
			int GetItemListSize()const { return itemList.size(); }
			int GetUserTypeListSize()const { return userTypeList.size(); }
			ItemType<std::string>& GetItemList(const int idx) { return itemList[idx]; }
			const ItemType<std::string>& GetItemList(const int idx) const { return itemList[idx]; }
			UserType*& GetUserTypeList(const int idx) { return userTypeList[idx]; }
			const UserType*& GetUserTypeList(const int idx) const { return const_cast<const UserType*&>(userTypeList[idx]); }

			bool IsItemList(const int idx) const
			{
				return ilist[idx] == 1;
			}
			bool IsUserTypeList(const int idx) const
			{
				return ilist[idx] == 2;
			}

			void AddItemList(const ItemType<std::string> & strTa)
			{
				for (int i = 0; i < strTa.size(); ++i) {
					this->AddItem(strTa.GetName(), strTa.Get(i));
				}
			}
			void AddItemList(ItemType<std::string> && strTa)
			{
				for (int i = 0; i < strTa.size(); ++i) {
					this->AddItem(std::move(strTa.GetName()), std::move(strTa.Get(i)));
				}
			}
		public:
			void SetParent(UserType * other)
			{
				parent = other;
			}
			UserType* GetParent() { return parent; }
			const UserType* GetParent()const { return parent; }

			void LinkUserType(UserType * ut) // danger fucntion?
			{
				userTypeList.push_back(ut);
				ilist.push_back(2);
				ut->parent = this;

				useSortedUserTypeList = false;
			}
		private:
			UserType* parent = nullptr;
			std::vector<std::string> commentList; // std::string?
			std::vector<int> ilist;
			std::vector< ItemType<std::string> > itemList;
			std::vector< UserType* > userTypeList;
			mutable std::vector< ItemType<std::string>* > sortedItemList;
			mutable std::vector< UserType* > sortedUserTypeList;
			mutable bool useSortedItemList = false;
			mutable bool useSortedUserTypeList = false;
			bool noRemove = false;
			//bool reservedA = false;
		public:
			explicit UserType(std::string && name, bool noRemove = false) : Type(move(name)), parent(nullptr), noRemove(noRemove) { }
			explicit UserType(const std::string & name = "", bool noRemove = false) : Type(name), parent(nullptr), noRemove(noRemove) { } //, userTypeList_sortFlagA(true), userTypeList_sortFlagB(true) { }
			UserType(const UserType & ut) : Type(ut.GetName()) {
				Reset(ut);  // Initial
			}
			UserType(UserType && ut) : Type(move(ut.GetName())) {
				Reset2(std::move(ut));
			}
			virtual ~UserType() {
				if (false == noRemove) {
					_Remove();
				}
			}
			UserType& operator=(const UserType & ut) {
				if (this == &ut) { return *this; }
				Type::operator=(ut);

				RemoveUserTypeList();
				Reset(ut);
				return *this;
			}
			UserType& operator=(UserType && ut) {
				if (this == &ut) { return *this; }

				Type::operator=(std::move(ut));
				RemoveUserTypeList();
				Reset2(std::move(ut));
				return *this;
			}
		private:
			void Reset(const UserType & ut) { /// UT ÔøΩÔøΩ√ºÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩ—¥ÔøΩ.
											 //	userTypeList_sortFlagA = ut.userTypeList_sortFlagA;
											 //userTypeList_sortFlagB = ut.userTypeList_sortFlagB;

				ilist = ut.ilist;
				itemList = ut.itemList;
				//parent = ut.parent;
				commentList = ut.commentList;

				sortedItemList = ut.sortedItemList;
				sortedUserTypeList = ut.sortedUserTypeList;

				useSortedItemList = ut.useSortedItemList;
				useSortedUserTypeList = ut.useSortedUserTypeList;

				noRemove = ut.noRemove;

				userTypeList.reserve(ut.userTypeList.size());

				for (int i = 0; i < ut.userTypeList.size(); ++i) {
					userTypeList.push_back(new UserType(*ut.userTypeList[i]));
					userTypeList.back()->parent = this;
				}
				if (useSortedUserTypeList) {
					sortedUserTypeList.clear();
					for (int i = 0; i < userTypeList.size(); ++i) {
						sortedUserTypeList.push_back(userTypeList[i]);
					}
				}
			}
			void Reset2(UserType && ut) {
				//std::swap( userTypeList_sortFlagA, ut.userTypeList_sortFlagA );
				//std::swap( userTypeList_sortFlagB, ut.userTypeList_sortFlagB );

				//no use - //parent = ut.parent;
				//no use - //ut.parent = nullptr; /// chk..
				ilist = std::move(ut.ilist);
				itemList = std::move(ut.itemList);
				commentList = std::move(ut.commentList);

				sortedItemList = std::move(ut.sortedItemList);
				sortedUserTypeList = std::move(ut.sortedUserTypeList);

				std::swap(this->noRemove, ut.noRemove);

				useSortedItemList = ut.useSortedItemList;
				useSortedUserTypeList = ut.useSortedUserTypeList;

				userTypeList.reserve(ut.userTypeList.size());

				for (int i = 0; i < ut.userTypeList.size(); ++i) {
					userTypeList.push_back(std::move(ut.userTypeList[i]));
					ut.userTypeList[i] = nullptr;
					userTypeList.back()->parent = this;
				}
				ut.userTypeList.clear();

				if (useSortedUserTypeList) {
					sortedUserTypeList.clear();
					for (int i = 0; i < userTypeList.size(); ++i) {
						sortedUserTypeList.push_back(userTypeList[i]);
					}
				}
			}

			void _Remove()
			{
				//parent = nullptr;
				ilist = std::vector<int>();
				itemList = std::vector< ItemType<std::string> >();

				sortedItemList.clear();
				sortedUserTypeList.clear();

				useSortedItemList = false;
				useSortedUserTypeList = false;
				RemoveUserTypeList();

				commentList.clear();
			}
			// static ??
		public:
			int GetIlistIndex(const int index, const int type)
			{
				return _GetIlistIndex(ilist, index, type);
			}
			int GetUserTypeIndexFromIlistIndex(const int ilist_idx)
			{
				return _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
			}
			int GetItemIndexFromIlistIndex(const int ilist_idx)
			{
				return _GetItemIndexFromIlistIndex(ilist, ilist_idx);
			}
		private:
			/// val : 1 or 2
			int _GetIndex(const std::vector<int> & ilist, const int val, const int start = 0) {
				for (int i = start; i < ilist.size(); ++i) {
					if (ilist[i] == val) { return i; }
				}
				return -1;
			}
			// test? - need more thinking!
			int _GetItemIndexFromIlistIndex(const std::vector<int> & ilist, const int ilist_idx) {
				if (ilist.size() == ilist_idx) { return ilist.size(); }
				int idx = _GetIndex(ilist, 1, 0);
				int item_idx = -1;

				while (idx != -1) {
					item_idx++;
					if (ilist_idx == idx) { return item_idx; }
					idx = _GetIndex(ilist, 1, idx + 1);
				}

				return -1;
			}
			int _GetUserTypeIndexFromIlistIndex(const std::vector<int> & ilist, const int ilist_idx) {
				if (ilist.size() == ilist_idx) { return ilist.size(); }
				int idx = _GetIndex(ilist, 2, 0);
				int usertype_idx = -1;

				while (idx != -1) {
					usertype_idx++;
					if (ilist_idx == idx) { return usertype_idx; }
					idx = _GetIndex(ilist, 2, idx + 1);
				}

				return -1;
			}
			/// type : 1 or 2
			int _GetIlistIndex(const std::vector<int> & ilist, const int index, const int type) {
				int count = -1;

				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == type) {
						count++;
						if (index == count) {
							return i;
						}
					}
				}
				return -1;
			}
		public:
			void RemoveItemList(const int idx)
			{
				// left shift start idx, to end, at itemList. and resize!
				for (int i = idx + 1; i < GetItemListSize(); ++i) {
					itemList[i - 1] = std::move(itemList[i]);
				}
				itemList.resize(itemList.size() - 1);
				//  ilist left shift and resize - count itemType!
				int count = 0;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 1) { count++; }
					if (count == idx + 1) {
						// iÔøΩÔøΩÔøΩÔøΩ left shift!and resize!
						for (int k = i + 1; k < ilist.size(); ++k) {
							ilist[k - 1] = std::move(ilist[k]);
						}
						ilist.resize(ilist.size() - 1);
						break;
					}
				}

				useSortedItemList = false;
			}
			void RemoveUserTypeList(const int idx, const bool chk = true)
			{
				if (chk && userTypeList[idx]) {
					delete userTypeList[idx];
				}
				//std::cout << GetUserTypeListSize() << std::endl;
				// left shift start idx, to end, at itemList. and resize!
				for (int i = idx + 1; i < GetUserTypeListSize(); ++i) {
					userTypeList[i - 1] = std::move(userTypeList[i]);
				}
				userTypeList.resize(userTypeList.size() - 1);
				//  ilist left shift and resize - count itemType!
				int count = 0;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 2) { count++; }
					if (count == idx + 1) {
						// iÔøΩÔøΩÔøΩÔøΩ left shift!and resize!
						for (int k = i + 1; k < ilist.size(); ++k) {
							ilist[k - 1] = std::move(ilist[k]);
						}
						ilist.resize(ilist.size() - 1);
						break;
					}
				}

				useSortedUserTypeList = false;
			}
			void RemoveItemList(const std::string & varName)
			{
				int k = _GetIndex(ilist, 1, 0);
				std::vector<ItemType<std::string>> tempDic;
				for (int i = 0; i < itemList.size(); ++i) {
					if (varName != itemList[i].GetName()) {
						tempDic.push_back(itemList[i]);
						k = _GetIndex(ilist, 1, k + 1);
					}
					else {
						// remove item, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 1, k);
					}
				}
				itemList = std::move(tempDic);

				useSortedItemList = false;
			}
			void RemoveItemList() /// ALL
			{
				itemList = std::vector<ItemType<std::string>>();
				//
				std::vector<int> temp;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 2)
					{
						temp.push_back(2);
					}
				}
				ilist = move(temp);

				useSortedItemList = false;
			}
			void RemoveEmptyItem() // fixed..
			{
				int k = _GetIndex(ilist, 1, 0);
				std::vector<ItemType<std::string>> tempDic;
				for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].size() > 0) {
						tempDic.push_back(itemList[i]);
						k = _GetIndex(ilist, 1, k + 1);
					}
					else {
						// remove item, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 1, k);
					}
				}
				itemList = move(tempDic);

				useSortedItemList = false;
			}
			void Remove()
			{
				/// parent->removeUserType(name); - ToDo - X
				ilist = std::vector<int>();
				itemList = std::vector< ItemType<std::string> >();

				RemoveUserTypeList();

				commentList.clear();

				sortedItemList.clear();
				sortedUserTypeList.clear();

				useSortedItemList = false;
				useSortedUserTypeList = false;
				//parent = nullptr;
			}
			void RemoveUserTypeList() { /// chk memory leak test!!
				for (int i = 0; i < userTypeList.size(); i++) {
					if (nullptr != userTypeList[i]) {
						delete userTypeList[i]; //
						userTypeList[i] = nullptr;
					}
				}
				// DO Empty..
				userTypeList.clear();

				std::vector<int> temp;
				for (int i = 0; i < ilist.size(); ++i) {
					if (ilist[i] == 1)
					{
						temp.push_back(1);
					}
				}
				ilist = move(temp);

				useSortedUserTypeList = false;
			}
			void RemoveUserTypeList(const std::string & varName, const bool chk = true)
			{
				int k = _GetIndex(ilist, 2, 0);
				std::vector<UserType*> tempDic;
				for (int i = 0; i < userTypeList.size(); ++i) {
					if (varName != userTypeList[i]->GetName()) {
						tempDic.push_back(userTypeList[i]);
						k = _GetIndex(ilist, 2, k + 1);
					}
					else {
						if (chk && userTypeList[i]) {
							delete userTypeList[i];
						}
						// remove usertypeitem, ilist left shift 1.
						for (int j = k + 1; j < ilist.size(); ++j) {
							ilist[j - 1] = ilist[j];
						}
						ilist.resize(ilist.size() - 1);
						k = _GetIndex(ilist, 2, k);
					}
				}
				userTypeList = move(tempDic);

				useSortedUserTypeList = false;
			}
			//			
			void RemoveList(const int idx) // ilist_idx!
			{
				// chk whether item or usertype.
				// find item_idx or usertype_idx.
				// remove item or remove usertype.
				if (ilist[idx] == 1) {
					int item_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[i] == 1) { item_idx++; }
					}

					RemoveItemList(item_idx);
				}
				else {
					int usertype_idx = -1;

					for (int i = 0; i < ilist.size() && i <= idx; ++i) {
						if (ilist[i] == 2) { usertype_idx++; }
					}

					RemoveUserTypeList(usertype_idx);
				}
			}
		public:
			bool empty()const { return ilist.empty(); }

			// chk
			void InsertItemByIlist(const int ilist_idx, const std::string & name, const std::string & item) {
				ilist.push_back(1);
				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;


				int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

				itemList.emplace_back("", std::string(""));

				if (itemIndex != -1) {
					for (int i = itemList.size() - 1; i > itemIndex; --i) {
						itemList[i] = move(itemList[i - 1]);
					}
					itemList[itemIndex] = ItemType<std::string>(name, item);
				}
				else {
					itemList[0] = ItemType<std::string>(name, item); // chk!!
				}

				useSortedItemList = false;
			}
			void InsertItemByIlist(const int ilist_idx, std::string && name, std::string && item) {
				ilist.push_back(1);


				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 1;

				int itemIndex = _GetItemIndexFromIlistIndex(ilist, ilist_idx);

				itemList.emplace_back("", std::string(""));
				if (itemIndex != -1) {
					for (int i = itemList.size() - 1; i > itemIndex; --i) {
						itemList[i] = move(itemList[i - 1]);
					}
					itemList[itemIndex] = ItemType<std::string>(move(name), move(item));
				}
				else {
					itemList[0] = ItemType<std::string>(move(name), move(item));
				}


				useSortedItemList = false;
			}
			// chk
			void InsertUserTypeByIlist(const int ilist_idx, UserType && item) {
				ilist.push_back(2);
				UserType* temp = new UserType(std::move(item));

				temp->parent = this;

				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
				userTypeList.push_back(nullptr);
				if (userTypeIndex != -1) {
					for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
						userTypeList[i] = std::move(userTypeList[i - 1]);
					}
					userTypeList[userTypeIndex] = temp;
				}
				else {
					userTypeList[0] = temp;
				}


				useSortedUserTypeList = false;
			}
			void InsertUserTypeByIlist(const int ilist_idx, const UserType & item) {
				ilist.push_back(2);
				UserType* temp = new UserType(item);

				temp->parent = this;


				for (int i = ilist.size() - 1; i > ilist_idx; --i) {
					ilist[i] = ilist[i - 1];
				}
				ilist[ilist_idx] = 2;

				int userTypeIndex = _GetUserTypeIndexFromIlistIndex(ilist, ilist_idx);
				userTypeList.push_back(nullptr);
				if (userTypeIndex != -1) {
					for (int i = userTypeList.size() - 1; i > userTypeIndex; --i) {
						userTypeList[i] = std::move(userTypeList[i - 1]);
					}
					userTypeList[userTypeIndex] = temp;
				}
				else {
					userTypeList[0] = temp;
				}

				useSortedUserTypeList = false;
			}

			/*
			// chk
			void InsertItem(const int item_idx, const std::string& name, const std::string& item) {
			int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

			ilist.push_back(0);
			for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
			ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 1;

			itemList.emplace_back("", "");
			for (int i = item_idx + 1; i < itemList.size(); ++i) {
			itemList[i] = move(itemList[i - 1]);
			}
			itemList[item_idx] = ItemType<std::string>(name, item);
			}
			void InsertItem(const int item_idx, std::string&& name, std::string&& item) {
			int ilist_idx = _GetIlistIndex(ilist, item_idx, 1);

			ilist.push_back(0);
			for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
			ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 1;

			itemList.emplace_back("", "");
			for (int i = item_idx + 1; i < itemList.size(); ++i) {
			itemList[i] = move(itemList[i - 1]);
			}
			itemList[item_idx] = ItemType<std::string>(move(name), move(item));
			}
			// chk
			void InsertUserType(const int ut_idx, UserType&& item) {
			int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
			UserType* temp = new UserType(std::move(item));

			temp->parent = this;

			ilist.push_back(0);
			for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
			ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 2;

			userTypeList.push_back(nullptr);
			for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
			userTypeList[i] = userTypeList[i - 1];
			}
			userTypeList[ut_idx] = temp;
			}
			void InsertUserType(const int ut_idx, const UserType& item) {
			int ilist_idx = _GetIlistIndex(ilist, ut_idx, 2);
			UserType* temp = new UserType(item);

			temp->parent = this;

			ilist.push_back(0);
			for (int i = ilist_idx + 1; i < ilist.size(); ++i) {
			ilist[i] = ilist[i - 1];
			}
			ilist[ilist_idx] = 2;

			userTypeList.push_back(nullptr);
			for (int i = ut_idx + 1; i < userTypeList.size(); ++i) {
			userTypeList[i] = userTypeList[i - 1];
			}
			userTypeList[ut_idx] = temp;
			}
			*/
			//
			void ReserveIList(int offset)
			{
				if (offset > 0) {
					ilist.reserve(offset);
				}
			}
			void ReserveItemList(int offset)
			{
				if (offset > 0) {
					itemList.reserve(offset);
				}
			}
			void ReserveUserTypeList(int offset)
			{
				if (offset > 0) {
					userTypeList.reserve(offset);
				}
			}
			void AddItem(std::string && name, std::string && item) {
				itemList.emplace_back(move(name), move(item));
				ilist.push_back(1);

				useSortedItemList = false;
			}
			void AddItem(const std::string & name, const std::string & item) {
				itemList.emplace_back(name, item);
				ilist.push_back(1);

				useSortedItemList = false;
			}
			void AddUserTypeItem(UserType && item) {
				UserType* temp = new UserType(std::move(item));
				temp->parent = this;
				//temp->SetName("");

				ilist.push_back(2);

				userTypeList.push_back(temp);

				useSortedUserTypeList = false;
			}
			void AddUserTypeItem(const UserType & item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.push_back(2);

				userTypeList.push_back(temp);

				useSortedUserTypeList = false;
			}
			void AddItemAtFront(std::string && name, std::string && item) {
				itemList.emplace(itemList.begin(), name, item);

				ilist.insert(ilist.begin(), 1);

				useSortedItemList = false;
			}
			void AddItemAtFront(const std::string & name, const std::string & item) {
				itemList.emplace(itemList.begin(), name, item);

				ilist.insert(ilist.begin(), 1);

				useSortedItemList = false;
			}
			void AddUserTypeItemAtFront(const UserType & item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.insert(ilist.begin(), 2);

				userTypeList.insert(userTypeList.begin(), temp);

				useSortedUserTypeList = false;
			}
			void AddUserTypeItemAtFront(UserType && item) {
				UserType* temp = new UserType(item);
				temp->parent = this;

				ilist.insert(ilist.begin(), 2);

				userTypeList.insert(userTypeList.begin(), temp);

				useSortedUserTypeList = false;
			}


			// $it?
			std::vector<ItemType<std::string>> GetItem(const std::string & name) const {
				std::vector<ItemType<std::string>> temp;
				if (String::startsWith(name, "$.") && name.size() >= 5) {
					// later, change to binary search?
					std::string str = name.substr(3, name.size() - 4);
					std::regex rgx(str);

					for (int i = 0; i < itemList.size(); ++i) {
						if (regex_match(itemList[i].GetName(), rgx)) {
							temp.push_back(itemList[i]);
						}
					}
				}
				else {
					if (false == useSortedItemList) {
						sortedItemList.clear();
						for (int i = 0; i < itemList.size(); ++i) {
							sortedItemList.push_back((ItemType<std::string>*) & itemList[i]);
						}

						std::sort(sortedItemList.begin(), sortedItemList.end(), ItemTypeStringPtrCompare());

						useSortedItemList = true;
					}
					// binary search
					{
						ItemType<std::string> x = ItemType<std::string>(name, "");
						int idx = binary_find_it(sortedItemList, x);
						if (idx >= 0) {
							int start = idx;
							int last = idx;

							for (int i = idx - 1; i >= 0; --i) {
								if (name == sortedItemList[i]->GetName()) {
									start--;
								}
								else {
									break;
								}
							}
							for (int i = idx + 1; i < sortedItemList.size(); ++i) {
								if (name == sortedItemList[i]->GetName()) {
									last++;
								}
								else {
									break;
								}
							}

							for (int i = start; i <= last; ++i) {
								temp.push_back(*sortedItemList[i]);
							}
						}
						else {
							//std::cout << "no found" << std::endl;
						}
					}

					/*
					for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].GetName() == name) {
					temp.push_back(itemList[i]);
					}
					}
					*/
				}
				return temp;
			}
			// regex to SetItem?
			bool SetItem(const std::string & name, const std::string & value) {
				int index = -1;

				for (int i = 0; i < itemList.size(); ++i) {
					if (itemList[i].GetName() == name)
					{
						itemList[i].Set(0, value);
						index = i;
					}
				}

				return -1 != index;
			}
			/// add set Data
			bool SetItem(const int var_idx, const std::string & value) {
				itemList[var_idx].Set(0, value);
				return true;
			}

			//// O(N) -> O(logN)?
			std::vector<UserType*> GetUserTypeItem(const std::string & name) const { /// chk...
				std::vector<UserType*> temp;

				if (false == useSortedUserTypeList) {
					// make sortedUserTypeList.
					sortedUserTypeList = userTypeList;

					std::sort(sortedUserTypeList.begin(), sortedUserTypeList.end(), UserTypeCompare());

					useSortedUserTypeList = true;
				}
				// binary search
				{
					UserType x = UserType(name);
					int idx = binary_find_ut(sortedUserTypeList, x);
					if (idx >= 0) {
						int start = idx;
						int last = idx;

						for (int i = idx - 1; i >= 0; --i) {
							if (name == sortedUserTypeList[i]->GetName()) {
								start--;
							}
							else {
								break;
							}
						}
						for (int i = idx + 1; i < sortedUserTypeList.size(); ++i) {
							if (name == sortedUserTypeList[i]->GetName()) {
								last++;
							}
							else {
								break;
							}
						}

						for (int i = start; i <= last; ++i) {
							temp.push_back(sortedUserTypeList[i]);
						}
					}
					else {
						//std::cout << "no found" << std::endl;
					}
				}

				/*
				for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[i]->GetName() == name) {
				temp.push_back(userTypeList[i]);
				}
				}
				*/
				return temp;
			}

			// deep copy.
			std::vector<UserType*> GetCopyUserTypeItem(const std::string & name) const { /// chk...
				std::vector<UserType*> temp;

				if (false == useSortedUserTypeList) {
					// make sortedUserTypeList.
					sortedUserTypeList = userTypeList;

					std::sort(sortedUserTypeList.begin(), sortedUserTypeList.end(), UserTypeCompare());

					useSortedUserTypeList = true;
				}
				// binary search
				{
					UserType x = UserType(name);
					int idx = binary_find_ut(sortedUserTypeList, x);
					if (idx >= 0) {
						int start = idx;
						int last = idx;

						for (int i = idx - 1; i >= 0; --i) {
							if (name == sortedUserTypeList[i]->GetName()) {
								start--;
							}
							else {
								break;
							}
						}
						for (int i = idx + 1; i < sortedUserTypeList.size(); ++i) {
							if (name == sortedUserTypeList[i]->GetName()) {
								last++;
							}
							else {
								break;
							}
						}

						for (int i = start; i <= last; ++i) {
							temp.push_back(new UserType(*sortedUserTypeList[i]));
						}
					}
					else {
						//std::cout << "no found" << std::endl;
					}
				}

				/*
				for (int i = 0; i < userTypeList.size(); ++i) {
				if (userTypeList[i]->GetName() == name) {
				temp.push_back(new UserType(*userTypeList[i]));
				}
				}
				*/
				return temp;
			}
		public:
			bool GetUserTypeItemRef(const int idx, UserType * &ref)
			{
				ref = userTypeList[idx];
				return true;
			}
			bool GetLastUserTypeItemRef(const std::string & name, UserType * &ref) {
				int idx = -1;

				for (int i = userTypeList.size() - 1; i >= 0; --i)
				{
					if (name == userTypeList[i]->GetName()) {
						idx = i;
						break;
					}
				}
				if (idx > -1) {
					ref = userTypeList[idx];
				}
				return idx > -1;
			}
		private:
			/// save1 - like EU4 savefiles.
			void Save1(std::ostream & stream, const UserType * ut, const int depth = 0) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				const bool existUserType = ut->GetUserTypeListSize() > 0;

				for (int i = 0; i < ut->commentList.size(); ++i) {
					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << (ut->commentList[i]);

					if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
						stream << "\n";
					}
				}

				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							std::string temp;
							if (existUserType) {
								for (int k = 0; k < depth; ++k) {
									temp += "\t";
								}
							}
							if (ut->itemList[itemListCount].GetName() != "") {
								temp += ut->itemList[itemListCount].GetName();
								temp += "=";
							}
							temp += ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1) {
								temp += " ";
							}
							stream << temp;
						}
						if (i != ut->ilist.size() - 1) {
							if (existUserType) {
								stream << "\n";
							}
							else {
								stream << " "; // \n
							}
						}
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}

						if (ut->userTypeList[userTypeListCount]->GetName() != "") {
							stream << ut->userTypeList[userTypeListCount]->GetName() << "=";
						}

						stream << "{\n";

						Save1(stream, ut->userTypeList[userTypeListCount], depth + 1);
						stream << "\n";

						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						stream << "}";
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}

						userTypeListCount++;
					}
				}
			}
			/// save2 - for more speed loading data!?
			void Save2(std::ostream & stream, const UserType * ut, const int depth = 0) const {
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ut->commentList.size(); ++i) {
					for (int k = 0; k < depth; ++k) {
						stream << "\t";
					}
					stream << (ut->commentList[i]);

					if (i < ut->commentList.size() - 1 || false == ut->ilist.empty()) {
						stream << "\n";
					}

				}
				for (int i = 0; i < ut->ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ut->ilist[i] == 1) {
						for (int j = 0; j < ut->itemList[itemListCount].size(); j++) {
							//for (int k = 0; k < depth; ++k) {
							//	stream << "\t";
							//}
							if (ut->itemList[itemListCount].GetName() != "")
								stream << ut->itemList[itemListCount].GetName() << " = ";
							stream << ut->itemList[itemListCount].Get(j);
							if (j != ut->itemList[itemListCount].size() - 1)
								stream << " ";
						}
						if (i != ut->ilist.size() - 1) {
							stream << " ";//"\n";
						}
						itemListCount++;
					}
					else if (ut->ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (ut->userTypeList[userTypeListCount]->GetName() != "")
						{
							stream << ut->userTypeList[userTypeListCount]->GetName() << " = ";
						}
						stream << "{\n";

						Save2(stream, ut->userTypeList[userTypeListCount], depth + 1);
						stream << "\n";

						for (int k = 0; k < depth; ++k) {
							stream << "\t";
						}
						stream << "}";
						if (i != ut->ilist.size() - 1) {
							stream << "\n";
						}
						userTypeListCount++;
					}
				}
			}

		public:
			void Save1(std::ostream & stream, int depth = 0) const {
				Save1(stream, this, depth);
			}

			std::string ItemListToString()const
			{
				std::string temp;
				int itemListCount = 0;

				for (int i = 0; i < itemList.size(); ++i) {
					for (int j = 0; j < itemList[itemListCount].size(); j++) {
						if (itemList[itemListCount].GetName() != "")
							temp = temp + itemList[itemListCount].GetName() + " = ";
						temp = temp + itemList[itemListCount].Get(j);
						if (j != itemList[itemListCount].size() - 1) {
							temp = temp + "/";
						}
					}
					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					itemListCount++;
				}
				return temp;
			}
			std::string ItemListNamesToString()const
			{
				std::string temp;
				int itemListCount = 0;

				for (int i = 0; i < itemList.size(); ++i) {
					for (int j = 0; j < itemList[itemListCount].size(); j++) {
						if (itemList[itemListCount].GetName() != "")
							temp = temp + itemList[itemListCount].GetName();
						else
							temp = temp + " ";

						if (j != itemList[itemListCount].size() - 1) {
							temp = temp + "/";
						}
					}
					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					itemListCount++;
				}
				return temp;
			}
			std::vector<std::string> userTypeListNamesToStringArray()const
			{
				std::vector<std::string> temp;
				int userTypeListCount = 0;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[userTypeListCount]->GetName() != "") {
						temp.push_back(userTypeList[userTypeListCount]->GetName());
					}
					else {
						temp.push_back(" "); // chk!! cf) wiz2::load_data::Utility::Find function...
					}
					userTypeListCount++;
				}
				return temp;
			}
			std::string UserTypeListNamesToString()const
			{
				std::string temp;
				int userTypeListCount = 0;

				for (int i = 0; i < userTypeList.size(); ++i) {
					if (userTypeList[userTypeListCount]->GetName() != "") {
						temp = temp + userTypeList[userTypeListCount]->GetName();
					}
					else {
						temp = temp + " "; // chk!! cf) wiz2::load_data::Utility::Find function...
					}

					if (i != itemList.size() - 1)
					{
						temp = temp + "/";
					}
					userTypeListCount++;
				}
				return temp;
			}
			std::string ToString()const
			{
				std::string temp;
				int itemListCount = 0;
				int userTypeListCount = 0;

				for (int i = 0; i < ilist.size(); ++i) {
					//std::cout << "ItemList" << endl;
					if (ilist[i] == 1) {
						for (int j = 0; j < itemList[itemListCount].size(); j++) {
							if (itemList[itemListCount].GetName() != "") {
								temp.append(itemList[itemListCount].GetName());
								temp.append(" = ");
							}
							temp.append(itemList[itemListCount].Get(j));
							if (j != itemList[itemListCount].size() - 1)
							{
								temp.append(" ");
							}
						}
						if (i != ilist.size() - 1) {
							temp.append(" ");
						}
						itemListCount++;
					}
					else if (ilist[i] == 2) {
						// std::cout << "UserTypeList" << endl;
						if (userTypeList[userTypeListCount]->GetName() != "") {
							temp.append(userTypeList[userTypeListCount]->GetName());
							temp.append(" = ");
						}
						temp.append(" { ");
						temp.append(userTypeList[userTypeListCount]->ToString());
						temp.append(" ");
						temp.append(" }");
						if (i != ilist.size() - 1) {
							temp.append(" ");
						}

						userTypeListCount++;
					}
				}
				return temp;
			}
		public:
			// find userType! not itemList!,// this has bug-> ex) /./test/../dd/../ has not work! 
			static std::pair<bool, std::vector< UserType*> > Find(UserType * global, const std::string & _position, StringBuilder * builder) /// option, option_offset
			{
				std::string position = _position;
				std::vector< UserType* > temp;

				if (!position.empty() && position[0] == '@') { position.erase(position.begin()); }
				if (position.empty()) { temp.push_back(global); return{ true, temp }; }
				if (position == ".") { temp.push_back(global); return{ true, temp }; }
				if (position == "/./") { temp.push_back(global); return{ true, temp }; } // chk..
				if (position == "/.") { temp.push_back(global); return{ true, temp }; }
				if (position == "/") { temp.push_back(global); return{ true, temp }; }
				if (String::startsWith(position, "/./"))
				{
					position = String::substring(position, 3);
				}

				StringTokenizer tokenizer(position, "/", builder, 1);
				std::vector<std::string> strVec;
				std::list<std::pair< UserType*, int >> utDeck;
				std::pair<UserType*, int> utTemp;
				utTemp.first = global;
				utTemp.second = 0;

				for (int i = 0; i < tokenizer.countTokens(); ++i) {
					std::string strTemp = tokenizer.nextToken();
					if (strTemp == "root" && i == 0) {
					}
					else {
						strVec.push_back(strTemp);
					}

					if ((strVec.size() >= 1) && (" " == strVec[strVec.size() - 1])) /// chk!!
					{
						strVec[strVec.size() - 1] = "";
					}
					else if ((strVec.size() >= 1) && ("_" == strVec[strVec.size() - 1]))
					{
						strVec[strVec.size() - 1] = "";
					}
				}

				// maybe, has bug!
				{
					int count = 0;

					for (int i = 0; i < strVec.size(); ++i) {
						if (strVec[i] == "..") {
							count++;
						}
						else {
							break;
						}
					}

					std::reverse(strVec.begin(), strVec.end());

					for (int i = 0; i < count; ++i) {
						if (utTemp.first == nullptr) {
							return{ false, std::vector< UserType* >() };
						}
						utTemp.first = utTemp.first->GetParent();
						strVec.pop_back();
					}
					std::reverse(strVec.begin(), strVec.end());
				}

				utDeck.push_front(utTemp);

				bool exist = false;
				while (false == utDeck.empty()) {
					utTemp = utDeck.front();
					utDeck.pop_front();

					if (utTemp.second < strVec.size() &&
						wiz2::String::startsWith(strVec[utTemp.second], "$ut")
						)
					{
						int idx = std::stoi(wiz2::String::substring(strVec[utTemp.second], 3));

						if (idx < 0 || idx >= utTemp.first->GetUserTypeListSize()) {
							throw std::string("ERROR NOT VALID IDX");
						}

						utDeck.push_front(std::make_pair(utTemp.first->GetUserTypeList(idx), utTemp.second + 1));
					}
					else if (utTemp.second < strVec.size() && strVec[utTemp.second] == "$")
					{
						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							UserType* x = utTemp.first->GetUserTypeList(j);
							utDeck.push_front(std::make_pair(x, utTemp.second + 1));
						}
					}
					else if (utTemp.second < strVec.size() && String::startsWith(strVec[utTemp.second], "$.")) /// $."abc"
					{
						std::string rex_str = strVec[utTemp.second].substr(3, strVec[utTemp.second].size() - 4);
						std::regex rgx(rex_str);

						for (int j = utTemp.first->GetUserTypeListSize() - 1; j >= 0; --j) {
							if (std::regex_match(utTemp.first->GetUserTypeList(j)->GetName(), rgx)) {
								UserType* x = utTemp.first->GetUserTypeList(j);
								utDeck.push_front(std::make_pair(x, utTemp.second + 1));
							}
						}
					}
					else if (utTemp.second < strVec.size() &&
						(utTemp.first->GetUserTypeItem(strVec[utTemp.second]).empty() == false))
					{
						auto  x = utTemp.first->GetUserTypeItem(strVec[utTemp.second]);
						for (int j = x.size() - 1; j >= 0; --j) {
							utDeck.push_front(std::make_pair(x[j], utTemp.second + 1));
						}
					}

					if (utTemp.second == strVec.size()) {
						exist = true;
						temp.push_back(utTemp.first);
					}
				}
				if (false == exist) { return{ false, std::vector<UserType*>() }; }
				return{ true, temp };
			}
		};

		// chk!!
		template <class Reserver>
		static bool ChkComment(ARRAY_QUEUE<Token> & strVec, wiz2::load_data::UserType * ut, Reserver reserver, const int offset, const wiz2::LoadDataOption & option)
		{
			if (strVec.size() < offset) {
				reserver(strVec, option);
				while (strVec.size() < offset) // 
				{
					reserver(strVec, option);
					if (
						strVec.size() < offset &&
						reserver.end()
						) {
						return false;
					}
				}
			}

			auto x = strVec.begin();
			int count = 0;

			do {
				if (x->isComment) { // x.ptr->, x.data->?
					ut->PushComment((std::move(x->str)));
					x = strVec.erase(x);
				}
				else if (count == offset - 1) {
					return true;
				}
				else {
					count++;
					++x;
					//x.ptr++;
					//x.pos++;
				}

				if (x == strVec.end()) {
					reserver(strVec, option);
					x = strVec.begin() + count;

					while (strVec.size() < offset) // 
					{
						reserver(strVec, option);
						x = strVec.begin() + count;
						if (
							strVec.size() < offset &&
							reserver.end()
							) {
							return false;
						}
					}
				}
			} while (true);
		}

		template<class Reserver>
		static std::string Top(ARRAY_QUEUE<Token> & strVec, wiz2::load_data::UserType * ut, Reserver reserver, const wiz2::LoadDataOption & option)
		{
			if (strVec.empty() || strVec[0].isComment) {
				if (false == ChkComment(strVec, ut, reserver, 1, option)) {
					return std::string();
				}
			}
			if (strVec.empty()) { return std::string(); }
			return strVec[0].str;
		}
		template <class Reserver>
		static bool Pop(ARRAY_QUEUE<Token> & strVec, std::string * str, wiz2::load_data::UserType * ut, Reserver reserver, const wiz2::LoadDataOption & option)
		{
			if (strVec.empty() || strVec[0].isComment) {
				if (false == ChkComment(strVec, ut, reserver, 1, option)) {
					return false;
				}
			}

			if (strVec.empty()) {
				return false;
			}

			if (str) {
				Token token;
				token = strVec.front();
				strVec.pop_front();
				*str = (std::move(token.str));
				//*str = move(strVec.front().str);
			}
			else {
				strVec.pop_front();
			}
			//strVec.pop_front();

			return true;
		}

		// lookup just one!
		template <class Reserver>
		static std::pair<bool, std::string> LookUp(ARRAY_QUEUE<Token> & strVec, wiz2::load_data::UserType * ut, Reserver reserver, const wiz2::LoadDataOption & option)
		{
			if (!(strVec.size() >= 2 && false == strVec[0].isComment && false == strVec[1].isComment)) {
				if (false == ChkComment(strVec, ut, reserver, 2, option)) {
					return{ false, "" };
				}
			}

			if (strVec.size() >= 2) {
				return{ true, strVec[1].str };
			}
			return{ false, "" };
		}

		// LoadData
		class LoadData
		{
		private:
			static bool isState0(const long long state_reserve)
			{
				return 1 == state_reserve;
			}
			/// core
		public:
			template <class Reserver>
			static bool _LoadData(ARRAY_QUEUE<Token>& strVec, Reserver& reserver, UserType& global, const wiz2::LoadDataOption& option) // first, strVec.empty() must be true!!
			{
				int state = 0;
				int braceNum = 0;
				long long state_reserve = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var1, var2, val;

				nestedUT[0] = &global;
				{
					reserver(strVec, option);

					while (strVec.empty())
					{
						reserver(strVec, option);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				//	for (int i = 0; i < strVec.size(); ++i) {
				//		cout << strVec[i].str << endl;
				//	}

				int sum = 0;
				int a = clock();
				int b;

				while (false == strVec.empty()) {

					//	cout << state << " " << Top(strVec, nestedUT[braceNum], reserver, option) << endl;

					switch (state)
					{
					case 0:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							state = 2;
						}
						else {
							std::pair<bool, std::string> bsPair = LookUp(strVec, nestedUT[braceNum], reserver, option);
							if (bsPair.first) {
								if (bsPair.second.size() == 1 && -1 != Equal(option.Assignment, bsPair.second[0])) {
									Pop(strVec, &var2, nestedUT[braceNum], reserver, option);
									Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
									state = 2;
								}
								else {
									if (Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										state = 0;
									}
								}
							}
							else {
								if (Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
									nestedUT[braceNum]->AddItem("", move(var1));
									state = 0;
								}
							}
						}
					}
					break;
					case 1:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							state = 0;
						}
						else {
							// syntax error.
							throw "syntax error 1 ";
						}
					}
					break;
					case 2:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							///
							nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							state = 3;
						}
						else {
							if (Pop(strVec, &val, nestedUT[braceNum], reserver, option)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = "";
								val = "";

								state = 0;
							}
						}
					}
					break;
					case 3:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							nestedUT[braceNum] = nullptr;
							braceNum--;

							state = 0;
						}
						else {
							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw  "syntax error 2 ";
							}
						}
					}
					break;
					case 4:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							//}

							state = 5;
						}
						else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							state = isState0(state_reserve) ? 0 : 4;
							state_reserve--;

							{
								nestedUT[braceNum] = nullptr;
								braceNum--;
							}
						}
						else {
							std::pair<bool, std::string> bsPair = LookUp(strVec, nestedUT[braceNum], reserver, option);
							if (bsPair.first) {
								if (bsPair.second.size() == 1 && -1 != Equal(option.Assignment, bsPair.second[0])) {
									// var2
									Pop(strVec, &var2, nestedUT[braceNum], reserver, option);
									Pop(strVec, nullptr, nestedUT[braceNum], reserver, option); // pass EQ_STR
									state = 6;
								}
								else {
									// var1
									if (Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										var1 = "";

										state = 4;
									}
								}
							}
							else
							{
								// var1
								if (Pop(strVec, &var1, nestedUT[braceNum], reserver, option))
								{
									nestedUT[braceNum]->AddItem("", move(var1));
									var1 = "";

									state = 4;
								}
							}
						}
					}
					break;
					case 5:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							//if (flag1 == 0) {
							nestedUT[braceNum] = nullptr;
							braceNum--;
							// }
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;


							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw "syntax error 4  ";
							}
						}
					}
					break;
					case 6:
					{
						std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
								var2 = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) /// changed 2014.01.23..
									nestedUT.push_back(nullptr);

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 7;
						}
						else {
							if (Pop(strVec, &val, nestedUT[braceNum], reserver, option)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = ""; val = "";

								top = Top(strVec, nestedUT[braceNum], reserver, option);

								if (strVec.empty())
								{
									//
								}
								else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
									Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

									{
										state = isState0(state_reserve) ? 0 : 4;
										state_reserve--;

										{
											nestedUT[braceNum] = nullptr;
											braceNum--;
										}
									}
									{
										//state = 4;
									}
								}
								else {
									state = 4;
								}
							}
						}
					}
					break;
					case 7:
					{
						const std::string top = Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							//

							nestedUT[braceNum] = nullptr;
							braceNum--;
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;

							{
								/// uisng struct
								state_reserve++;

								state = 4;
							}
							//else
							{
								//throw "syntax error 5 ";
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}

					if (strVec.size() < 10) {
						b = clock();
						sum += b - a;
						//std::cout << b - a << "ms" << std::endl;

						reserver(strVec, option);

						while (strVec.empty()) // (strVec.empty())
						{
							reserver(strVec, option);
							if (
								strVec.empty() &&
								reserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
						a = clock();
					}
				}
				if (state != 0) {
					throw std::string("error final state is not 0!  : ") + toStr(state);
				}
				if (braceNum != 0) {
					throw std::string("chk braceNum is ") + toStr(braceNum);
				}
				b = clock();

				//std::cout << sum << "ms" << std::endl;
				return true;
			}

		private:
			static bool _Empty(std::vector<ARRAY_QUEUE<Token>> & strVec) // chk!!
			{
				for (int i = 0; i < strVec.size(); ++i) {
					if (strVec[i].empty()) {
						continue;
					}
					else {
						return false;
					}
				}

				return true;
			}
			static int _Size(std::vector<ARRAY_QUEUE<Token>> & strVec) // chk!!
			{
				int sum = 0;

				for (int i = 0; i < strVec.size(); ++i) {
					bool isEmpty = false;
					for (int j = 0; j < strVec[i].size(); ++j) {
						if (strVec[i][j].str == "") {
							isEmpty = true;
						}
						else {
							isEmpty = false;
							break;
						}
					}
					if (isEmpty == false) {
						sum = sum + strVec[i].size();
					}
				}

				return sum;
			}
		public:
			// line comment -> accept but no save!
			// no multiple comment!
			static bool __LoadData3_2(VECTOR<Token2> * _strVec, long long start_idx, long long last_idx, UserType * _global, const wiz2::LoadDataOption * _option,
				int start_state, int last_state, UserType * *next) // first, strVec.empty() must be true!!
			{
				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz2::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				//long long state_reserve = 0;

				std::vector< UserType* > nestedUT(1);
				std::string var1, var2, val;

				bool varOn = false;
				nestedUT[0] = &global;

				//	for (int i = 0; i < strVec.size(); ++i) {
				//		cout << strVec[i].str << endl;
				//	}
				long long i = start_idx;
				bool end_state = false;
				while (i <= last_idx && !end_state) {
					//std::cout << state << " ";
					//	cout << state << " " << Top(strVec, nestedUT[braceNum], reserver, option) << endl;

					switch (state)
					{
					case 0:
					{
						while (i <= last_idx && strVec[i].isComment) { // remove this part? has bug!

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								state = 2;
							}
							else {
								std::pair<bool, Token2> bsPair;

								int tmp = i;
								++i;

								// remove?
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//
								}

								if (!end_state) {
									if (i <= last_idx) {
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = tmp;

									if (bsPair.first) {
										if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
											var2 = std::string(strVec[i].str, strVec[i].len);// Pop(strVec, &var2, nestedUT[braceNum], option);
																//Pop(strVec, nullptr, nestedUT[braceNum], option);
											state = 2;
											i++;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											i++;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}

										}
										else {
											if (i <= last_idx) {
												//Pop(strVec, &var1, nestedUT[braceNum], option)) {

												var1 = std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", var1);
												state = 0;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {

													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//
												}
											}
										}
									}
									else {
										if (i <= last_idx) {
											//Pop(strVec, &var1, nestedUT[braceNum], option)) {
											var1 = std::string(strVec[i].str, strVec[i].len);

											nestedUT[braceNum]->AddItem("", var1);
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											state = 0;
										}
									}
								}
							}
						}
					}
					break;
					case 1:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								state = 0;
							}
							else {
								// syntax error.
								throw "syntax error 1 ";
							}
						}
					}
					break;
					case 2:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
													//break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}
								if (!end_state) {
									///
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									state = 3;
								}
							}
							else {
								if (i <= last_idx) {
									//Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = std::string(strVec[i].str, strVec[i].len);

									nestedUT[braceNum]->AddItem(var2, val);
									i += 1;
									while (i <= last_idx && strVec[i].isComment) {

										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										var2.clear();
										val.clear();

										state = 0;
									}
								}
							}
						}
					}
					break;
					case 3:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//	break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#")); // option->LineComment[0]?
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));


											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;

								state = 0;
							}
							else {
								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw  "syntax error 2 ";
								}
							}
						}
					}
					break;
					case 4:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
													//	break;
						}
						if (!end_state) {
							auto top = strVec[i];

							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									UserType temp("");

									nestedUT[braceNum]->AddUserTypeItem(temp);
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									//}


									state = 5;
								}
							}
							else if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									state = 4;
									//state = isState0(state_reserve) ? 0 : 4;
									//state_reserve--;


									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}

										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

										braceNum++;
									}

									{
										if (braceNum < nestedUT.size()) {
											nestedUT[braceNum] = nullptr;
										}
										braceNum--;
									}
								}
							}
							else {
								int temp = i;
								++i;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									std::pair<bool, Token2> bsPair;

									if (i <= last_idx)
									{
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = temp;

									if (bsPair.first) {
										if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
											// var2
											//Pop(strVec, &var2, nestedUT[braceNum], option);
											//Pop(strVec, nullptr, nestedUT[braceNum], option); // pass EQ_STR
											var2 = std::string(strVec[i].str, strVec[i].len);
											state = 6;
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
										}
										else {
											// var1
											if (i <= last_idx) {
												//Pop(strVec, &var1, nestedUT[braceNum], option)) {
												var1 = std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", var1);
												var1.clear();// = "";

												state = 4;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {

													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//	break;
												}
											}
										}
									}
								}
								else
								{
									// var1
									if (i <= last_idx) //Pop(strVec, &var1, nestedUT[braceNum], option))
									{
										var1 = std::string(strVec[i].str, strVec[i].len);
										nestedUT[braceNum]->AddItem("", var1);
										var1.clear();

										state = 4;
										i += 1;
										while (i <= last_idx && strVec[i].isComment) {

											++i;
										}
										if (i >= last_idx + 1) {
											//end_state = true;
											//	break;
										}
									}
								}
							}
						}
					}
					break;
					case 5:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#"));
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
											//

											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								//if (flag1 == 0) {
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
								// }
								//
								state = 4;
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
							}
							else {
								int idx = -1;
								int num = -1;


								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw "syntax error 4  ";
								}
							}
						}
					}
					break;
					case 6:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							std::string top = std::string(strVec[i].str, strVec[i].len);

							if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								if (!end_state) {
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
									var2.clear();
									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;


								}
								///
								state = 7;
							}
							else {
								if (i <= last_idx) { //Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = std::string(strVec[i].str, strVec[i].len);

									i += 1;
									while (i <= last_idx && strVec[i].isComment) {

										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										nestedUT[braceNum]->AddItem(var2, val);
										var2.clear(); val.clear();

										top = std::string(strVec[i].str, strVec[i].len);

										if (strVec.empty())
										{
											//
										}
										else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
											//Pop(strVec, nullptr, nestedUT[braceNum], option);
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											if (!end_state) {
												if (braceNum == 0) {
													int utCount = 0;
													int itCount = 0;
													UserType ut;
													ut.AddUserTypeItem(UserType("#"));
													UserType* pTemp = nullptr;
													ut.GetLastUserTypeItemRef("#", pTemp);

													/// move data
													ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
													ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
													ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

													for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
														if (nestedUT[braceNum]->IsUserTypeList(k)) {
															ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
															//	//


															utCount++;
														}
														else {
															ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
															itCount++;
														}
													}
													nestedUT[braceNum]->Remove();
													nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


													braceNum++;
												}
											}
											{
												state = 4;
												//state = isState0(state_reserve) ? 0 : 4;
												//state_reserve--;

												{
													if (braceNum < nestedUT.size()) {
														nestedUT[braceNum] = nullptr;
													}
													braceNum--;
												}
											}
											{
												//state = 4;
											}
										}
										else {
											state = 4;
										}
									}
								}
							}
						}
					}
					break;
					case 7:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Pop(strVec, nullptr, nestedUT[braceNum], option);
								//
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}

								if (!end_state) {
									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
												//

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}
										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


										braceNum++;
									}

									if (braceNum < nestedUT.size()) {
										nestedUT[braceNum] = nullptr;
									}
									braceNum--;
									//
									state = 4;
								}
							}
							else {
								int idx = -1;
								int num = -1;

								{
									/// uisng struct
									//state_reserve++;

									state = 4;
								}
								//else
								{
									//throw "syntax error 5 ";
								}
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}
				if (state != last_state) { // 0 : empty or 4 : else?
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}

			template <class Reserver>
			static bool _LoadData3_2(VECTOR<Token2> & strVec, Reserver & reserver, UserType & global, const wiz2::LoadDataOption & option, const int pivot_num, const int lex_thr_num) // first, strVec.empty() must be true!!
			{
				char left[] = "{";
				char right[] = "}";

				strVec.emplace_back(left, 1, false);
				strVec.emplace_back(left, 1, false);

				char* buffer = nullptr;
				bool end = false;
				{
					end = !reserver(&strVec, option, lex_thr_num, buffer);

					while (strVec.empty())
					{
						end = !reserver(&strVec, option, lex_thr_num, buffer);
						if (
							strVec.empty() &&
							end
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				UserType* before_next = nullptr;
				UserType _global;

				bool first = true;
				int sum = 0;

				while (true) {
					end = !reserver(&strVec, option, lex_thr_num, buffer);

					if (end) {
						strVec.emplace_back(right, 1, false);
						strVec.emplace_back(right, 1, false);
					}

					int a = clock();


					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = FindRight(strVec, 0, strVec.size() - 1, option);

					if (pivot_num > 0) {
						//int c1 = clock();
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						//std::cout << _pivots.size() << std::endl;
						for (auto& x : _pivots) {
							pivots.push_back(x);
							//	std::cout << x << " ";
						}
						//std::cout << last_idx << std::endl;
						//std::cout << std::endl;
					}


					//std::cout << pivot << "\t" << pivot3 << "\t" << pivot2 << "\t" << last_idx << "\t" << strVec.size() - 1 << std::endl;
					//std::cout << strVec[pivot + 1].str << "\t";
					//std::cout << strVec[pivot3 + 1].str << "\t";
					//std::cout << strVec[pivot2 + 1].str << "\t";
					//std::cout << std::endl;


					std::vector<UserType*> next(pivots.size() + 1, nullptr);

					{
						std::vector<UserType> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);


						if (first) {
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData3_2, &strVec, 0, idx, &__global[0], &option, 0, 4, &next[0]);
						}
						else {
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData3_2, &strVec, 0, idx, &__global[0], &option, 4, 4, &next[0]);
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData3_2, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 4, 4, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData3_2, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 4, 4, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}

						//Merge
						//int merge_start = clock();

						if (first) {
							Merge(&_global, &__global[0], &next[0]);
							first = false;
						}
						else {
							Merge(before_next, &__global[0], &next[0]);
						}

						for (int i = 1; i < pivots.size() + 1; ++i) {
							Merge(next[i - 1], &__global[i], &next[i]);
						}

						//std::cout << "merge " << clock() - merge_start << "ms" << std::endl;

						//int pop_start = clock();
						//
						//for (int i = 0; i <= last_idx; ++i) {
						//	strVec.pop_front();
						//}

						strVec.erase(strVec.begin(), strVec.begin() + last_idx + 1);

						//std::cout << "pop " << clock() - pop_start << "ms" << std::endl;

						before_next = next.back();
					}

					int b = clock();
					sum += b - a; //std::cout << b - a << "ms" << std::endl;

					if (!end) {

						end = !reserver(&strVec, option, lex_thr_num, buffer);

						while (!end && strVec.empty())
						{
							end = !reserver(&strVec, option, lex_thr_num, buffer);
							if (
								strVec.empty() &&
								end
								) {
								break; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
							}
						}
					}
					else {
						break;
					}
				}

				global = std::move(*_global.GetUserTypeList(0)->GetUserTypeList(0));
				//
				delete[] buffer;

				//// todo - option?
				//std::cout << "lexing " << log_result << "ms" << std::endl;
				//				log_result = 0;
				//std::cout << "parse " << sum << "ms" << std::endl;
				return true;
			}

		private:
			static int FindRight(VECTOR<Token2> & strVec, int start, int last, const wiz2::LoadDataOption & option)
			{
				for (int i = last; i >= start; --i) {
					if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
						return i;
					}
				}

				return -1;
			}

			static void Merge(UserType * next, UserType * ut, UserType * *ut_next)
			{
				//check!!
				while (ut->GetIListSize() >= 1 && ut->GetUserTypeListSize() >= 1
					&& (ut->GetUserTypeList(0)->GetName() == "##" ||
						ut->GetUserTypeList(0)->GetName() == "#"))
				{
					ut = ut->GetUserTypeList(0);
				}

				//int chk = 0;
				while (true) {
					int itCount = 0;
					int utCount = 0;

					UserType* _ut = ut;
					UserType* _next = next;

					if (ut_next && _ut == *ut_next) {
						*ut_next = _next;
					}

					for (int i = 0; i < _ut->GetIListSize(); ++i) {
						if (_ut->IsUserTypeList(i)) {
							if (_ut->GetUserTypeList(utCount)->GetName() == "#" ||
								_ut->GetUserTypeList(utCount)->GetName() == "##") {
								_ut->GetUserTypeList(utCount)->SetName("");
							}
							else {
								{
									_next->LinkUserType(_ut->GetUserTypeList(utCount));
									_ut->GetUserTypeList(utCount) = nullptr;
								}
							}
							utCount++;
						}
						else if (_ut->IsItemList(i)) {
							_next->AddItemList(std::move(_ut->GetItemList(itCount)));
							itCount++;
						}
					}
					_ut->Remove();
					if (ut->GetParent() && next->GetParent()) {
						ut = ut->GetParent();
						next = next->GetParent();
					}
					else {
						break;
					}
				}

			}

		public:
			static bool __LoadData5(VECTOR<Token2> * _strVec, int start_idx, int last_idx, UserType * _global, const wiz2::LoadDataOption * _option,
				int start_state, int last_state, UserType * *next) // first, strVec.empty() must be true!!
			{
				std::vector<std::string> varVec;
				std::vector<std::string> valVec;

				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz2::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var, val;


				nestedUT[0] = &global;

				int i = start_idx;

				while (false == strVec.empty() && i <= last_idx) {
					switch (state)
					{
					case 0:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;

							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(std::move(varVec[i]), std::move(valVec[i]));
							}
							varVec.clear();
							valVec.clear();


							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) { /// changed 2014.01.23..
								nestedUT.push_back(nullptr);
							}

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///

							state = 0;
						}
						else if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
							i += 1;

							state = 0;

							// UserTypeListsize?
							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(std::move(varVec[i]), std::move(valVec[i]));
							}
							varVec.clear();
							valVec.clear();

							if (braceNum == 0) {
								UserType ut;
								ut.AddUserTypeItem(UserType("#")); // json -> "var_name" = val  // clautext, # is line comment delimiter.
								UserType* pTemp = nullptr;
								ut.GetLastUserTypeItemRef("#", pTemp);
								int utCount = 0;
								int itCount = 0;
								auto max = nestedUT[braceNum]->GetIListSize();
								for (auto i = 0; i < max; ++i) {
									if (nestedUT[braceNum]->IsUserTypeList(i)) {
										ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*(nestedUT[braceNum]->GetUserTypeList(utCount))));
										utCount++;
									}
									else {
										ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
										itCount++;
									}
								}

								nestedUT[braceNum]->Remove();
								nestedUT[braceNum]->AddUserTypeItem(std::move(*(ut.GetUserTypeList(0))));

								braceNum++;
							}

							{
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
							}
						}
						else {
							std::pair<bool, Token2> bsPair;

							if (i < last_idx)
							{
								bsPair = std::make_pair(true, strVec[i + 1]);
							}
							else {
								bsPair = std::make_pair(false, Token2());
							}

							if (bsPair.first) {
								if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
									// var2
									var = std::string(strVec[i].str, strVec[i].len);
									state = 1;
									i += 1;
									i += 1;
								}
								else {
									// var1
									if (i <= last_idx) {
										val = std::string(strVec[i].str, strVec[i].len);

										varVec.push_back(var);
										valVec.push_back(val);
										//nestedUT[braceNum]->AddItem("", ""); // std::move(val));
										val = "";

										state = 0;
										i += 1;
									}
								}
							}
							else
							{
								// var1
								if (i <= last_idx)
								{
									val = std::string(strVec[i].str, strVec[i].len);
									varVec.push_back(var);
									valVec.push_back(val);
									//nestedUT[braceNum]->AddItem("", "");// std::move(val));
									val = "";

									state = 0;
									i += 1;
								}
							}
						}
					}
					break;
					case 1:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;
							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(std::move(varVec[i]), std::move(valVec[i]));
							}
							varVec.clear();
							valVec.clear();


							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var, pTemp);
								var = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) {
									nestedUT.push_back(nullptr);
								}

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 0;
						}
						else {
							if (i <= last_idx) {
								val = std::string(strVec[i].str, strVec[i].len);

								i += 1;
								varVec.push_back(var);
								valVec.push_back(val);
								//nestedUT[braceNum]->AddItem("", ""); // std::move(var), std::move(val));
								var = ""; val = "";

								state = 0;
							}
						}
					}
					break;
					default:
						// syntax err!!
						throw "syntax error ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}

				if (varVec.empty() == false) {
					nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
					nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

					for (int i = 0; i < varVec.size(); ++i) {
						nestedUT[braceNum]->AddItem(varVec[i], valVec[i]);
					}
					varVec.clear();
					valVec.clear();
				}

				if (state != last_state) {
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}
			
			class Node { // all node has first
			public:
				wiz2::Token2 var;
				wiz2::Token2 val;
				Node* next = nullptr; //
				Node* first = nullptr; //
				Node* last = nullptr;  // only first node has parent, last.
				Node* parent = nullptr; // only first node has parent, last.
				Node* first_child = nullptr;
				bool is_ut = true; // false -> itemtype, true -> usertype.
				bool is_start = true;
				int size = 0;
				int ut_size = 0;
			};
			
			static Node* NewNode() {
				return new Node();
			}
			static Node* Parent(Node* x) {
				if (x && x->first) {
					return x->first->parent;
				}
				return nullptr;
			}

			static bool AddItem(Node* ut, wiz2::Token2 var, wiz2::Token2 val) {
				if (!ut || !ut->is_ut) {
					return false;
				}
				if (!ut->first_child) {
					Node* temp = NewNode();
					temp->var = var;
					temp->val = val;
					temp->parent = ut;
					temp->first = temp;
					temp->last = temp;
					temp->is_ut = false;
					temp->is_start = true;
					ut->first_child = temp;
					ut->first_child->last = temp;
				}
				else {
					Node* temp = NewNode();
					temp->var = var;
					temp->val = val;
					//
					temp->first = ut->first_child;
					temp->is_ut = false;
					temp->is_start = false;
					ut->first_child->last->next = temp;
					ut->first_child->last = temp;
				}
				ut->size++;
				return true;
			}
			static bool AddItem(Node* ut, Node* it) {
				if (!ut || !ut->is_ut) {
					return false;
				}
				it->next = nullptr;
				if (!ut->first_child) {
					Node* temp = it;
					temp->parent = ut;
					temp->first = temp;
					temp->last = temp;
					temp->is_ut = false;
					temp->is_start = true;
					ut->first_child = temp;
					ut->first_child->last = temp;
				}
				else {
					Node* temp = it;
					//
					temp->first = ut->first_child;
					temp->is_ut = false;
					temp->is_start = false;
					ut->first_child->last->next = temp;
					ut->first_child->last = temp;
				}
				ut->size++;
				return true;
			}
			static bool AddUserType(Node* ut, wiz2::Token2 var) {
				if (!ut || !ut->is_ut) {
					return false;
				}
				if (!ut->first_child) {
					Node* temp = NewNode();
					temp->var = var;
					temp->parent = ut;
					temp->first = temp;
					temp->last = temp;
					temp->is_ut = true;
					temp->is_start = true;
					ut->first_child = temp;
					ut->first_child->last = temp;
				}
				else {
					Node* temp = NewNode();
					temp->var = var;
					//
					temp->first = ut->first_child;
					temp->is_ut = true;
					temp->is_start = false;
					ut->first_child->last->next = temp;
					ut->first_child->last = temp;
				}
				ut->size++;
				ut->ut_size++;
				return true;
			}
			static bool AddUserType(Node* ut, Node* other_ut) {
				if (!ut || !ut->is_ut || !other_ut->is_ut) {
					return false;
				}
				other_ut->next = nullptr;
				
				if (!ut->first_child) {
					other_ut->first = other_ut;
					ut->first_child = other_ut;
					ut->first_child->last = other_ut;
				}
				else {
					other_ut->first = ut->first_child;
					ut->first_child->last->next = other_ut;
					ut->first_child->last = other_ut;
				}
				ut->size++;
				ut->ut_size++;
				other_ut->parent = ut;
				other_ut->next = nullptr;
				return true;
			}

			static void _RemoveAll(Node* node) {
				if (node == nullptr) { return; }
				_RemoveAll(node->first_child);
				_RemoveAll(node->next);
				delete node;
			}
			static void RemoveAll(Node* node) { // node inner remove all
				if (node) {
					_RemoveAll(node->first_child);
					node->size = 0;
					node->ut_size = 0;
					node->first_child = nullptr;
				}
			}
			static void Clear(Node* node) {
				if (node) {
					node->size = 0;
					node->ut_size = 0;
					node->first_child = nullptr;
					node->first = nullptr;
					node->last = nullptr;
					node->next = nullptr;
				}
			}

			static void Print(Node* node, std::ostream& out) {
				if (nullptr == node) {
					return;
				}
				if (node->is_ut) {
					if (node->var.len == 0) {
						out << " { \n";
					}
					else {
						out << std::string(node->var.str, node->var.len) << " = { \n";
					}
				}
				else {
					if (node->var.len == 0) {
						out << std::string(node->val.str, node->val.len) << " ";
					}
					else {
						out << std::string(node->var.str, node->var.len) << " = ";
						out << std::string(node->val.str, node->val.len) << "\n";
					}
				}
				Print(node->first_child, out);
				if (node->is_ut) {
					out << " }\n";
				}
				Print(node->next, out);
			}
			static void Merge2(Node* next, Node* ut, Node** ut_next)
			{
				static Token2 EmptyName;
				EmptyName.str = nullptr;
				EmptyName.len = 0;
				//check!!
				while (ut->size > 0 && ut->ut_size > 0//ut->GetIListSize() >= 1 && ut->GetUserTypeListSize() >= 1
					&& ( ut->first_child->var == "##" || //ut->GetUserTypeList(0)->GetName() == "##" ||
						ut->first_child->var == "#" )) //ut->GetUserTypeList(0)->GetName() == "#"))
				{
					ut = ut->first_child;
				}

				//int chk = 0;
				while (true) {
					Node* _ut = ut;
					Node* _next = next;

					if (ut_next && _ut == *ut_next) {
						*ut_next = _next;
					}

					Node* iter = _ut->first_child;
					if (iter) {
						Node* last = iter->last;
						Node* next_iter = nullptr;

						for (; iter != nullptr; iter = next_iter) {
							next_iter = iter->next;
							if (iter->is_ut) {
								if (iter->var == "#" ||
									iter->var == "##") {
									iter->var = EmptyName;
								}
								else {
									{
										AddUserType(_next, iter);
										//LinkUserType(_next, iter);
									}
								}
							}
							else {
								AddItem(_next, iter);
								//_next->AddItemList(std::move(_ut->GetItemList(itCount)));
							}
						}
					}
					
					//_ut->Remove();

					if (Parent(ut) && Parent(next)) {
						ut = Parent(ut);
						next = Parent(next);
					}
					else {
						break;
					}
				}
			}

			static bool __LoadData6(VECTOR<wiz2::Token2>* _strVec, int start_idx, int last_idx, Node* _global, const wiz2::LoadDataOption* _option,
				int start_state, int last_state, Node** next) // first, strVec.empty() must be true!!
			{
				static wiz2::Token2 EmptyToken;
				EmptyToken.str = nullptr;
				EmptyToken.len = 0;
				static wiz2::Token2 SharpToken;
				static char sharp = '#';
				SharpToken.str = &sharp;
				SharpToken.len = 1;

				std::vector<wiz2::Token2> varVec;
				std::vector<wiz2::Token2> valVec;

				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<wiz2::Token2>& strVec = *_strVec;
				Node& global = *_global;
				const wiz2::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				std::vector< Node* > nestedUT(1);
				wiz2::Token2 var = EmptyToken, val = EmptyToken;


				nestedUT[0] = &global;

				int i = start_idx;

				while (false == strVec.empty() && i <= last_idx) {
					switch (state)
					{
					case 0:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;

							for (int i = 0; i < varVec.size(); ++i) {
								AddItem(nestedUT[braceNum], varVec[i], valVec[i]);
							}
							varVec.clear();
							valVec.clear();

							AddUserType(nestedUT[braceNum], EmptyToken);
							Node* pTemp = nullptr;

							pTemp = nestedUT[braceNum]->first_child->last;
							//nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) { /// changed 2014.01.23..
								nestedUT.push_back(nullptr);
							}

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///

							state = 0;
						}
						else if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
							i += 1;

							state = 0;

							for (int i = 0; i < varVec.size(); ++i) {
								AddItem(nestedUT[braceNum], varVec[i], valVec[i]);
							}
							varVec.clear();
							valVec.clear();

							if (braceNum == 0) {
								Node ut;
								AddUserType(&ut, SharpToken); // json -> "var_name" = val  // clautext, # is line comment delimiter.
								Node* pTemp = nullptr;
								pTemp = ut.first_child->last;
								//ut.GetLastUserTypeItemRef("#", pTemp);
								
								Node* iter = nestedUT[braceNum]->first_child;
								Node* last = iter->last;
								Node* next_iter = iter->next;
								for (; iter != nullptr; iter = next_iter) {
									next_iter = iter->next;

									if (iter->is_ut) {
										AddUserType(ut.first_child, iter);
										//ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*(nestedUT[braceNum]->GetUserTypeList(utCount))));
									}
									else {
										AddItem(ut.first_child, iter);
									}
								}

								nestedUT[braceNum]->first_child = nullptr;
								nestedUT[braceNum]->size = 0;
								nestedUT[braceNum]->ut_size = 0;
								//nestedUT[braceNum]->Remove();
								
								AddUserType(nestedUT[braceNum], ut.first_child);
								//nestedUT[braceNum]->AddUserTypeItem(std::move(*(ut.GetUserTypeList(0))));

								braceNum++;
							}

							{
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
							}
						}
						else {
							std::pair<bool, Token2> bsPair;

							if (i < last_idx)
							{
								bsPair = std::make_pair(true, strVec[i + 1]);
							}
							else {
								bsPair = std::make_pair(false, Token2());
							}

							if (bsPair.first) {
								if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
									// var2
									var = strVec[i];
									state = 1;
									i += 1;
									i += 1;
								}
								else {
									// var1
									if (i <= last_idx) {
										val = strVec[i];

										varVec.push_back(var);
										valVec.push_back(val);
										//nestedUT[braceNum]->AddItem("", ""); // std::move(val));
										val = EmptyToken; // "";
										var = EmptyToken;
										state = 0;
										i += 1;
									}
								}
							}
							else
							{
								// var1
								if (i <= last_idx)
								{
									val = strVec[i];
									varVec.push_back(var);
									valVec.push_back(val);
									//nestedUT[braceNum]->AddItem("", "");// std::move(val));
									var = EmptyToken;
									val = EmptyToken;

									state = 0;
									i += 1;
								}
							}
						}
					}
					break;
					case 1:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;
							
							for (int i = 0; i < varVec.size(); ++i) {
								AddItem(nestedUT[braceNum], varVec[i], valVec[i]);
								//nestedUT[braceNum]->AddItem(std::move(varVec[i]), std::move(valVec[i]));
							}
							varVec.clear();
							valVec.clear();


							///
							{
								AddUserType(nestedUT[braceNum], var);
								Node* pTemp = nullptr;
								pTemp = nestedUT[braceNum]->first_child->last;
								//nestedUT[braceNum]->GetLastUserTypeItemRef(var, pTemp);
								var = EmptyToken; // "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) {
									nestedUT.push_back(nullptr);
								}

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 0;
						}
						else {
							if (i <= last_idx) {
								val = strVec[i]; // std::string(strVec[i].str, strVec[i].len);

								i += 1;
								varVec.push_back(var);
								valVec.push_back(val);
								//nestedUT[braceNum]->AddItem("", ""); // std::move(var), std::move(val));
								var = EmptyToken; val = EmptyToken;

								state = 0;
							}
						}
					}
					break;
					default:
						// syntax err!!
						throw "syntax error ";
						break;
					}
				}


				if (varVec.empty() == false) {
					for (int i = 0; i < varVec.size(); ++i) {
						AddItem(nestedUT[braceNum], varVec[i], valVec[i]);
					}
					varVec.clear();
					valVec.clear();
				}
				
				if (next) {
					*next = nestedUT[braceNum];
				}

				if (state != last_state) {
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}

		private:
			// [must] option.Assignment[i].size() == 1  - chk!
			static int FindRight2(VECTOR<Token2> & strVec, int start, int last, const wiz2::LoadDataOption & option)
			{
				for (int i = last; i >= start; --i) {
					if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
						return i;
					}

					if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
						return i;
					}
					else if (strVec[i].len == 1 && -1 != Equal(option.Assignment, strVec[i].str[0])) {
						//
					}
					else if (i < last && !(strVec[i + 1].len == 1 && -1 != Equal(option.Assignment, strVec[i + 1].str[0])))
					{                // NOT
						return i;
					}
				}

				return -1;
			}

		public:
			//// ToDo : # => option.~?
			template <class Reserver>
			static bool _LoadData5(VECTOR<Token2> & strVec, Reserver & reserver, UserType & global, const wiz2::LoadDataOption & option, const int lex_thr_num, const int parse_num) // first, strVec.empty() must be true!!
			{
				const int pivot_num = parse_num - 1;
				char* buffer = nullptr;

				bool end = false;
				{
					int a = clock();
					end = !reserver(&strVec, option, lex_thr_num, buffer);
					int b = clock();
					std::cout << b - a << "ms ";
				}

				UserType* before_next = nullptr;
				UserType _global;

				bool first = true;
				long long sum = 0;
int a, c;
				while (true) {
					end = true;

					
					a = clock();
					
					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = strVec.size() - 1;

					if (pivot_num > 0) {
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight2(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						for (auto& x : _pivots) {
							pivots.push_back(x);
						}
					}

					std::vector<UserType*> next(pivots.size() + 1, nullptr);

					{
						std::vector<UserType> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);

						{
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData5, &strVec, 0, idx, &__global[0], &option, 0, 0, &next[0]);
							// __LoadData4 -> __LoadData5
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData5, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 0, 0, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData5, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 0, 0, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}
						c = clock();
						// Merge
						try {
							if (first) {
								Merge(&_global, &__global[0], &next[0]);
								first = false;
							}
							else {
								Merge(before_next, &__global[0], &next[0]);
							}

							for (int i = 1; i < pivots.size() + 1; ++i) {
								Merge(next[i - 1], &__global[i], &next[i]);
							}

						}
						catch (...) {
							delete[] buffer;
							buffer = nullptr;
							throw "in Merge, error";
						}
						{
							if (next[pivots.size()] != nullptr && next[pivots.size()]->GetParent() != nullptr) {
								delete[] buffer;
								buffer = nullptr;
								throw "merge error";
							}
						}

						strVec.erase(strVec.begin(), strVec.begin() + (last_idx + 1));

						before_next = next.back();
					}

					
					if (!end) {
						//
					}
					else {
						break;
					}
				}
int b = clock();
				delete[] buffer;

				global = std::move(_global);
				std::cout << b - a << "ms" << " " << b - c << "ms" << "\n";
					

				return true;
			}
			template <class Reserver>
			static bool _LoadData6(VECTOR<Token2>& strVec, Reserver& reserver, Node& global, const wiz2::LoadDataOption& option, const int lex_thr_num, const int parse_num, char** out_buffer) // first, strVec.empty() must be true!!
			{
				const int pivot_num = parse_num - 1;
				char* buffer = nullptr;

				bool end = false;
				{
					int a = clock();
					end = !reserver(&strVec, option, lex_thr_num, buffer);
					int b = clock();
					std::cout << b - a << "ms ";
				}

				Node* before_next = nullptr;
				Node _global;

				bool first = true;
				long long sum = 0;

				while (true) {
					end = true;

					int a = clock();
					int c;

					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = strVec.size() - 1;

					if (pivot_num > 0) {
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight2(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						for (auto& x : _pivots) {
							pivots.push_back(x);
						}
					}

					std::vector<Node*> next(pivots.size() + 1, nullptr);

					{
						std::vector<Node> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);

						{
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData6, &strVec, 0, idx, &__global[0], &option, 0, 0, &next[0]);
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData6, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 0, 0, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData6, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 0, 0, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}
					
						c = clock();
						// Merge
						try 
						{
							if (first) {
								Merge2(&_global, &__global[0], &next[0]);
								first = false;
							}
							else {
								Merge2(before_next, &__global[0], &next[0]);
							}

							for (int i = 1; i < pivots.size() + 1; ++i) {
								Merge2(next[i - 1], &__global[i], &next[i]);
							}

						}
						catch (...) {
							delete[] buffer;
							buffer = nullptr;
							throw "in Merge, error";
						}
						{
							if (next[pivots.size()] != nullptr && Parent(next[pivots.size()]) != nullptr) {
								delete[] buffer;
								buffer = nullptr;
								throw "merge error";
							}
						}

						strVec.erase(strVec.begin(), strVec.begin() + (last_idx + 1));

						before_next = next.back();
					}

					int b = clock();
					std::cout << b - a << "ms";
					std::cout << " " << b - c << "ms\n";
					if (!end) {
						//
					}
					else {
						break;
					}
				}

				//delete[] buffer;
				*out_buffer = buffer;

				global = _global;

				return true;
			}

			static bool LoadDataFromFile(const std::string & fileName, UserType & global, int pivot_num = -1, int lex_thr_num = 0) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = 1;
				}


				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp;
				static VECTOR<Token2> strVec;

				strVec.clear();

				try {
					InFileReserver3 ifReserver(inFile);
					wiz2::LoadDataOption option;
					option.Assignment.push_back('=');
					option.Left.push_back('{');
					option.Right.push_back('}');
					option.LineComment.push_back("#");

					ifReserver.Num = 1 << 19;
					//	strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData5(strVec, ifReserver, globalTemp, option, lex_thr_num, pivot_num + 1))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string & e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (std::exception e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				global = std::move(globalTemp);
				return true;
			}	
			
			static bool LoadDataFromFile2(const std::string& fileName, Node& global, int pivot_num, int lex_thr_num, char** _buffer) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = 1;
				}


				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				Node globalTemp;
				static VECTOR<Token2> strVec;

				strVec.clear();

				try {
					InFileReserver3 ifReserver(inFile);
					wiz2::LoadDataOption option;
					option.Assignment.push_back('=');
					option.Left.push_back('{');
					option.Right.push_back('}');
					option.LineComment.push_back("#");

					char* buffer = nullptr;
					ifReserver.Num = 1 << 19;
					//	strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData6(strVec, ifReserver, globalTemp, option, lex_thr_num, pivot_num + 1, _buffer))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string & e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (std::exception e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				
				global = globalTemp;
				
				return true;
			}

		public:
			static bool LoadDataFromString(const std::string & str, UserType & ut)
			{

				UserType utTemp = ut;
				ARRAY_QUEUE<Token> strVec;

				wiz2::StringBuilder builder(str.size(), str.c_str(), str.size());

				wiz2::LoadDataOption option;
				option.Assignment.push_back('=');
				option.Left.push_back('{');

				//option.MuitipleLineCommentStart.push_back("###");
				//option.MuitipleLineCommentEnd.push_back("###");

				option.LineComment.push_back("#");
				option.Right.push_back('}');
				Utility::DoThread doThread(&builder, &strVec, &option);

				doThread();

				try {
					// empty std::string!
					NoneReserver nonReserver;
					if (false == _LoadData(strVec, nonReserver, utTemp, option))
					{
						return true;
					}
				}
				catch (const char* err) { std::cout << err << std::endl; return false; }
				catch (std::exception & e) { std::cout << e.what() << std::endl; return false; }
				catch (std::string str) { std::cout << str << std::endl; return false; }
				catch (...) { std::cout << "not expected error" << std::endl; return  false; }

				ut = std::move(utTemp);
				return true;
			}
		public:
			static bool LoadWizDB(UserType & global, const std::string & fileName, const int thr_num) {
				UserType globalTemp = UserType("global");

				// Scan + Parse 
				if (false == LoadDataFromFile(fileName, globalTemp, thr_num, thr_num)) { return false; }
				std::cout << "LoadData End" << std::endl;

				global = std::move(globalTemp);
				return true;
			}
			// SaveQuery
			static bool SaveWizDB(const UserType & global, const std::string & fileName, const bool append = false) {
				std::ofstream outFile;
				if (fileName.empty()) { return false; }
				if (false == append) {
					outFile.open(fileName);
					if (outFile.fail()) { return false; }
				}
				else {
					outFile.open(fileName, std::ios::app);
					if (outFile.fail()) { return false; }

					outFile << "\n";
				}

				/// saveFile
				global.Save1(outFile); // cf) friend

				outFile.close();

				return true;
			}
		};
	}
}

#endif

