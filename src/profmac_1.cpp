//  --------------------------------------------------------------
//  --  Goal:  Read a coinbase, manipulate it, and re-emit it.
//  --    An experiment toward implementing a peer mining pool.
//  --
//  --  This code
//  --    re-creates de-novo Block 1 on the main bitcoin chain,
//  --    transforms it to the getrawtransaction hex result,
//  --    recovers a txn from that hex, (main goal of this exercise)
//  --    edits the recovered txn,
//  --    and documents it all in a JSON return structure.
//  --------------------------------------------------------------

//  --------------------------------------------------------------
#include "uint256.h"
#include "init.h"
#include "bitcoinrpc.h"
#include "build.h"
#include "profmac_0.h"
#include "profmac_1.h"

//  --------------------------------------------------------------

extern void ScriptPubKeyToJSON(const CScript& scriptDeNovoPubKey, json_spirit::Object& out);
extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, json_spirit::Object& entry);

//  --------------------------------------------------------------
//  -- END INCLUDE STUFF
//  -- BEGIN USER CUSTOMIZATION STUFF
//  --------------------------------------------------------------
bool bAuthentic_block_1 = true;  //toggle some exploratory behavior
//  --------------------------------------------------------------
CBigNum bnExtraNonce = 3;
unsigned int nBits = 0x1d00ffff;
std::vector<unsigned char> reservekey_GetReservedKey = ParseHex("0496b538e853519c726a2c91e61ec11600ae1390813a627c66fb8be7947be63c52da7589379515d4e0a604f8141781e62294721166bf621e73a82cbf2342c858ee"); 
std::string knownPayoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
//  --------------------------------------------------------------
//  -- http://gobittest.appspot.com/Address was interesting while I was working on this.
//  -- END USER CUSTOMIZATION STUFF
//  --------------------------------------------------------------




//  --------------------------------------------------------------
//  --------------------------------------------------------------
//  --  Implement "expt_block1" as an RPC command.
//  --------------------------------------------------------------
json_spirit::Value expt_block1(const json_spirit::Array& params, bool fHelp)
{
    if (fHelp || 3 < params.size())
        throw std::runtime_error(
            "expt_block1 [bAuthentic_block_1=false]\n"
            "Returns an json_spirit::Object containing mining-related information.");

//  --------------------------------------------------------------
//  --  Some worksite preparation & paperwork.
//  --
//  -- document into result
//  --------------------------------------------------------------
    if (params.size() > 0) bAuthentic_block_1 = ("true" == params[0].get_str());
    if(bAuthentic_block_1) bnExtraNonce = 3;  //else free running
    json_spirit::Object result;
//  --
    result.push_back(json_spirit::Pair("ProfMac", "expt_block1"));
    result.push_back(json_spirit::Pair(BUILD_DESC, BUILD_DATE));
    result.push_back(json_spirit::Pair("bAuthentic_block_1", bAuthentic_block_1));

//  --------------------------------------------------------------
//  --  create coinbase transaction de-novo for historic block 1
//  --    while doing final edits, I notice that
//  --    CWallet::CreateTransaction exists.
//  --------------------------------------------------------------
    static CReserveKey reservekey(pwalletMain);

    //
    // Create coinbase tx
    //
    CTransaction txDeNovo;
    txDeNovo.vin.resize(1);
    txDeNovo.vin[0].prevout.SetNull();
    txDeNovo.vin[0].scriptSig << nBits << ++bnExtraNonce;
    txDeNovo.vout.resize(0);

    CScript scriptDeNovoPubKey;
    if(bAuthentic_block_1) {
      scriptDeNovoPubKey = CScript() << reservekey_GetReservedKey << OP_CHECKSIG;
    } else {
      scriptDeNovoPubKey << reservekey.GetReservedKey() << OP_CHECKSIG;
      reservekey.KeepKey();
    }
    txDeNovo.vout.push_back(CTxOut(50 * COIN, scriptDeNovoPubKey));

//  --------------------------------------------------------------
//  --  document TxToJSON(txDeNovo) into result.
//  --------------------------------------------------------------
//  --
    result.push_back(json_spirit::Pair("ProfMac", "txDeNovo"));
    TxToJSON(txDeNovo, 0, result);

//  --------------------------------------------------------------
//  --  transform txDeNovo to hex à la getrawtransaction
//  --
//  --  document hex into result.
//  --------------------------------------------------------------
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << txDeNovo;
    int int_size = GetSizeOfCompactSize(ssTx.size());
    std::string hexDeNovo = HexStr(ssTx.begin(), ssTx.end());
//  --
    printf("profmac_1.cpp line 108: %s\n", hexDeNovo.c_str());
    result.push_back(json_spirit::Pair("ProfMac", "hex-DeNovo"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    result.push_back(json_spirit::Pair("bytes used to encode size", int_size));
    result.push_back(json_spirit::Pair("hex-datastream", hexDeNovo));

//  --------------------------------------------------------------
//  -- Verify Txn recovery from stream, 
//  -- Verify ability to edit, 
//  --
//  -- document into result
//  --------------------------------------------------------------
    CTransaction txVerify;
    ssTx >> txVerify;  //  -- test that (txDeNovo --> txVerify) is idempotent.
//  --
    result.push_back(json_spirit::Pair("ProfMac", "txVerify"));
    TxToJSON(txVerify, 0, result);


//  --------------------------------------------------------------
//  --------------------------------------------------------------
//  -- Serialize the getrawtransaction-format hex string.
//  -- Success at this step is the core goal of this exercise.
//  --
//  -- document into result
//  --------------------------------------------------------------
    ssTx.clear();
    ssTx << ParseHex(hexDeNovo);
    ssTx.ignore(GetSizeOfCompactSize(ssTx.size()));
//  --
    std::string hexGoalSuccess = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(json_spirit::Pair("ProfMac", "hexGoalSuccess"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    result.push_back(json_spirit::Pair("hexGoalSuccess", hexGoalSuccess));
//  --------------------------------------------------------------
//  --------------------------------------------------------------


//  --------------------------------------------------------------
//  -- Verify abilty to edit TxRecover.
//  --
//  -- document into result
//  --------------------------------------------------------------
    CTransaction txRecover;
    ssTx >> txRecover;
    txRecover.vout[0].nValue = 30 * COIN;
    txDeNovo.vin[0].scriptSig << nBits << ++bnExtraNonce;  //opportunity for timestamp
//  --
    result.push_back(json_spirit::Pair("ProfMac", "txRecover-edited"));
    TxToJSON(txRecover, 0, result);

//  --------------------------------------------------------------
//  --  Build newly stale block.
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Submit to network
//  --------------------------------------------------------------

    return result;
}




