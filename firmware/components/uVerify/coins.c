/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "coins.h"

#include <string.h>

#include "cryptoaddress.h"
#include "base58.h"
#include "curves.h"
#include "ecdsa.h"
#include "secp256k1.h"

// filled CoinInfo structure defined in coins.h
const CoinInfo coins[COINS_COUNT] = {
		{"Bitcoin",      " BTC",     2000000, "\x18" "Bitcoin Signed Message:\n",      true, true, true,  false, false,    0,    5, 0x0488b21e, 0x0488ade4,  0, "bc",   0x80000000, SECP256K1_NAME, &secp256k1_info, },
		{"Testnet",      " TEST",   10000000, "\x18" "Bitcoin Signed Message:\n",      true, true, true,  false, false,  111,  196, 0x043587cf, 0x04358394,  0, "tb",   0x80000001, SECP256K1_NAME, &secp256k1_info, },
		{"Bcash",        " BCH",      500000, "\x18" "Bitcoin Signed Message:\n",      true, true, false, true,  true,     0,    5, 0x0488b21e, 0x0488ade4,  0, NULL,   0x80000091, SECP256K1_NAME, &secp256k1_info, },
		{"Namecoin",     " NMC",    10000000, "\x19" "Namecoin Signed Message:\n",     true, true, false, false, false,   52,    5, 0x019da462, 0x019d9cfe,  0, NULL,   0x80000007, SECP256K1_NAME, &secp256k1_info, },
		{"Litecoin",     " LTC",    40000000, "\x19" "Litecoin Signed Message:\n",     true, true, true,  false, false,   48,   50, 0x019da462, 0x019d9cfe,  0, "ltc",  0x80000002, SECP256K1_NAME, &secp256k1_info, },
		{"Dogecoin",     " DOGE", 1000000000, "\x19" "Dogecoin Signed Message:\n",     true, true, false, false, false,   30,   22, 0x02facafd, 0x02fac398,  0, NULL,   0x80000003, SECP256K1_NAME, &secp256k1_info, },
		{"Dash",         " DASH",     100000, "\x19" "DarkCoin Signed Message:\n",     true, true, false, false, false,   76,   16, 0x02fe52cc, 0x02fe52f8,  0, NULL,   0x80000005, SECP256K1_NAME, &secp256k1_info, },
		{"Zcash",        " ZEC",     1000000, "\x16" "Zcash Signed Message:\n",        true, true, false, false, false, 7352, 7357, 0x0488b21e, 0x0488ade4,  0, NULL,   0x80000085, SECP256K1_NAME, &secp256k1_info, },
		{"Bitcoin Gold", " BTG",      500000, "\x1d" "Bitcoin Gold Signed Message:\n", true, true, true,  true,  true,    38,   23, 0x0488b21e, 0x0488ade4, 79, "btg",  0x8000009c, SECP256K1_NAME, &secp256k1_info, },
		{"DigiByte",     " DGB",      500000, "\x19" "DigiByte Signed Message:\n",     true, true, true,  false, false,   30,    5, 0x0488b21e, 0x0488ade4,  0, "dgb",  0x80000014, SECP256K1_NAME, &secp256k1_info, },
		{"Monacoin",     " MONA",    5000000, "\x19" "Monacoin Signed Message:\n",     true, true, true,  false, false,   50,   55, 0x0488b21e, 0x0488ade4,  0, "mona", 0x80000016, SECP256K1_NAME, &secp256k1_info, },
		{"Fujicoin",     " FJC",     1000000, "\x19" "FujiCoin Signed Message:\n",     true, true, false, false, false,   36,   16, 0x0488b21e, 0x0488ade4,  0, NULL,   0x8000004b, SECP256K1_NAME, &secp256k1_info, },
		{"Vertcoin",     " VTC",    40000000, "\x19" "Vertcoin Signed Message:\n",     true, true, true,  false, false,   71,    5, 0x0488b21e, 0x0488ade4,  0, "vtc",  0x8000001c, SECP256K1_NAME, &secp256k1_info, },
	#if DEBUG_LINK
		{"Bcash Testnet",  " TBCH", 10000000, "\x18" "Bitcoin Signed Message:\n", true, true, false, true,  true,   111,  196, 0x043587cf, 0x04358394, 0, NULL, 0x80000001, SECP256K1_NAME, &secp256k1_info, },
		{"Zcash Testnet",  " TAZ",  10000000, "\x16" "Zcash Signed Message:\n",   true, true, false, false, false, 7461, 7354, 0x043587cf, 0x04358394, 0, NULL, 0x80000001, SECP256K1_NAME, &secp256k1_info, },
		{"Decred Testnet", " TDCR", 10000000, "\x17" "Decred Signed Message:\n",  true, true, false, false, false, 3873, 3836, 0x043587d1, 0x04358397, 0, NULL, 0x80000001, SECP256K1_NAME, &secp256k1_info, },
	#endif

};

const CoinInfo *coinByName(const char *name)
{
	if (!name) return 0;
	for (int i = 0; i < COINS_COUNT; i++) {
		if (strcmp(name, coins[i].coin_name) == 0) {
			return &(coins[i]);
		}
	}
	return 0;
}

const CoinInfo *coinByAddressType(uint32_t address_type)
{
	for (int i = 0; i < COINS_COUNT; i++) {
		if (address_type == coins[i].address_type) {
			return &(coins[i]);
		}
	}
	return 0;
}

const CoinInfo *coinByCoinType(uint32_t coin_type)
{
	for (int i = 0; i < COINS_COUNT; i++) {
		if (coin_type == coins[i].coin_type) {
			return &(coins[i]);
		}
	}
	return 0;
}

bool coinExtractAddressType(const CoinInfo *coin, const char *addr, uint32_t *address_type)
{
	if (!addr) return false;
	uint8_t addr_raw[MAX_ADDR_RAW_SIZE];
	int len = base58_decode_check(addr, coin->curve->hasher_type, addr_raw, MAX_ADDR_RAW_SIZE);
	if (len >= 21) {
		return coinExtractAddressTypeRaw(coin, addr_raw, address_type);
	}
	return false;
}

bool coinExtractAddressTypeRaw(const CoinInfo *coin, const uint8_t *addr_raw, uint32_t *address_type)
{
	if (coin->has_address_type && address_check_prefix(addr_raw, coin->address_type)) {
		*address_type = coin->address_type;
		return true;
	}
	if (coin->has_address_type_p2sh && address_check_prefix(addr_raw, coin->address_type_p2sh)) {
		*address_type = coin->address_type_p2sh;
		return true;
	}
	*address_type = 0;
	return false;
}

