//  --------------------------------------------------------------
//  --  Goal:  manipulate serialized transaction
//  --------------------------------------------------------------

#ifndef BYTECOIN_PROFMAC_1_H
#define  BYTECOIN_PROFMAC_1_H

#include "json/json_spirit_utils.h"
using namespace json_spirit;

extern json_spirit::Value deserializetransaction(const Array& params, bool fHelp);

#endif
