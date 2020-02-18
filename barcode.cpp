#include "barcode.h"
#include "json.h"
#include <iostream>
#include "mqClient.h"
#include "boxconfig.h"
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
	cout << "confirmed, content is " << preStr << endl;
    cout << "json is " << packet << endl;
    mqClient::getInstance().publish("command/x/test", packet);
}

string barcode::generatePacket(const string& id)
{
    string cfgFile = "common.properties";
    Config cfg;
	if (cfg.FileExist(cfgFile))
	{
		cfg.ReadFile(cfgFile);
	}
    else
    {
        cout  << "read  file fail " << endl;
    }
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
