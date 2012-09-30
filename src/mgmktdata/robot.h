/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 06 NOV 2011
 * Purpose				: MG_Robot is a class dealing with market data
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgmktdata/marketdata.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_Robot : public MG_XLObject
{
	typedef std::map<std::string,MG_MarketDataPtr> mapStrMkt;
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Robot)

	ASSIGN_OPERATOR(MG_Robot)
	SWAP_DECL(MG_Robot)
	CLONE_METHOD(MG_Robot)

	MG_Robot(const MG_Date& aAsOf, const std::vector<MG_MarketDataPtr>& aMktData);

	virtual ~MG_Robot(void);

public:
	/* Engine */
	void AddMktData(const MG_MarketDataPtr& aMktData);
	MG_MarketDataPtr GetMktData(const std::string& aType, const std::string& aCcy, const std::string& aUnderIndex);

private:
	std::string CreateKey(const MG_MarketData& aMktData) const;

private:
	MG_Date							myAsOf;
	std::vector<MG_MarketDataPtr>	myMktData;
	mapStrMkt						myKeyData;
};


MG_NAMESPACE_END
