﻿// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <TSettings.h>
#include <TRelocation.h>
#include <TUtils.h>
#include <toml.hpp>

extern uintptr_t GlobalBase;

namespace Turpentine
{
	Setting::Setting(const char* name, bool value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.b = value;
	}

	Setting::Setting(const char* name, char value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.c = value;
	}

	Setting::Setting(const char* name, uint8_t value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.h = value;
	}

	Setting::Setting(const char* name, int32_t value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.i = value;
	}

	Setting::Setting(const char* name, uint32_t value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.u = value;
	}

	Setting::Setting(const char* name, float value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.f = value;
	}

	Setting::Setting(const char* name, const char* value, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.s = value ? _strdup(value) : nullptr;
	}

	Setting::Setting(const char* name, uint8_t r, uint8_t g, uint8_t b, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.rgba = { r, g, b, 0xFF };
	}

	Setting::Setting(const char* name, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char* desc) :
		_Name(name), _Description(desc)
	{
		_Value.rgba = { r, g, b, a };
	}

	Setting::Setting(const char* name, const Value& value, const char* desc) :
		_Value(value), _Name(name), _Description(desc)
	{}

	Setting::~Setting()
	{
		ClearValue();
	}

	bool Setting::operator==(const char* name) const noexcept
	{
		return _stricmp(_Name.c_str(), name) != 0;
	}

	bool Setting::operator!=(const char* name) const noexcept
	{
		return !_stricmp(_Name.c_str(), name);
	}

	Setting::Types Setting::GetValueType() const noexcept
	{
		if (_Name.empty()) return stError;
		switch (tolower(_Name[0]))
		{
		case 'b':
			return stBool;
		case 'c':
			return stChar;
		case 'h':
			return stUnsignedChar;
		case 'i':
		case 'n':
			return stSignedInt;
		case 'u':
			return stUnsignedInt;
		case 'f':
			return stFloat;
		case 's':
			return stString;
		case 'r':
			return stRGB;
		case 'a':
			return stRGBA;
		default:
			return stError;
		}
	}

	uint32_t Setting::GetNameHash() const noexcept(true)
	{
		if (_Name.empty())
			return 0xFFFFFFFFul;

		auto s = _strdup(_Name.c_str());
		if (!s)
			return 0xFFFFFFFFul;

		auto h = Utils::MurmurHash32A(_strlwr(s), _Name.length(), 0);
		free(s);

		return h;
	}

	uint64_t Setting::GetNameHash64() const noexcept(true)
	{
		if (_Name.empty())
			return 0xFFFFFFFFul;

		auto s = _strdup(_Name.c_str());
		if (!s)
			return 0xFFFFFFFFul;

		auto h = Utils::MurmurHash64A(_strlwr(s), _Name.length(), 0);
		free(s);

		return h;
	}

	void Setting::ClearValue()
	{
		if (CheckValidValueType(stString))
		{
			if (_Value.s)
			{
				free(_Value.s);
				_Value.s = nullptr;
			}
		}
		else _Value.s = nullptr;
	}

	bool Setting::EmptyValue() const noexcept
	{
		return _Value.s == nullptr;
	}

	bool Setting::SetBool(bool value)
	{
		bool r = CheckValidValueType(stBool);
		if (r) _Value.b = value;
		return r;
	}

	bool Setting::SetChar(char value)
	{
		bool r = CheckValidValueType(stChar);
		if (r) _Value.c = value;
		return r;
	}

	bool Setting::SetChar(wchar_t value)
	{
		bool r = CheckValidValueType(stChar);
		if (r) _Value.c = (char)std::max((int32_t)value, (int32_t)0x7F);
		return r;
	}

	bool Setting::SetUnsignedChar(uint8_t value)
	{
		bool r = CheckValidValueType(stUnsignedChar);
		if (r) _Value.h = value;
		return r;
	}

	bool Setting::SetSignedInt(int32_t value)
	{
		bool r = CheckValidValueType(stSignedInt);
		if (r) _Value.i = value;
		return r;
	}

	bool Setting::SetUnsignedInt(uint32_t value)
	{
		bool r = CheckValidValueType(stUnsignedInt);
		if (r) _Value.u = value;
		return r;
	}

	bool Setting::SetFloat(float value)
	{
		bool r = CheckValidValueType(stFloat);
		if (r) _Value.f = value;
		return r;
	}

	bool Setting::SetString(const char* value)
	{
		bool r = CheckValidValueType(stString);
		if (r)
		{
			uint16_t len = (uint16_t)strlen(value);
			auto newStr = (char*)malloc((size_t)len + 1);
			if (!newStr) r = false;
			if (r)
			{
				memcpy(newStr, value, len);
				newStr[len] = '\0';
				if (_Value.s) free(_Value.s);
				_Value.s = newStr;
			}
		}
		return r;
	}

	bool Setting::SetRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		bool res = CheckValidValueType(stRGB);
		if (res)
		{
			_Value.rgba.red = r;
			_Value.rgba.green = g;
			_Value.rgba.blue = b;
			_Value.rgba.alpha = 0xFF;
		}
		return res;
	}

	bool Setting::SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		bool res = CheckValidValueType(stRGBA);
		if (res)
		{
			_Value.rgba.red = r;
			_Value.rgba.green = g;
			_Value.rgba.blue = b;
			_Value.rgba.alpha = a;
		}
		return res;
	}

	bool Setting::SetDescription(const char* value)
	{
		_Description = value;
		return true;
	}

	string Setting::GetName() const noexcept(true)
	{
		return _Name;
	}

	string Setting::GetSection() const noexcept
	{
		string s(_Name);
		auto it = s.find_first_of(':');
		if (it != string::npos)
			return s.substr(++it);
		return "";
	}

	string Setting::GetOptionName() const noexcept
	{
		string s(_Name);
		auto it = s.find_first_of(':');
		if (it != string::npos)
			return s.substr(0, it);
		return s;
	}

	string Setting::GetValueAsString() const noexcept
	{
		char szBuf[64];
		ZeroMemory(szBuf, _ARRAYSIZE(szBuf));

		switch (GetValueType())
		{
		case Setting::stBool:
			return _Value.b ? "true" : "false";
		case Setting::stChar:
			szBuf[0] = _Value.c;
			szBuf[1] = 0;
			return szBuf;
		case Setting::stUnsignedChar:
			_ultoa_s(_Value.h, szBuf, 16);
			return szBuf;
		case Setting::stSignedInt:
			_ltoa_s(_Value.i, szBuf, 10);
			return szBuf;
		case Setting::stUnsignedInt:
			_ultoa_s(_Value.u, szBuf, 10);
			return szBuf;
		case Setting::stFloat:
			sprintf_s(szBuf, "%.4f", _Value.f);
			return szBuf;
		case Setting::stString:
			return _Value.s ? _Value.s : "";
		case Setting::stRGB:
			sprintf_s(szBuf, "%u,%u,%u", _Value.rgba.red, _Value.rgba.green, _Value.rgba.blue);
			return szBuf;
		case Setting::stRGBA:
			sprintf_s(szBuf, "%u,%u,%u,%u", _Value.rgba.red, _Value.rgba.green, _Value.rgba.blue, _Value.rgba.alpha);
			return szBuf;
		default:
			return "";
		}
	}

	string Setting::GetDescription() const noexcept
	{
		return _Description;
	}

	// CollectionSettings

	bool CollectionSettings::Add(const std::shared_ptr<Setting>& Setting)
	{
		if (!Setting || Setting->Name.empty())
			return false;

		IScopedCriticalSection Locker(&_lock);

		auto hash = Setting->NameHash;
		if (hash == 0xFFFFFFFF)
			return false;

		if (_collection.size() == _collection.max_size())
			return false;

		auto it = _collection.lower_bound(hash);
		if (it == _collection.end())
		{
			if (it->first == hash)
			{
				_ERROR("An option with that name \"%s\" is already in the list, it will be skipped.", Setting->Name.c_str());
				return false;
			}
		}

		_collection.insert(std::make_pair<>(hash, Setting));

		return true;
	}

	bool CollectionSettings::Remove(const std::shared_ptr<Setting>& Setting)
	{
		if (!Setting || Setting->Name.empty())
			return false;

		IScopedCriticalSection Locker(&_lock);

		auto hash = Setting->NameHash;
		if (hash == 0xFFFFFFFF)
			return false;

		auto item = _collection.lower_bound(hash);
		if ((item == _collection.end()) || (Setting != item->second))
			return false;

		_collection.erase(item);

		return true;
	}

	bool CollectionSettings::RemoveByName(const char* SettingName)
	{
		if (!SettingName)
			return false;

		IScopedCriticalSection Locker(&_lock);

		auto hash = Setting(SettingName, "").NameHash;
		if (hash == 0xFFFFFFFF)
			return false;

		auto item = _collection.lower_bound(hash);
		if (item == _collection.end())
			return false;

		if (item->first != hash)
			return false;

		_collection.erase(item);

		return true;
	}

	std::shared_ptr<Setting> CollectionSettings::FindByName(const char* SettingName) const noexcept
	{
		if (!SettingName)
			return nullptr;

		IScopedCriticalSection Locker(&(const_cast<CollectionSettings*>(this)->_lock));

		auto hash = Setting(SettingName, "").NameHash;
		if (hash == 0xFFFFFFFF)
			return nullptr;

		if (!_collection.size())
			return nullptr;

		auto item = _collection.lower_bound(hash);
		if (item == _collection.end())
			return nullptr;

		if (item->first != hash)
			return nullptr;

		return item->second;
	}

	std::shared_ptr<Setting> CollectionSettings::At(size_t Index) const noexcept
	{
		IScopedCriticalSection Locker(&(const_cast<CollectionSettings*>(this)->_lock));
		if (_collection.size() <= Index)
			return nullptr;

		auto it = _collection.begin();
		std::advance(it, Index);

		return it->second;
	}

	size_t CollectionSettings::Count() const noexcept
	{
		IScopedCriticalSection Locker(&(const_cast<CollectionSettings*>(this)->_lock));
		return _collection.size();
	}

	bool CollectionSettings::LoadFromFileRelative(long CategoryID, const char* FileName)
	{
		char Path[MAX_PATH];
		HRESULT Result = SHGetFolderPathA(NULL, CategoryID | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, Path);

		if (!SUCCEEDED(Result))
		{
			_ERROR("SHGetFolderPath %08X failed (result = %08X lasterr = %08X)", CategoryID, Result, GetLastError());
			return false;
		}

		strcat_s(Path, FileName);
		return LoadFromFile(Path);
	}

	// TOMLCollectionSettings

	bool TOMLCollectionSettings::LoadFromFile(const char* FileName)
	{
		if (!FileName)
			return false;

		IScopedCriticalSection Locker(&_lock);

		auto Attr = GetFileAttributesA(FileName);
		if (((Attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) ||
			(Attr == INVALID_FILE_ATTRIBUTES))
		{
			_MESSAGE("The settings file \"%s\" was not found.", FileName);
			return false;
		}

		auto TOMLResult = toml::try_parse(FileName, toml::spec::v(1, 1, 0));
		if (!TOMLResult.is_ok())
		{
			_ERROR("Error reading the settings file \"%s\".", FileName);
			
			auto& TOMLError = TOMLResult.as_err();
			for (size_t i = 0; i < TOMLError.size(); i++)
				_MESSAGE("\t%s", TOMLError.at(i).title().c_str());

			return false;
		}

		auto& TOMLData = TOMLResult.unwrap();
		for (auto& Item : _collection)
		{
			if (!Item.second)
			{
				_WARNING("Nullptr in collection settings!");
				continue;
			}

			auto SectionName = Item.second->Section;
			auto OptionName = Item.second->OptionName;
			if (SectionName.empty() || OptionName.empty())
			{
				_WARNING("Incorrect option setting \"%s:%s\"!",
					(OptionName.empty() ? "" : OptionName.c_str()),
					(SectionName.empty() ? "" : SectionName.c_str()));
				continue;
			}

			if (TOMLData.contains(SectionName))
			{
				auto& TOMLTable = TOMLData.at(Item.second->Section);
				if (!TOMLTable.is_table())
				{
					_ERROR("There is a section, but it is not a section! Stop!");
					return false;
				}

				if (TOMLTable.contains(OptionName))
				{
					auto& TOMLItem = TOMLTable.at(OptionName);

					switch (Item.second->ValueType)
					{
					case Setting::stBool:			// 'b' size 1
						if (TOMLItem.is_boolean())
							Item.second->SetBool(TOMLItem.as_boolean());
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stChar:			// 'c' size 1
						if (TOMLItem.is_string())
							Item.second->SetChar(TOMLItem.as_string()[0]);
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stUnsignedChar:	// 'h' size 1
						if (TOMLItem.is_integer())
						{
							TOMLItem.as_integer_fmt().fmt = toml::integer_format::hex;
							Item.second->SetUnsignedChar(max(0ll, min(TOMLItem.as_integer(), 255ll)));
						}
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stSignedInt:		// 'i'/'n' size 4
						if (TOMLItem.is_integer())
							Item.second->SetSignedInt((int32_t)TOMLItem.as_integer());
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stUnsignedInt:	// 'u' size 4
						if (TOMLItem.is_integer())
							Item.second->SetUnsignedInt((uint32_t)TOMLItem.as_integer());
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stFloat:			// 'f' size 4
						if (TOMLItem.is_floating())
							Item.second->SetFloat((float)TOMLItem.as_floating());
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stString:			// 'S'/'s' size indet.
						if (TOMLItem.is_string())
							Item.second->SetString(TOMLItem.as_string());
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stRGB:			// 'r' size 4, alpha byte set to 255
						if (TOMLItem.is_array() && (TOMLItem.as_array().size() == 3))
						{
							auto& Color = TOMLItem.as_array();
							if (!Color[0].is_integer() || !Color[1].is_integer() || !Color[2].is_integer())
								_WARNING("The parameter \"%s\" should be [%u,%u,%u,%u].", Item.second->Name.c_str());
							else
								Item.second->SetRGB(
									max(0ll, min(Color[0].as_integer(), 255ll)),
									max(0ll, min(Color[1].as_integer(), 255ll)),
									max(0ll, min(Color[2].as_integer(), 255ll)));
						}
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					case Setting::stRGBA:			// 'a' size 4
						if (TOMLItem.is_array() && (TOMLItem.as_array().size() == 4))
						{
							auto& Color = TOMLItem.as_array();
							if (!Color[0].is_integer() || !Color[1].is_integer() || !Color[2].is_integer() || !Color[3].is_integer())
								_WARNING("The parameter \"%s\" should be [%u,%u,%u,%u].", Item.second->Name.c_str());
							else
								Item.second->SetRGBA(
									max(0ll, min(Color[0].as_integer(), 255ll)),
									max(0ll, min(Color[1].as_integer(), 255ll)),
									max(0ll, min(Color[2].as_integer(), 255ll)),
									max(0ll, min(Color[3].as_integer(), 255ll)));
						}
						else
						{
							_WARNING("The parameter \"%s\" type does not match the setting type.", Item.second->Name.c_str());
							continue;
						}
						break;
					default:
						continue;
					}
				}
			}
		}

		return true;
	}
}
