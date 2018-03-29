//  --------------------------------------------------------------
//  --  Goal:  manipulate serialized transaction
//  --------------------------------------------------------------

#ifndef BYTECOIN_PROFMAC_1_H
#define  BYTECOIN_PROFMAC_1_H

#include "json/json_spirit_utils.h"
using namespace json_spirit;

extern uint256 hashGenesisBlockStd;
extern uint256 hashGenesisBlockTest;
extern uint256 hashGenesisBlock;
extern uint256 hashTarget;
extern const char chHashTarget[];
extern const char chPreviousBlockHash[];
extern json_spirit::Value deserializetransaction(const Array& params, bool fHelp);

#endif
