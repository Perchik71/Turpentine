// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <f4se_common/ICriticalSection.h>

namespace Turpentine
{
	class Setting
	{
	public:
		// union for various value types
		union Value
		{
			bool		b;
			char		c;
			uint8_t		h;
			int32_t		i;
			uint32_t	u;
			float		f;
			char* s;
			struct Color {
				// alpha is least sig. byte, *opposite* standard windows order
				uint8_t alpha;
				uint8_t blue;
				uint8_t green;
				uint8_t red;
			} rgba;
		};
	private:
		string _Name;
		Value _Value;
		string _Description;
	public:
		enum Types
		{
			stBool = 0x0,			// 'b' size 1
			stChar = 0x1,			// 'c' size 1
			stUnsignedChar = 0x2,	// 'h' size 1
			stSignedInt = 0x3,		// 'i'/'n' size 4
			stUnsignedInt = 0x4,	// 'u' size 4
			stFloat = 0x5,			// 'f' size 4
			stString = 0x6,			// 'S'/'s' size indet.
			stRGB = 0x7,			// 'r' size 4, alpha byte set to 255
			stRGBA = 0x8,			// 'a' size 4
			stMAX = 0x9,
			stError
		};

		explicit Setting(const char* name, bool value, const char* desc = "");
		explicit Setting(const char* name, char value, const char* desc = "");
		Setting(const char* name, uint8_t value, const char* desc = "");
		Setting(const char* name, int32_t value, const char* desc = "");
		Setting(const char* name, uint32_t value, const char* desc = "");
		Setting(const char* name, float value, const char* desc = "");
		Setting(const char* name, const char* value, const char* desc = "");
		Setting(const char* name, uint8_t r, uint8_t g, uint8_t b, const char* desc = "");
		Setting(const char* name, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const char* desc = "");

		explicit Setting(const char* name, const Value& value, const char* desc = "");
		virtual ~Setting();

		Setting(const Setting&) = delete;
		Setting& operator=(const Setting&) = delete;

		bool operator==(const char* name) const noexcept(true);
		bool operator!=(const char* name) const noexcept(true);

		static constexpr uint32_t HashInvalid = 0xFFFFFFFFul;

		[[nodiscard]] virtual uint32_t GetNameHash() const noexcept(true);
		[[nodiscard]] virtual uint64_t GetNameHash64() const noexcept(true);
		[[nodiscard]] virtual Types GetValueType() const noexcept(true);

		virtual void ClearValue();
		[[nodiscard]] virtual bool EmptyValue() const noexcept(true);

		virtual bool SetBool(bool value);
		virtual bool SetChar(char value);
		virtual bool SetChar(wchar_t value);
		virtual bool SetUnsignedChar(uint8_t value);
		virtual bool SetSignedInt(int32_t value);
		virtual bool SetUnsignedInt(uint32_t value);
		virtual bool SetFloat(float value);
		virtual bool SetString(const char* value);
		virtual bool SetRGB(uint8_t r, uint8_t g, uint8_t b);
		virtual bool SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		virtual bool SetDescription(const char* value);
		inline bool SetString(const std::string& value) { return SetString(value.c_str()); }
		inline bool SetDescription(const std::string& value) { return SetDescription(value.c_str()); }

		[[nodiscard]] inline bool CheckValidValueType(Types t) const noexcept(true) { return GetValueType() == t; }
		[[nodiscard]] inline bool GetBool() const noexcept(true) { return _Value.b; }
		[[nodiscard]] inline char GetChar() const noexcept(true) { return _Value.c; }
		[[nodiscard]] inline uint8_t GetUnsignedChar() const noexcept(true) { return _Value.h; }
		[[nodiscard]] inline int32_t GetSignedInt() const noexcept(true) { return _Value.i; }
		[[nodiscard]] inline uint32_t GetUnsignedInt() const noexcept(true) { return _Value.u; }
		[[nodiscard]] inline float GetFloat() const noexcept(true) { return _Value.f; }
		[[nodiscard]] inline const char* GetString() const noexcept(true) { return _Value.s; }
		[[nodiscard]] inline Value::Color GetRGB() const noexcept(true) { return _Value.rgba; }
		[[nodiscard]] inline Value::Color GetRGBA() const noexcept(true) { return _Value.rgba; }

		[[nodiscard]] virtual string GetName() const noexcept(true);
		[[nodiscard]] virtual string GetSection() const noexcept(true);
		[[nodiscard]] virtual string GetOptionName() const noexcept(true);
		[[nodiscard]] virtual string GetValueAsString() const noexcept(true);
		[[nodiscard]] virtual string GetDescription() const noexcept(true);

		PropertyReadOnly(GetNameHash) uint32_t NameHash;
		PropertyReadOnly(GetNameHash64) uint64_t NameHash64;
		PropertyReadOnly(GetValueType) Types ValueType;
		PropertyReadOnly(GetName) string Name;
		PropertyReadOnly(GetSection) string Section;
		PropertyReadOnly(GetOptionName) string OptionName;
		PropertyReadOnly(GetValueAsString) string ValueAsString;
		PropertyReadOnly(GetDescription) string Description;
	};

	class CollectionSettings
	{
	protected:
		ICriticalSection _lock;
		map<uint32_t, shared_ptr<Setting>> _collection;
	public:
		CollectionSettings() = default;
		virtual ~CollectionSettings() = default;

		virtual bool Add(const std::shared_ptr<Setting>& Setting);
		virtual bool Remove(const std::shared_ptr<Setting>& Setting);
		virtual bool RemoveByName(const char* SettingName);
		[[nodiscard]] virtual std::shared_ptr<Setting> FindByName(const char* SettingName) const noexcept(true);
		[[nodiscard]] virtual inline std::shared_ptr<Setting> FindByName(const string& SettingName) const noexcept(true) { return FindByName(SettingName.c_str()); }

		[[nodiscard]] virtual std::shared_ptr<Setting> At(size_t Index) const noexcept(true);
		[[nodiscard]] virtual size_t Count() const noexcept(true);

		[[nodiscard]] std::shared_ptr<Setting> operator[](size_t Index) const noexcept(true) { return At(Index); }

		virtual bool LoadFromFile(const char* FileName) = 0;
		virtual bool LoadFromFileRelative(long CategoryID, const char* FileName);

		CollectionSettings(const CollectionSettings&) = delete;
		CollectionSettings& operator=(const CollectionSettings&) = delete;
	};

	class TOMLCollectionSettings : public CollectionSettings
	{
	public:
		TOMLCollectionSettings() = default;
		virtual ~TOMLCollectionSettings() = default;

		virtual bool LoadFromFile(const char* FileName);

		TOMLCollectionSettings(const TOMLCollectionSettings&) = delete;
		TOMLCollectionSettings& operator=(const TOMLCollectionSettings&) = delete;
	};
}