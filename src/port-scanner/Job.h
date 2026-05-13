/*
 * Leopard-Watchdog - Port Scanner Module
 *
 * Module 2: Enumerates open ports, identifies services,
 * and provides security hardening recommendations.
 *
 * Copyright (c) 2026 Vugar Ahadli
 */

#ifndef JOB_H_
#define JOB_H_
#include<string>


using namespace std;
enum Status{ ASSIGNED,INPROGESS,COMPLETED,NOTNOW };
class Job {

public:
	string scanType;
	string port;
	string IP;
	Status jobStatus;
	string conclusion;
	string serviceName;
	string serviceVersion;
	string scanResult;

	Job();
	Job(string,string,string);
	void* (*funcPointer)(void*);
	Job* args;
	void setJob(void* (*funcPointer)(void *));
	void execute();
	~Job();
};



#endif /* JOB_H_ */
