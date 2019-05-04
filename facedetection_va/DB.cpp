/*
 * DB.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: jednipat
 */

#include "DB.h"
#include <iostream>
using namespace std;

int DB::_iObjCount = 0;
ErrorHandler* DB::_pErrorHandler = NULL;
Logger* DB::_pLogger = NULL;
bool DB::_bIsSetErrorDiagnosticsList = false;


void DB::initErrorDiagnosticsList()
{
    if (!DB::_pErrorHandler) DB::_pErrorHandler = ErrorHandler::getInstance();
    // Assign ErrorCode for this class in the document below:
    // https://docs.google.com/spreadsheets/d/1U8OHmdtxA618jtciWHUr70zJR0wZqTG8smuaCYS3WCU/edit#gid=1777797050
    if (!DB::_bIsSetErrorDiagnosticsList)
    {
#define DB_ERROR_ACCESSING_DATABASE                 100301
        _pErrorHandler->insertErrorDiagnostics(
                (int) DB_ERROR_ACCESSING_DATABASE,
                string("DB : Error accessing database."));
#define DB_INVALID_QUERY                            100302
        _pErrorHandler->insertErrorDiagnostics((int) DB_INVALID_QUERY,
                string("DB : Invalid query."));
#define DB_INVALID_RESULT                           100303
        _pErrorHandler->insertErrorDiagnostics((int) DB_INVALID_RESULT,
                string("DB : Invalid result."));
#define DB_DB_CONFIG_FILE_NOT_SET                   100304
        _pErrorHandler->insertErrorDiagnostics((int) DB_DB_CONFIG_FILE_NOT_SET,
                string("DB : DB config file not set."));
#define DB_DB_NOT_CONNECTED                         100305
        _pErrorHandler->insertErrorDiagnostics((int) DB_DB_NOT_CONNECTED,
                string("DB : Not connected to database."));
#define DB_STATION_INFO_NOT_SET                     100306
        _pErrorHandler->insertErrorDiagnostics((int) DB_STATION_INFO_NOT_SET,
                string("DB : Station information is not set."));

        // Escapable Exceptions
    }

    DB::_bIsSetErrorDiagnosticsList = true;
}

DB::DB() {
	 DB::_iObjCount++;
	_pLogger = Logger::getInstance();
	_bIsSetDBConfigFile = false;
	_bIsConnectedToDB = false;
	_pClock = new Clock();
	_pTimer = new Timer();
	_pUtilities = new Utilities();
	_pDBConnectorMySQL = new DBConnectorMySQL();
	_pMySQL_Conn = NULL;

	// Values to be read
	_iMaxEntryID = 0;

	_ssOut.str("");

}

DB::~DB() {
	DB::_iObjCount--;
	if (_pErrorHandler) _pErrorHandler = NULL;
	if (_pLogger) _pLogger = NULL;

	if (_fsDBConfigFileRead.isOpened()) _fsDBConfigFileRead.release();
	_pMySQL_Conn = NULL;
	_pDBConnectorMySQL->close();
	if (_pDBConnectorMySQL) delete _pDBConnectorMySQL;

	if (_pClock) delete _pClock;
	if (_pTimer) delete _pTimer;
	if (_pUtilities) delete _pUtilities;
}

void DB::updateParamsFromConfig()
{
    DB::setDBConfigFile("../../../input/dbconfig.yml");
}

string DB::getOutputFolder()
{
    return string(_pLogger->getOutputFolder());
}

void DB::setDBConfigFile(string pchDBConfigFile)
{
    _bIsSetDBConfigFile = true;
    _sDBConfigFile = pchDBConfigFile;
    if (!_fsDBConfigFileRead.isOpened())
    {
        _fsDBConfigFileRead.open(_sDBConfigFile, FileStorage::READ);
        // Read the database config into variables right here.
        cv::FileNode nodeDBInfo = _fsDBConfigFileRead["dbinfo"];
        _sHost = (string)nodeDBInfo["host"];
        _sDatabase = (string)nodeDBInfo["db"];
        _sUser = (string)nodeDBInfo["user"];
        _sPwd = (string)nodeDBInfo["pwd"];
    }
}


void DB::connectToDB()
{
    if (!_bIsConnectedToDB)
    {
        _pDBConnectorMySQL->setHost(_sHost);
        _pDBConnectorMySQL->setDatabase(_sDatabase);
        _pDBConnectorMySQL->setUsername(_sUser);
        _pDBConnectorMySQL->setPwd(_sPwd);
        _pDBConnectorMySQL->connect();
        _pMySQL_Conn = _pDBConnectorMySQL->getMySQLConn();
        _bIsConnectedToDB = true;
    }
    else cout << "Already connected to database." << endl;
}

void DB::checkIfConnectedToDB(bool bDebug)
{
    if (!_bIsConnectedToDB) _pErrorHandler->setErrorCode(
            (int) DB_DB_NOT_CONNECTED);
}

void DB::insertEvent(string sFirstImgPath, string sLastImgPath, string sFirstCroppedImgPath, string sLastCroppedImgPath,
		long long int iFirstFrame, long long int iLastFrame, bool bDebug)
{
	checkIfConnectedToDB();
	MYSQL_RES *pMySQL_Result = NULL;
	MYSQL_ROW row;
	stringstream ssFirstFrame;
	stringstream ssLastFrame;
	ssFirstFrame << iFirstFrame;
	ssLastFrame << iLastFrame;
	//INSERT INTO `event_snapshots` (`id`, `imgPath`, `label`, `created_at`, `camera_id`) VALUES (NULL, 'etet', 'test', NULL, '3');
	string sQuery = string("INSERT INTO `detections` (`id`,`starFrame`, `endFrame`, `firstImgPath`, `lastImgPath`, `created_at`, `updated_at`, `firtCroppedImgPath`, `lastCroppedImgPath`) "
			"VALUES (NULL, "+ssFirstFrame.str()+", "+ssLastFrame.str()+",'"+sFirstImgPath+"','"+sLastImgPath+"', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP,'"+sFirstCroppedImgPath+"','"+sLastCroppedImgPath+"' );");
	if (bDebug) cout << "Query: " << sQuery << endl;
	if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
	{
		unsigned long long iRetID = mysql_insert_id(_pMySQL_Conn);

		if(bDebug)
		cout << "Inserted!. " << endl;
	}
	else
	{
		cerr << "mysql_error in setStation() -- invalid query: #"
				<< mysql_errno(_pMySQL_Conn) << " - "
				<< mysql_error(_pMySQL_Conn) << endl;
		_pErrorHandler->setErrorCode((int) DB_INVALID_QUERY, sQuery);
	}
}

void DB::insertEventWithAgeGenderSentiment(string sFirstImgPath, string sLastImgPath, string sFirstCroppedImgPath, string sLastCroppedImgPath,
		string sAgeID, string sGenderID, string sSentimentID, long long int iFirstFrame, long long int iLastFrame, bool bDebug)
{
	checkIfConnectedToDB();
	MYSQL_RES *pMySQL_Result = NULL;
	MYSQL_ROW row;
	stringstream ssFirstFrame;
	stringstream ssLastFrame;
	ssFirstFrame << iFirstFrame;
	ssLastFrame << iLastFrame;
	//INSERT INTO `event_snapshots` (`id`, `imgPath`, `label`, `created_at`, `camera_id`) VALUES (NULL, 'etet', 'test', NULL, '3');
	string sQuery = string("INSERT INTO `detections` (`id`,`starFrame`, `endFrame`, `firstImgPath`, `lastImgPath`, `created_at`, `updated_at`, `firtCroppedImgPath`, `lastCroppedImgPath`) "
			"VALUES (NULL, '"+ssFirstFrame.str()+"', '"+ssLastFrame.str()+"','"+sFirstImgPath+"','"+sLastImgPath+"', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP,'"+sFirstCroppedImgPath+"','"+sLastCroppedImgPath+"' );");
	if (bDebug) cout << "Query: " << sQuery << endl;
	if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
	{
		unsigned long long iRetID = mysql_insert_id(_pMySQL_Conn);
		stringstream ssDetectionID;
		ssDetectionID << iRetID;
		string sQuery = string("INSERT INTO `face_analyses` (`id`, `faceImagePath`, `age`, `gender`, `sentiment`, `created_at`, `updated_at`, `detection_id` )"
				"VALUES (NULL, '"+sFirstCroppedImgPath+"',"+sAgeID+","+sGenderID+","+sSentimentID+", CURRENT_TIMESTAMP, CURRENT_TIMESTAMP,"+ssDetectionID.str()+" )");
		std::cout << "sQuery: " << sQuery << std::endl;

		if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
		{
			if(bDebug)
			cout << "Inserted!. " << endl;
		}else{
			std::cout << "face_analyses data is not inserted." << std::endl;
		}
	}
	else
	{
		cerr << "mysql_error in setStation() -- invalid query: #"
				<< mysql_errno(_pMySQL_Conn) << " - "
				<< mysql_error(_pMySQL_Conn) << endl;
		_pErrorHandler->setErrorCode((int) DB_INVALID_QUERY, sQuery);
	}
}

void DB::insertEventWithRecognition( string sFirstImgPath, string sLastImgPath,
		string sFirstCroppedImgPath, string sLastCroppedImgPath,
		string sFirstLabelID, string sSecondLabelID, string sThirdLabelID,
		string sFirstConfValue, string sSecondConfValue, string sThirdConfValue,
		long long int iFirstFrame, long long int iLastFrame, bool bDebug )
{
	checkIfConnectedToDB();
	MYSQL_RES *pMySQL_Result = NULL;
	MYSQL_ROW row;
	stringstream ssFirstFrame;
	stringstream ssLastFrame;
	ssFirstFrame << iFirstFrame;
	ssLastFrame << iLastFrame;
	//INSERT INTO `event_snapshots` (`id`, `imgPath`, `label`, `created_at`, `camera_id`) VALUES (NULL, 'etet', 'test', NULL, '3');
	string sQuery = string("INSERT INTO `detections` (`id`,`starFrame`, `endFrame`, `firstImgPath`, `lastImgPath`, `created_at`, `updated_at`, `firtCroppedImgPath`, `lastCroppedImgPath`) "
			"VALUES (NULL, '"+ssFirstFrame.str()+"', '"+ssLastFrame.str()+"','"+sFirstImgPath+"','"+sLastImgPath+"', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP,'"+sFirstCroppedImgPath+"','"+sLastCroppedImgPath+"' );");
	if (bDebug) cout << "Query: " << sQuery << endl;
	if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
	{
		unsigned long long iRetID = mysql_insert_id(_pMySQL_Conn);
		stringstream ssDetectionID;
		ssDetectionID << iRetID;
		string sQuery = string("INSERT INTO `facerecognitions` (`id`, `filename`, `detectedfacefilename`, `similarno1`, `confno1`, `similarno2`, `confno2`, `similarno3`, `confno3`, `created_at`, `updated_at`, `detection_id` )"
				"VALUES (NULL, '', '"+sFirstCroppedImgPath+"', "+sFirstLabelID+","+sFirstConfValue+","+sSecondLabelID+","+sSecondConfValue+","+sThirdLabelID+","+sThirdConfValue+", CURRENT_TIMESTAMP, CURRENT_TIMESTAMP,"+ssDetectionID.str()+" )");
		std::cout << "sQuery: " << sQuery << std::endl;

		if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
		{
			if(bDebug)
			cout << "Inserted!. " << endl;
		}else{
			std::cout << "face_analyses data is not inserted." << std::endl;
		}
	}
	else
	{
		cerr << "mysql_error in setStation() -- invalid query: #"
				<< mysql_errno(_pMySQL_Conn) << " - "
				<< mysql_error(_pMySQL_Conn) << endl;
		_pErrorHandler->setErrorCode((int) DB_INVALID_QUERY, sQuery);
	}
}

void DB::insertFace(string sFirstname, string sLastname, string sGender, string sLabel,
		string sFilename, string sFaceimage, string description, bool bDebug)
{
	checkIfConnectedToDB();
	MYSQL_RES *pMySQL_Result = NULL;
	MYSQL_ROW row;

	//INSERT INTO `event_snapshots` (`id`, `imgPath`, `label`, `created_at`, `camera_id`) VALUES (NULL, 'etet', 'test', NULL, '3');
	string sQuery = string("INSERT INTO `faceapp_development`.`faces`  (`id`,`firstname`, `lastname`, `gender`, `label`, `filename`, `faceimage`, `description`, `created_at`, `updated_at`) "
			"VALUES (NULL, '"+sFirstname+"', '"+sLastname+"',"+sGender+","+sLabel+",'"+sFilename+"','"+sFaceimage+"','"+description+"', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP );");
	if (bDebug) cout << "Query: " << sQuery << endl;
	if (!mysql_query(_pMySQL_Conn, sQuery.c_str()))
	{
		cout << "Inserted!. " << endl;
	}
	else
	{
		cerr << "mysql_error in setStation() -- invalid query: #"
				<< mysql_errno(_pMySQL_Conn) << " - "
				<< mysql_error(_pMySQL_Conn) << endl;
		_pErrorHandler->setErrorCode((int) DB_INVALID_QUERY, sQuery);
	}
}
