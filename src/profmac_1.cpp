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

//  --------------------------------------------------------------
string payoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
CBigNum bnExtraNonce = 3;
unsigned int nBits = 0x1d00ffff;
bool authentic_block_1 = false;  //toggle some exploratory behavior
//  --------------------------------------------------------------

extern void ScriptPubKeyToJSON(const CScript& scriptPubKey, json_spirit::Object& out);
extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, json_spirit::Object& entry);

//  --------------------------------------------------------------
//  --  Implement "deserializetransaction" as an RPC command.
//  --    verify and document selected ways to manipulate transactions.
//  --------------------------------------------------------------
json_spirit::Value deserializetransaction(const json_spirit::Array& params, bool fHelp)
{
    if (fHelp || 3 < params.size())
        throw runtime_error(
            "deserializetransaction [authentic_block_1=false]\n"
            "Returns an json_spirit::Object containing mining-related information.");

//  --------------------------------------------------------------
//  --  dump a lot of parameters, mostly to learn the type system.
//  --------------------------------------------------------------
    json_spirit::Object result;
    result.push_back(json_spirit::Pair("ProfMac", "deserializetransaction"));
    result.push_back(json_spirit::Pair(BUILD_DESC, BUILD_DATE));
    result.push_back(json_spirit::Pair("params.size()", params.size()));
    result.push_back(json_spirit::Pair("parameters", params));
    json_spirit::Array arry;
    for (uint ix=0; ix < params.size(); ix++) {
      arry.push_back(params[ix].type());}
    result.push_back(json_spirit::Pair("parameter_types",arry));
//  --
    if (params.size() > 0) authentic_block_1 = ("true" == params[0].get_str());
    if(authentic_block_1) bnExtraNonce = 3;
    result.push_back(json_spirit::Pair("authentic_block_1", authentic_block_1));

//  --------------------------------------------------------------
//  --  create coinbase transaction de-novo for historic block 1
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
    if(authentic_block_1) {
      scriptPubKey = CScript() << ParseHex("0496b538e853519c726a2c91e61ec11600ae1390813a627c66fb8be7947be63c52da7589379515d4e0a604f8141781e62294721166bf621e73a82cbf2342c858ee") << OP_CHECKSIG;
    } else {
      scriptPubKey << reservekey.GetReservedKey() << OP_CHECKSIG;
      reservekey.KeepKey();
    }
    txNew.vout.push_back(CTxOut(50 * COIN, scriptPubKey));

bool Execute_CheckSyntax = false;
if(Execute_CheckSyntax) {
}

//  --------------------------------------------------------------
//  --  use TxToJSON and include new txn in JSON return value
//  --------------------------------------------------------------
    result.push_back(json_spirit::Pair("ProfMac", "txNew"));
    TxToJSON(txNew, 0, result);
    if(true) {
      string strPrint;
      printf("profmac_1.cpp line 73: %s\n", strPrint.c_str());
    }

//  --------------------------------------------------------------
//  --  Serialize the txn
//  --  Recover Hex-string, include in JSON return value
//  --------------------------------------------------------------
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << txNew;
    result.push_back(json_spirit::Pair("ProfMac", "process-ssTx"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    string strHex = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(json_spirit::Pair("hex-datastream", strHex));

//  --------------------------------------------------------------
//  -- Recover Txn from stream, edit, include in JSON return value
//  --------------------------------------------------------------
    CTransaction txRecover;
    ssTx >> txRecover;  //  -- test an idempotent operation.
    txRecover.vout[0].nValue = 20 * COIN;
    result.push_back(json_spirit::Pair("ProfMac", "txRecover"));
    TxToJSON(txRecover, 0, result);

//  --------------------------------------------------------------
//  -- Recover Txn from hex-string, edit, include in JSON return value
//  --------------------------------------------------------------
    CDataStream ssTxInjected(SER_NETWORK, PROTOCOL_VERSION);
    ssTxInjected << ParseHex(strHex);
    ssTxInjected.ignore(1); 
    result.push_back(json_spirit::Pair("ProfMac", "process-ssTxInjected"));
    result.push_back(json_spirit::Pair("ssTxInjected.size", ssTxInjected.size()));
    strHex = HexStr(ssTxInjected.begin(), ssTxInjected.end());
    result.push_back(json_spirit::Pair("hex-datastream-injected", strHex));

    CTransaction txRecoverInjected;
    ssTxInjected >> txRecoverInjected;  //  -- test an idempotent operation.
    txRecoverInjected.vout[0].nValue = 30 * COIN;
    
//  --------------------------------------------------------------
//  --  use TxToJSON and include in JSON return value
//  --------------------------------------------------------------
    result.push_back(json_spirit::Pair("ProfMac", "txRecoverInjected"));
    TxToJSON(txRecoverInjected, 0, result);

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




