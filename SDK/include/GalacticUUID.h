#pragma once

namespace Galactic3D
{
	struct tUUID
	{
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];
	};

	inline bool operator==(const tUUID& lhs, const tUUID& rhs)
	{
		return memcmp(&lhs, &rhs, sizeof(tUUID)) == 0;
	}

	inline bool operator!=(const tUUID& lhs, const tUUID& rhs)
	{
		return memcmp(&lhs, &rhs, sizeof(tUUID)) != 0;
	}

	constexpr const tUUID DefineUUID(uint32_t l, uint16_t w1, uint16_t w2, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8)
	{
		return { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } };
	}
}
