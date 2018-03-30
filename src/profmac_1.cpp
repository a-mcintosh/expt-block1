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

//  --------------------------------------------------------------
#include <boost/asio.hpp>
#include <boost/asio/ip/v6_only.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_ptr.hpp>
using namespace boost;
using namespace boost::asio;
//  --------------------------------------------------------------

using namespace json_spirit;
using namespace std;


//  --------------------------------------------------------------
//  --  moved some constants here so I can change them easier.
//  --------------------------------------------------------------

uint256 hashGenesisBlockStd ( "0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
uint256 hashGenesisBlockTest ("0x000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943");
uint256 hashGenesisBlock(hashGenesisBlockStd);
const char chHashTarget[] = \
			"00000000ffff0000000000000000000000000000000000000000000000000000";
const char chPreviousBlockHash[] = \
			"000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f";
//payoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
//  --------------------------------------------------------------

extern void ScriptPubKeyToJSON(const CScript& scriptPubKey, Object& out);

extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, Object& entry);


string JSONRPCRequest2(const string& strMethod, const Array& params, const Value& id)
{
    Object request;
    request.push_back(Pair("method", strMethod));
    request.push_back(Pair("params", params));
    request.push_back(Pair("id", id));
    return write_string(Value(request), false) + "\n";
}


//  --------------------------------------------------------------
//  --  capture behavior of some serialization libraries
//  --------------------------------------------------------------
Value deserializetransaction(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmininginfo\n"
            "Returns an object containing mining-related information.");

    Object result;
    result.push_back(Pair("ProfMac", "Testing"));

//  --------------------------------------------------------------
//  --  create coinbase transaction for historic block 1
//  --------------------------------------------------------------

    static CReserveKey reservekey(pwalletMain);

    // Create coinbase tx
    CTransaction txNew;
    txNew.vin.resize(1);
    txNew.vin[0].prevout.SetNull();
    txNew.vin[0].scriptSig = CScript() << OP_0 << OP_0;
    txNew.vout.resize(0);

    CScript scriptPubKey;
//    scriptPubKey.SetDestination(CBitcoinAddress(reservekey.GetReservedKey().GetID()).Get());
    scriptPubKey.SetDestination(CBitcoinAddress("12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX").Get());
    txNew.vout.push_back(CTxOut(50 * COIN, scriptPubKey));


  if(true) {
    string strPrint;
    printf("profmac_1.cpp line 73: %s\n", strPrint.c_str());
  }
    TxToJSON(txNew, 0, result);

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

    return result;
}




