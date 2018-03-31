//  --------------------------------------------------------------
//  --  Goal:  Read a coinbase, manipulate it, and re-emit it.
//  --    An experiment toward implementing a peer mining pool.
//  --
//  --  This code re-creates de-novo Block 1 on the main bitcoin chain,
//  --  transforms it to the getrawtransaction hex result,
//  --  recovers a txn from that hex, (main goal of this exercise)
//  --  edits the recovered txn,
//  --  and documents it all in a JSON return structure.
//  --------------------------------------------------------------

//  --------------------------------------------------------------
#include "uint256.h"
#include "profmac_0.h"
#include "profmac_1.h"
#include "init.h"
#include "bitcoinrpc.h"
#include "build.h"

//  --------------------------------------------------------------

using namespace std;

extern void ScriptPubKeyToJSON(const CScript& scriptDeNovoPubKey, json_spirit::Object& out);
extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, json_spirit::Object& entry);

//  --------------------------------------------------------------
//  -- END INCLUDE STUFF
//  -- BEGIN USER CUSTOMIZATION STUFF
//  --------------------------------------------------------------
bool bAuthentic_block_1 = false;  //toggle some exploratory behavior
//  --------------------------------------------------------------
string knownPayoutAddress = "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX";
CBigNum bnExtraNonce = 3;
unsigned int nBits = 0x1d00ffff;
//  --------------------------------------------------------------
//  -- END USER CUSTOMIZATION STUFF
//  --------------------------------------------------------------




//  --------------------------------------------------------------
//  --------------------------------------------------------------
//  --  Implement "expt_block1" as an RPC command.
//  --------------------------------------------------------------
json_spirit::Value expt_block1(const json_spirit::Array& params, bool fHelp)
{
    if (fHelp || 3 < params.size())
        throw runtime_error(
            "expt_block1 [bAuthentic_block_1=false]\n"
            "Returns an json_spirit::Object containing mining-related information.");

//  --------------------------------------------------------------
//  --  dump a lot of parameters, mostly to learn how they are handled
//  --  in the RPC system.
//  --------------------------------------------------------------
    json_spirit::Object result;
    result.push_back(json_spirit::Pair("ProfMac", "expt_block1"));
    result.push_back(json_spirit::Pair(BUILD_DESC, BUILD_DATE));
    result.push_back(json_spirit::Pair("params.size()", params.size()));
    result.push_back(json_spirit::Pair("parameters", params));
    json_spirit::Array arry;
    for (uint ix=0; ix < params.size(); ix++) {
      arry.push_back(params[ix].type());}
    result.push_back(json_spirit::Pair("parameter_types",arry));
//  --
    if (params.size() > 0) bAuthentic_block_1 = ("true" == params[0].get_str());
    if(bAuthentic_block_1) bnExtraNonce = 3;
    result.push_back(json_spirit::Pair("bAuthentic_block_1", bAuthentic_block_1));

//  --------------------------------------------------------------
//  --  create coinbase transaction de-novo for historic block 1
//  --    while doing final edits, I notice that
//  --    CWallet::CreateTransaction exists.
//  --    CWallet::SetReservedKey also.
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
      scriptDeNovoPubKey = CScript() << ParseHex("0496b538e853519c726a2c91e61ec11600ae1390813a627c66fb8be7947be63c52da7589379515d4e0a604f8141781e62294721166bf621e73a82cbf2342c858ee") << OP_CHECKSIG;
    } else {
      scriptDeNovoPubKey << reservekey.GetReservedKey() << OP_CHECKSIG;
      reservekey.KeepKey();
    }
    txDeNovo.vout.push_back(CTxOut(50 * COIN, scriptDeNovoPubKey));

//  --------------------------------------------------------------
//  --  document TxToJSON(txDeNovo) into result.
//  --------------------------------------------------------------
    result.push_back(json_spirit::Pair("ProfMac", "txDeNovo"));
    TxToJSON(txDeNovo, 0, result);

//  --------------------------------------------------------------
//  --  transform txDeNovo to hex à la getrawtransaction
//  --  document hex into result.
//  --------------------------------------------------------------
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << txDeNovo;
    result.push_back(json_spirit::Pair("ProfMac", "DeNovo-hex"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    string hexDeNovo = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(json_spirit::Pair("hex-datastream", hexDeNovo));

//  --------------------------------------------------------------
//  -- Verify Txn recovery from stream, 
//  -- Verify ability to edit, 
//  -- document into result
//  --------------------------------------------------------------
    CTransaction txVerify;
    ssTx >> txVerify;  //  -- test an idempotent operation.
    txVerify.vout[0].nValue = 20 * COIN;
    result.push_back(json_spirit::Pair("ProfMac", "txVerify"));
    TxToJSON(txVerify, 0, result);

//  --------------------------------------------------------------
//  -- Serialize getrawtransaction format hex string.
//  -- Success at this step is the core goal of this code.
//  --------------------------------------------------------------
    ssTx.clear();
    ssTx << ParseHex(hexDeNovo);
//  --
    ssTx.ignore(1); //still buggy, fix before release
    result.push_back(json_spirit::Pair("ProfMac", "trying to grok hex"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    hexDeNovo = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(json_spirit::Pair("compare-this-to-previous-hex", hexDeNovo));

//  --------------------------------------------------------------
//  -- Verify abilty to edit TxRecover.
//  --------------------------------------------------------------
    CTransaction txRecover;
    ssTx >> txRecover;  //  -- test an idempotent operation.
    txRecover.vout[0].nValue = 30 * COIN;
    
//  --------------------------------------------------------------
//  --  Document edits in JSON return value.
//  --------------------------------------------------------------
    result.push_back(json_spirit::Pair("ProfMac", "txRecover-edited"));
    TxToJSON(txRecover, 0, result);

//  --------------------------------------------------------------
//  --  Piggybacked exploratory doodles, off-mission, don't release.
//  --------------------------------------------------------------
    ssTx.clear();
    ssTx << ParseHex("01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff0c022f030101062f503253482fffffffff070165cd1d000000001976a91459524640e26292e52f06b8d4aefb342bf3dad02a88acc3194e7e000000001976a91405bdf8f93d0ec3aea163727f90a869d201bf18fe88ac860b310e000000001976a9146a4919132c79c83c4c5cd1667709df33cb9cc0ae88ac49914915000000001976a9149cea2b01fe9557ecc471abaadbc903993ae3d67488ac0c17621c000000001976a914d50554c616c019f23c20149c4aa9f22f047e2e2888accf9c7a23000000001976a914d8ce8783e108ce285edde3805575cce75a43beed88ac9222932a000000001976a914ed7f8072c69613b15918e99413e950e6954a52e688ac00000000");
    ssTx.ignore(0); 
    result.push_back(json_spirit::Pair("ProfMac", "bytecoin-hexStr"));
    result.push_back(json_spirit::Pair("ssTx.size", ssTx.size()));
    hexDeNovo = HexStr(ssTx.begin(), ssTx.end());
    result.push_back(json_spirit::Pair("bytecoin", hexDeNovo));

    ssTx >> txRecover;  //  -- test an idempotent operation.
    result.push_back(json_spirit::Pair("ProfMac", "bytecoin-txn"));
    TxToJSON(txRecover, 0, result);
    
//  --------------------------------------------------------------
//  --  Submit to network
//  --------------------------------------------------------------

//  --------------------------------------------------------------
//  --  Pro-forma template to be metamorphasized.
//  --------------------------------------------------------------

    return result;
}




