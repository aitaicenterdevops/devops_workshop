/*
 * DB.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: jednipat
 */

#include "DB.h"

DB::DB() {
	_bFaceFound = false;
	_iFaceWidth = -1;
	_iFaceHeight = -1;

	
	try {

		/* Create a connection */
		_driver = get_driver_instance();
		_con = _driver->connect("tcp://127.0.0.1:3306", "root", "csimAIT");
		/* Connect to the MySQL test database */
		_con->setSchema("FAAS_development");

		_stmt = _con->createStatement();

	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

}

DB::~DB() {
	delete _res;
	delete _stmt;
	delete _con;

}

void DB::updateFaceInfo(string sFaceID, string sFaceFound, string sFaceWidth, string sFaceHeight)
{
	string sSQLUpdate = "UPDATE fimage64s SET faceFound = "+sFaceFound+" , faceWidth ="+sFaceWidth+", faceHeight="+sFaceHeight+" WHERE ID = "+sFaceID+";";
	std::cout << "sSQLUpdate" << sSQLUpdate << std::endl;
	_stmt->executeUpdate(sSQLUpdate.c_str());
}
