/*
 * DB.h
 *
 *  Created on: Sep 19, 2018
 *      Author: jednipat
 */

#ifndef DB_H_
#define DB_H_

#include <cv.h>
using namespace cv;

#include <sstream>

#include "Clock.h"
#include "DBConnectorMySQL.h"
#include "ErrorHandler.h"
#include "Logger.h"
#include "Timer.h"
#include "Utilities.h"

class DB {
public:
	DB();
	virtual ~DB();

    void updateParamsFromConfig();
    static void initErrorDiagnosticsList();
    static string getOutputFolder();
    void setDBConfigFile(string pchDBConfigFile);

    // Getters and Setters
	static int getObjCount();
	int getMaxEntryID(bool bDebug = false);
	bool setStation(int iStationID, bool bDebug = false); // loads station info
	int getStationID();

	void connectToDB();
	void checkIfConnectedToDB(bool bDebug = false);
	bool loadStationInfo(int iStationID, bool bDebug = false);
	void checkIfStationInfoSet(bool bDebug = false);

	void insertEvent(string sFirstImgPath, string sLastImgPath,  string sFirstCroppedImgPath, string sLastCroppedImgPath,
			long long int iFirstFrame, long long int iLastFrame, bool bDebug = false);

	void insertEventWithAgeGenderSentiment(string sFirstImgPath, string sLastImgPath,
			string sFirstCroppedImgPath, string sLastCroppedImgPath,
			string sAgeID, string sGenderID, string sSentimentID,
			long long int iFirstFrame, long long int iLastFrame, bool bDebug = false);
	void insertEventWithRecognition(string sFirstImgPath, string sLastImgPath,
			string sFirstCroppedImgPath, string sLastCroppedImgPath,
			string sFirstLabelID, string sSecondLabelID, string sThirdLabelID,
			string sFirstConfValue, string sSecondConfValue, string sThirdConfValue,
			long long int iFirstFrame, long long int iLastFrame, bool bDebug = false);

	void insertFace(string sFirstname, string sLastname, string sGender, string sLabel,
			string sFilename, string sFaceimage, string description, bool bDebug = false);

private:
    static int _iObjCount;
	static ErrorHandler *_pErrorHandler;
	static Logger *_pLogger;
    static bool _bIsSetErrorDiagnosticsList;

    Clock *_pClock;
	Timer *_pTimer;
	Utilities *_pUtilities;
	DBConnectorMySQL *_pDBConnectorMySQL;
	MYSQL *_pMySQL_Conn;

	// Values to be read
	int _iMaxEntryID;

	stringstream _ssOut;

	// Database config variables
	bool _bIsSetDBConfigFile;
	bool _bIsConnectedToDB;
	string _sDBConfigFile;
	FileStorage _fsDBConfigFileRead;
	string _sHost;
	string _sDatabase;
	string _sUser;
	string _sPwd;

};

#endif /* DB_H_ */
