#pragma once

#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cassert>

//	#include <stdint.h>		=> DWORD
#ifndef int32_t
#define uint32_t DWORD
#define int32_t int
#endif

namespace stackoverflow_20745184
{

struct MapItem
{
	std::string term;
    std::vector<int32_t> pl;

	//  this function also emphasizes the way this objects are serialized to file; if something changes here,
    //    ReadNext & WriteNext must also be updated (unfortunate dependency) 
    size_t SizeWrittenToFile() const 
	{
		return 2*sizeof(uint32_t)+term.length()+pl.size()*sizeof(int32_t);
	}
};

bool ReadNext(std::istream& in, MapItem& item)
{
	size_t size;
	in.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
	if (!in)
		return false;

	std::istreambuf_iterator<char> itIn(in); 
	std::string& out = item.term;
	out.reserve(size);
	out.clear();	//	this is necessary if the string is not empty
	//	copy_n available in C++11
	for (std::insert_iterator<std::string> itOut(out, out.begin()); in && (out.length() < size); itIn++, itOut++)
		*itOut = *itIn;

	assert(in);
	if (!in)
		return false;

	in.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
	if (!in)
		return false;

	std::vector<int32_t>& out2 = item.pl;
	out2.resize(size);	//	reserve doesn't work here
	in.read(reinterpret_cast<char*>(&out2[0]), size * sizeof(int32_t));
	assert(in);

	return true;
}

// a "header" should be added to mark complete data (to write "atomically")
bool WriteNext(std::ostream& out, const MapItem& item)
{
	size_t size = item.term.length();
	out.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
	if (!out)
		return false;
	out.write(item.term.c_str(), size);
	if (!out)
		return false;

	size = item.pl.size();
	out.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
	if (!out)
		return false;
	out.write(reinterpret_cast<const char*>(&item.pl[0]), size * sizeof(int32_t));
	if (!out)
		return false;

	return true;
}

bool UpdateItem(std::ifstream& in, std::ofstream& out, const MapItem& item)
{
	MapItem it;
	bool result;
	for (result = ReadNext(in, it); result && (it.term != item.term); result = ReadNext(in, it))
		if (!WriteNext(out, it))
			return false;
	if (!result)
		return false;

	assert(it.term == item.term);
	if (!WriteNext(out, item))
		return false;

	for (result = ReadNext(in, it); result; result = ReadNext(in, it))
		if (!WriteNext(out, it))
			return false;

	return in.eof();
}

bool UpdateItem(const char* filename, const MapItem& item)
{
	std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
	assert(in);

	std::string filename2(filename);
	filename2 += ".tmp";
	std::ofstream out(filename2.c_str(), std::ios_base::out | std::ios_base::binary);
	assert(out);

	bool result = UpdateItem(in, out, item);
	in.close();
	out.close();

	int err = 0;
	
	if (result)
	{
		err = remove(filename);
		assert(!err && "remov_140");
		result = !err;
	}
	if (!result)
	{
		err = remove(filename2.c_str());
		assert(!err && "remov_147");
	}
	else
	{
		err = rename(filename2.c_str(), filename);
		assert(!err && "renam_151");
		result = !err;
	}

	return result;
}


void Print(const MapItem& it)
{
	std::cout<<it.term<<" : ";
	for (size_t i = 0; i < it.pl.size(); ++i) std::cout<<it.pl[i];
	std::cout<<std::endl;
}

void PrintNext(std::istream& in)
{
	MapItem item;
	if (!ReadNext(in, item))
	{
		std::cout<<"Error reading\n";
		return;
	}
	Print(item);

	std::streampos offset = item.SizeWrittenToFile();
	offset = -offset;
	in.seekg(offset, std::ios_base::cur);
}

void PrintAll(const char* filename)
{
	std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
	assert(in);

	MapItem it;
	for (bool result = ReadNext(in, it); result; result = ReadNext(in, it))
		Print(it);
}

void Test1_WriteOneReadOne()
{
	static const char* FILENAME = "sample1.dat";

	std::ofstream out(FILENAME, std::ios_base::out | std::ios_base::binary);
	assert(out);

	MapItem it1;
	it1.term = "one";
	it1.pl.push_back(1); it1.pl.push_back(2); it1.pl.push_back(3);

	bool result = WriteNext(out, it1);
	assert(result && "WriteNex_71");

	out.flush();

	std::ifstream in(FILENAME, std::ios_base::in | std::ios_base::binary);
	assert(in);

	MapItem it2;
	result = ReadNext(in, it2);
	assert(result && "ReadNex_80");

	assert(it1.term == it2.term);
	assert(it1.pl == it2.pl);
}


void Test2_WriteMoreReadMore()
{
	static const char* FILENAME = "sample2.dat";

	std::ofstream out(FILENAME, std::ios_base::out | std::ios_base::binary);
	assert(out);

	static const unsigned ELEMENTS_NO = 13;
	std::vector<MapItem> items(ELEMENTS_NO);
	for (size_t i = 0; i < items.size(); ++i)
	{
		items[i].term = "item";
		std::ostringstream ostr;
		ostr<<i;
		items[i].term += ostr.str();
		items[i].pl.push_back(i+1); items[i].pl.push_back(i+2); items[i].pl.push_back(i+3);
	}

	bool result = false;
	for (size_t i = 0; i < items.size(); ++i)
	{
		result = WriteNext(out, items[i]);
		assert(result && "WriteNex_71");
	}

	out.flush();

	std::ifstream in(FILENAME, std::ios_base::in | std::ios_base::binary);
	assert(in);

	int cnt = 0;
	MapItem it2;
	for (result = ReadNext(in, it2); result; result = ReadNext(in, it2))
	{
		if (0)
			Print(it2);

		assert(it2.term == items[cnt].term);
		assert(it2.pl == items[cnt].pl);

		++cnt;
	}
	assert(ELEMENTS_NO == cnt);
}

void Test3_PrintNext()
{
	std::ifstream in("sample2.dat", std::ios_base::in | std::ios_base::binary);
	assert(in);

	std::streampos first = in.tellg();
	assert(0 == first);
	PrintNext(in);
	std::streampos second = in.tellg();
	assert(first == second);

	MapItem item;
	bool result = ReadNext(in, item);
	assert(result); result;
	first = in.tellg();
	assert(first > second);
	PrintNext(in);
	second = in.tellg();
	assert(first == second);
}

void Test4_ReadMore()
{
	std::ifstream in("sample2.dat", std::ios_base::in | std::ios_base::binary);
	assert(in);

	MapItem it2;
	for (bool result = ReadNext(in, it2); result; result = ReadNext(in, it2))
		Print(it2);
}

void Test5_UpdateItem()
{
	MapItem item;
	item.term = "item7";
	for (int i = 0; i < 4; i++)
		item.pl.push_back(7+i);

	bool result = UpdateItem("sample2.dat", item);
	assert(result && "UpdateIte_289");

	item.term = "item77";
	result = UpdateItem("sample2.dat", item);
	assert(!result && "UpdateIte_294: not found");

	item.term = "item0";
	result = UpdateItem("sample2.dat", item);
	assert(result && "UpdateIte_298");
}

}	//stackoverflow_20745184