//  --------------------------------------------------------------
//  --  Goal:  manipulate serialized transaction
//  --------------------------------------------------------------

#include "uint256.h"
#include "profmac_1.h"

#include "util.h"
#include "json/json_spirit_utils.h"
#include "wallet.h"

#include "main.h"
#include "db.h"
#include "init.h"
#include "bitcoinrpc.h"

using namespace json_spirit;
using namespace std;


uint256 hashGenesisBlockStd ( "0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
uint256 hashGenesisBlockTest ("0x000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943");
uint256 hashGenesisBlock(hashGenesisBlockStd);
const char chHashTarget[] = \
			"00000000ffff0000000000000000000000000000000000000000000000000000";
const char chPreviousBlockHash[] = \
			"000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f";
//payoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";


json_spirit::Value deserializetransaction(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmininginfo\n"
            "Returns an object containing mining-related information.");

//  --------------------------------------------------------------
//  --  create coinbase transaction for historic block 1
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  use TxToJSON and print to debug.log
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Serialize
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Recover Hex, print to debug.log
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  De-serialize
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  use TxToJSON and print to debug.log
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Mine new nonce
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Submit to network
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Pro-forma templace to be metamorphasized.
//  --------------------------------------------------------------

    Object obj;
    obj.push_back(Pair("ProfMac", "Testing"));
    return obj;
}




