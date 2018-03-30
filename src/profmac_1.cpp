//  --------------------------------------------------------------
//  --  Goal:  manipulate serialized transaction
//  --  use bitcoin block 1 as a reference.
//  --------------------------------------------------------------

#include "uint256.h"
#include "profmac_0.h"
#include "profmac_1.h"
#include "init.h"
#include "bitcoinrpc.h"
#include "build.h"

//  --------------------------------------------------------------
//  --------------------------------------------------------------

using namespace std;
using namespace boost;

//  --------------------------------------------------------------
string payoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
CBigNum bnExtraNonce = 3;
unsigned int nBits = 0x1d00ffff;
bool authentic_block_1 = false;  //toggle some exploratory behavior
//  --------------------------------------------------------------

extern void ScriptPubKeyToJSON(const CScript& scriptPubKey, Object& out);
extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, Object& entry);

//  --------------------------------------------------------------
//  --  Implement "deserializetransaction" as an RPC command.
//  --    verify and document selected ways to manipulate transactions.
//  --------------------------------------------------------------
Value deserializetransaction(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmininginfo\n"
            "Returns an object containing mining-related information.");

    Object result;
    result.push_back(Pair("ProfMac", "deserializetransaction"));
    result.push_back(Pair(BUILD_DESC, BUILD_DATE));
//  --------------------------------------------------------------
//  --  create coinbase transaction for historic block 1
//  --------------------------------------------------------------

    static CReserveKey reservekey(pwalletMain);

    //
    // Create coinbase tx
    //
    CTransaction txNew;
    txNew.vin.resize(1);
    txNew.vin[0].prevout.SetNull();
    txNew.vin[0].scriptSig << nBits << ++bnExtraNonce;
    txNew.vout.resize(0);

    CScript scriptPubKey;
    if(authentic_block_1)
      scriptPubKey = CScript() << ParseHex("0496b538e853519c726a2c91e61ec11600ae1390813a627c66fb8be7947be63c52da7589379515d4e0a604f8141781e62294721166bf621e73a82cbf2342c858ee") << OP_CHECKSIG;
    else {
      scriptPubKey << reservekey.GetReservedKey() << OP_CHECKSIG;
      txNew.vout.push_back(CTxOut(50 * COIN, scriptPubKey));
      reservekey.KeepKey();
    }

bool Execute_CheckSyntax = false;
if(Execute_CheckSyntax) {
}

//  --------------------------------------------------------------
//  --  use TxToJSON and print to debug.log
//  --------------------------------------------------------------
    TxToJSON(txNew, 0, result);
    if(true) {
      string strPrint;
      printf("profmac_1.cpp line 73: %s\n", strPrint.c_str());
    }

//  --------------------------------------------------------------
//  --  Serialize
//  --------------------------------------------------------------
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << txNew;
    string strHex = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(Pair("hex-datastream", strHex));

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
//  --  Pro-forma template to be metamorphasized.
//  --------------------------------------------------------------

    return result;
}




