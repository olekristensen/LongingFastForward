//
//  Logger.h
//  lffCinderCapture
//
//  Created by Ole Kristensen on 2012-07-15.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lffCinderCapture_Logger_h
#define lffCinderCapture_Logger_h

#include "boost/format.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


static void LogStr(std::string lStr){
    
    boost::posix_time::ptime uTime = boost::posix_time::microsec_clock::universal_time();
    
    std::cout << "[" << boost::posix_time::to_iso_extended_string(uTime) << "] " << lStr << std::endl;
    
}


#endif
