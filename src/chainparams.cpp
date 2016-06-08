// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Owncoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0, uint256("0x00000205a2b36c89d27547db95cdf4a3038662dd26cf21af8b9c8978a64b06a2"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1449579900, // * UNIX timestamp of last checkpoint block
        0,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        2800        // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("00000416024caa7a7900f9a73c120660773ca4019d0671ee5d5bb528acbaa633"))
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1449579901,
        0,
        500
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0x17a08f326f61be14e0502edeb93d46013596a0bf1d8ddd9d5909f37dab7305a4"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x1c;
        pchMessageStart[1] = 0xbd;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0x4f;
        vAlertPubKey = ParseHex("04f4ef416b8597cbadde2216eb0d4fc5137075a3fa3442b71b4ea97efca14e885aa75911b7ef3e6353281d2515a375e8a510455214031df1ac46e443e1ea9a9647");
        nDefaultPort = 9887;
        bnProofOfWorkLimit = ~uint256(0) >> 20;  // Owncoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // Owncoin: 1 day
        nTargetSpacing = 2.5 * 60; // Owncoin: 2.5 minutes

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         * 
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "I create my owncoin 07/12/2015 for test , End its work for me. I hope this guide help you";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0436b2d4b1a72bcad8a8f9b8d12048014bcbbb21df91d84ed172cbcfe8aa6f0e9da5e995fa3b8c63da129c247b73cdd55bac80abc2b67adfbf88381a9889e64177") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1449579900;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 2087222;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000205a2b36c89d27547db95cdf4a3038662dd26cf21af8b9c8978a64b06a2"));
        assert(genesis.hashMerkleRoot == uint256("0xb7ba7a0616705a327d39f6bf791c7db48abe4c79a11b62c6d690816eddc4d730"));

        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of( 76);                    // Owncoin addresses start with 'X'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 16);                    // Owncoin script addresses start with '7'
        base58Prefixes[SECRET_KEY] =     list_of(204);                    // Owncoin private keys start with '7' or 'X'
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0xFE)(0x52)(0xF8); // Owncoin BIP32 pubkeys start with 'drkv'
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0xFE)(0x52)(0xCC); // Owncoin BIP32 prvkeys start with 'drkp'
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000005);             // Owncoin BIP44 coin type is '5'

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04594d9bcfd432ed21603c81dc6a2cbdc58ded77217ecd91c0420737f60a1948871a66bb84bff26e0f5dd447ee931b5213bca0d9de686c49f6f03545a03fd79b75";
        strMasternodePaymentsPubKey = "04594d9bcfd432ed21603c81dc6a2cbdc58ded77217ecd91c0420737f60a1948871a66bb84bff26e0f5dd447ee931b5213bca0d9de686c49f6f03545a03fd79b75";
        strDarksendPoolDummyAddress = "Xq19GqFvajRrEdDHYRKGYjTsQfpV5jyipF";
        nStartMasternodePayments = 1449579900; //Wed, 25 Jun 2014 20:36:16 GMT
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x1f;
        pchMessageStart[1] = 0xbc;
        pchMessageStart[2] = 0xc7;
        pchMessageStart[3] = 0x4c;
        vAlertPubKey = ParseHex("04b0105bfdf3a50cc51a503caeb23526d62b7fcc3e5bd6f451eb29fd66846714ba3af73ecc317d9b25df525f03b76e22d14a8e7d8631fcc89cee9122a1cc6420c0");
        nDefaultPort = 19887;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // Owncoin: 1 day
        nTargetSpacing = 2.5 * 60; // Owncoin: 2.5 minutes

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1449579901;
        genesis.nNonce = 1899912;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000416024caa7a7900f9a73c120660773ca4019d0671ee5d5bb528acbaa633"));

        vFixedSeeds.clear();
        vSeeds.clear();
        /*vSeeds.push_back(CDNSSeedData("mycointest.io", "testnet-seed.mycointest.io"));
        vSeeds.push_back(CDNSSeedData("owncoin.qa", "testnet-seed.owncoin.qa"));
        *///legacy seeders
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(139);                    // Testnet owncoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 19);                    // Testnet owncoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY]     = list_of(239);                    // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x3a)(0x80)(0x61)(0xa0); // Testnet owncoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x3a)(0x80)(0x58)(0x37); // Testnet owncoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000001);             // Testnet owncoin BIP44 coin type is '5' (All coin's testnet default)

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "041edf37d10503df7213bef9c07a13c1c94afe70ed8a4c85a79a658097bb36766e0330736acf2e921a3b97e507b270d779b454cbd4e7adf7032d9b02a37d5d680a";
        strMasternodePaymentsPubKey = "041edf37d10503df7213bef9c07a13c1c94afe70ed8a4c85a79a658097bb36766e0330736acf2e921a3b97e507b270d779b454cbd4e7adf7032d9b02a37d5d680a";
        strDarksendPoolDummyAddress = "y1EZuxhhNMAUofTBEeLqGE1bJrpC2TWRNp";
        nStartMasternodePayments = 1449579901; //Fri, 09 Jan 2015 21:05:58 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xdc;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xb3;
        pchMessageStart[3] = 0x4c;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Owncoin: 1 day
        nTargetSpacing = 2.5 * 60; // Owncoin: 2.5 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1449579902;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 19883;
        assert(hashGenesisBlock == uint256("0x17a08f326f61be14e0502edeb93d46013596a0bf1d8ddd9d5909f37dab7305a4"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18334;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
