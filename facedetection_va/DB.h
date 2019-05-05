/*
 * DB.h
 *
 *  Created on: Sep 19, 2018
 *      Author: jednipat
 */

#ifndef DB_H_
#define DB_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class DB {
public:
	DB();
	virtual ~DB();

	void updateFaceInfo(string sFaceID, string sFaceFound, string sFaceWidth, string sFaceHeight);

private:
	bool _bFaceFound;
	int _iFaceWidth;
	int _iFaceHeight;

	sql::Driver *_driver;
	sql::Connection *_con;
	sql::Statement *_stmt;
	sql::ResultSet *_res;

};

#endif /* DB_H_ */
