#include "barcode.h"
#include "json.h"
#include <iostream>
#include "mqClient.h"
#include "boxconfig.h"
#include <regex.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
using namespace std;

const string confirmStr = "ISESOL";
const string confirmStr2 = "isesol";

barcode::barcode()
{
    mqClient::setContext(this);
    mqClient::getInstance().connect();
}

void barcode::onGetStr(const string& str)
{
    if(str != confirmStr && str != confirmStr2)
    {
        cout << "getStr is " << str << endl;
        preStr = str;
        return;
    }
    string packet = generatePacket(preStr);
    if(packet.empty())
    {
		cout << "empty" << endl;
		return;
    }
	cout << "confirmed, content is " << preStr << endl;
    cout << "json is " << packet << endl;
    mqClient::getInstance().publish("command/x/test", packet);
}

string barcode::generatePacket(const string& id)
{
    string cfgFile = "/home/isesol/config/common/common.properties";
    Config cfg;
	if (cfg.FileExist(cfgFile))
	{
		cfg.ReadFile(cfgFile);
	}
    else
    {
        cout  << "read  file fail " << endl;
    }

//reg
    regex_t reg;
    const char* patten = "([a-zA-Z0-9]+)-([a-zA-Z0-9]+)-([a-zA-Z0-9]+)-([a-zA-Z0-9]+)-([a-zA-Z0-9]+)";
    //const char* patten = "(dr[^\\s]+)\\s+([\\d]+)\\s+.*";
    if(0 > regcomp(&reg, patten, REG_EXTENDED))
    {
        cout << "reg comp fail" << endl;
        return "";
    }
    char match[100]="hello";
    string des[6];

    int nmatch = 6;
    regmatch_t matchptr[32];
    int err = regexec (&reg, id.c_str(), nmatch, matchptr, REG_NOTBOL);
    if(err == REG_NOMATCH)
    {
        cout << "no match " << endl;
        return "";
    }
    for(int i=0;i<6 && matchptr[i].rm_so!=-1;i++){
        int len = matchptr[i].rm_eo-matchptr[i].rm_so;
        cout << "rm_so: " << matchptr[i].rm_so << " rm_eo: " << matchptr[i].rm_eo << endl;
        if(len){
            memset(match,'\0',sizeof(match));
            strncpy(match,id.c_str()+matchptr[i].rm_so,len);
            des[i]=match;
            printf("match %d is %s\n",i,match);
        }
    }
//reg

    string machineno = cfg.Read<string>("machineno", "");
    cout << "machineno is " <<  machineno << endl;
    Json::Value val;
    val["encode"] = "false";
    val["id"] = "123";
    val["machineNo"] = "A131420035";
    val["type"] = "20";
    val["order"] = 253;
    val["dest"] = "A131420035";
        Json::Value content;
        content["cmdId"] = "toolCompensationfilwDownload";
        content["concurrent"] = "true";
            Json::Value data;
            data["toolIdentifier"] = id;
            data["PT-L-A-ID"] = des[1];
            data["deviceID"] = des[2];
            data["optSeq"] = des[3];
            data["toolNo"] = des[4];
            data["seqID"] = des[5];
        content["data"] = data;
    val["content"] = content;
    return val.toStyledString();
    
   
}
void barcode::messageArrived(char* topicName, int topicLen, MQTTAsync_message* message)
{

}

void barcode::connectLost(char* cause)
{

}

void barcode::connectSuccess(MQTTAsync_successData* response)
{
    cout << "barcode connect mq success" << endl;
}

void barcode::connectFail(MQTTAsync_failureData* response)
{
    cout << "barcode connect mq fail" << endl;
}

void barcode::subscribeOk(MQTTAsync_successData* response)
{

}

void barcode::subscribeFail()
{

}
