// Written by James Turner, started 2009.
//
// Copyright (C) 2009  Curtis L. Olson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <iostream>

#include <simgear/misc/sg_path.hxx>

#include <Navaids/fixlist.hxx>
//#include "ilslist.hxx"
#include <Navaids/navlist.hxx>
//#include "mkrbeacons.hxx"

#include "test_suite/dataStore.hxx"

#include "testnavs.hxx"


void NavaidsTests::testNavs()
{
    double heading, dist;

    DataStore &data = DataStore::get();

    FGNavList *current_navlist = new FGNavList;
    SGPath p_nav = data.getFGRoot() / "/Navaids/default.nav";

    current_navlist->init( p_nav );

    FGNavRecord *n;
    if ( (n = current_navlist->findByFreq( -93.2 * SG_DEGREES_TO_RADIANS,
                                           45.14 * SG_DEGREES_TO_RADIANS,
                                           3000, 117.30)) != NULL )
    {
        std::cout << "Found a vor station in range" << std::endl;
        std::cout << " id = " << n->get_ident() << std::endl;
    } else {
        std::cout << "not picking up vor. :-(" << std::endl;
    }

    FGNavRecord *dcs;
    if ( (dcs = current_navlist->findByIdent( "DCS",
                                              -3.3 * SG_DEGREES_TO_RADIANS,
                                              55.9 * SG_DEGREES_TO_RADIANS))
         != NULL ) {

        std::cout << "Found DCS by ident" << std::endl;
        if (dcs->get_freq() != 11520)
            std::cout << "Frequency for DCS VOR is wrong (should be 115.20), it's " 
                 << dcs->get_freq() << std::endl;
    } else {
        std::cout << "couldn't locate DCS (Dean-Cross) VOR" << std::endl;
    }

    // we have to init the marker beacon storage before we parse the ILS file
    FGMarkerBeacons *current_beacons = new FGMarkerBeacons;
    current_beacons->init();
	
    FGILSList *current_ilslist = new FGILSList;
    SGPath p_ils = data.getFGRoot() / "/Navaids/default.ils";
    current_ilslist->init( p_ils );
    FGILS *i = current_ilslist->findByFreq( -93.1 * SG_DEGREES_TO_RADIANS,
                                            45.24 * SG_DEGREES_TO_RADIANS,
                                            3000, 110.30);
    if ( i != NULL ) {
        std::cout << "Found an ils station in range" << std::endl;
        std::cout << " apt = " << i->get_aptcode() << std::endl;
        std::cout << " rwy = " << i->get_rwyno() << std::endl;
    } else {
        std::cout << "not picking up ils. :-(" << std::endl;
    }

    FGFixList *current_fixlist = new FGFixList;
    SGPath p_fix = data.getFGRoot() / "/Navaids/default.fix";
    current_fixlist->init( p_fix );
    FGFix fix;
	
    // attempting to get the position relative to the OTR VOR; heading
    // should be 108 degrees, distance 74nm (according to my SimCharts
    // v1.5)
    if ( current_fixlist->query_and_offset( "DOGGA",
                                            -0.103 * SG_DEGREES_TO_RADIANS,
                                            53.698 * SG_DEGREES_TO_RADIANS,
                                            3000, &fix, &heading, &dist) )
    {
        std::cout << "Found a matching fix" << std::endl;
        std::cout << " id = " << fix.get_ident() << std::endl;
        std::cout << " heading = " << heading << " dist = " << dist * SG_METER_TO_NM
             << std::endl;
    } else {
        CPPUNIT_FAIL("did not find fix. :-(");
    }
}
